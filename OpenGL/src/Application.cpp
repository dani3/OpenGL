#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string file)
{
	std::ifstream stream(file);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int) type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << 
			((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);

		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing GLEW" << std::endl;
		return 0;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray* va = new VertexArray();
	VertexBuffer* vb = new VertexBuffer(positions, 4 * 2 * sizeof(float));
	VertexBufferLayout* layout = new VertexBufferLayout();
	layout->Push<float>(2);
	va->AddBuffer(*vb, *layout);
	
	IndexBuffer* ib = new IndexBuffer(indices, 6);
	ib->Bind();

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);

	float r = 0.0f;
	float increment = 0.05f;
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, .2f, .5f, 1.0f));

		va->Bind();
		ib->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (r > 1.0f)
		{
			increment = -.05f;
		}
		else if (r < 0.0f)
		{
			increment = .05f;
		}

		r += increment;

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	delete vb;
	delete ib;
	delete layout;

	glfwTerminate();
	return 0;
}
