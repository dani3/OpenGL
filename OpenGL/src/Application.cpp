#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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

	Shader* shader = new Shader("res/shaders/Basic.shader");
	shader->Bind();

	va->Unbind();
	vb->Unbind();
	ib->Unbind();
	shader->Unbind();

	Renderer* renderer = new Renderer();

	float r = 0.0f;
	float increment = 0.05f;
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer->Clear();

		shader->Bind();
		shader->SetUniform4f("u_Color", r, .2f, .5f, 1.0f);

		renderer->Draw(*va, *ib, *shader);

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

	delete vb;
	delete ib;
	delete layout;
	delete shader;
	delete renderer;

	glfwTerminate();
	return 0;
}
