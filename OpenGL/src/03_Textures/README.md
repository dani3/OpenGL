# Textures

Basic steps to use a __texture__ in OpenGL:

1. Load the image, meaning getting a pointer to a buffer containing all the RGB pixels.
2. Upload and bind the image to the GPU.
3. Modify the shader to read from that texture memory.

## Loading the texture

First, we need to generate the texture:

```c++
// Generate one texture
glGenTextures(1, &m_RendererID);
// Bind it
glBindTexture(GL_TEXTURE_2D, m_RendererID);
```

Then, we need to setup some settings for the texture:

```c++
// Set how it will resample down if the image is bigger than the area.
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// Set how it will resample up if the image is smaller than the area.
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Set up the wrap mode, we want it to not extend the area. (S, T) -> (X, Y)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
```

Finally, load the image:

```c++
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
```

## Binding the texture

First we need to bind it and specify the slot. Basically, we need to tell OpenGL that the next texture I bind, will be bound to a certain slot.

```c++
glActiveTexture(GL_TEXTURE0 + slot);
glBindTexture(GL_TEXTURE_2D, m_RendererID);
```

## Textures coordinates

Texture coordinates are used in the rasterization to sample a certain area from that texture to retrieve what color that pixel should be:

Basically we should specify for each vertex, what area of the texture it should be, like so:

```c++
float positions[] = {
    -0.5f, -0.5f, 0.0f, 0.0f,  // Bottom left corner
     0.5f, -0.5f, 1.0f, 0.0f,  // Top left corner
     0.5f,  0.5f, 1.0f, 1.0f,  // Top right corner
    -0.5f,  0.5f, 0.0f, 1.0f   // Bottom right corner
};
```

Then, we need to tell our shader what texture slot it should sample from using a uniform:

```c++
glUniform1i(GetUniformLocation("u_Texture"), slot);
```

And modify our shader to receive those texture coordinates:

```c++
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

// Pass the texture coordinates to the fragment shader.
out vec2 v_TexCoord;

void main()
{
    gl_Position = position;
    v_TexCoord = texCoord;
};
```

```c++
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

// Receive the texture coordinates from the vertex shader
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
};
```

---
Note: textures in OpenGL start from the bottom left, so we need to flip it upside down.
