#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RenderTexture.hpp"

using namespace Brickware;
using namespace Graphics;

/*
	All Private
*/

#ifdef GL_SUPPORT

void RenderTexture::InitGL()
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	GLuint textureType = GL_TEXTURE_2D;

	if (type == TextureType::TEX_3D)
		textureType = GL_TEXTURE_3D;
	else if (type == TextureType::TEX_CUBE_MAP)
		textureType = GL_TEXTURE_CUBE_MAP;

	//Setup depth texture if we wanted one
	if (depth > 0)
	{
		glGenTextures(1, &depthBufferTexture);
		glBindTexture(textureType, depthBufferTexture);
	
		GLint glDepth = GL_DEPTH_COMPONENT16;
		if (depth == 24)
			glDepth = GL_DEPTH_COMPONENT24;
	
		//Create internal texture based on settings
		if (type == TextureType::TEX_2D)
		{
			glTexImage2D(textureType, 0, glDepth, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
		if (type == TextureType::TEX_3D)
		{
			//Not implemented
			//glTexImage3D(textureType, 0, glDepth, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
		if (type == TextureType::TEX_CUBE_MAP)
		{
			for (unsigned int i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
				NULL);
		}
		
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Set texture to framebuffer based on texture type
		if (type == TextureType::TEX_2D)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferTexture, 0);
		if (type == TextureType::TEX_3D)
			glFramebufferTexture3D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_3D, depthBufferTexture, 0, 0);
		if (type == TextureType::TEX_CUBE_MAP)
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferTexture, 0);

		// Check that the buffer is OK
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "RenderTexture depth buffer encountered an error!"
			<< std::endl;
	}

	//Setup color texture
	glGenTextures(1, &colorBufferTexture);
	glBindTexture(textureType, colorBufferTexture);

	GLint glFormat = GL_RGBA;

	switch (format)
	{
	case TextureFormat::RGBA:
		glFormat = GL_RGBA;
		break;
	case TextureFormat::BGRA:
		glFormat = GL_BGRA;
		break;
	case TextureFormat::RGB:
		glFormat = GL_RGB;
		break;
	case TextureFormat::BGR:
		glFormat = GL_BGR;
		break;
	}

	//Create internal texture based on settings
	if (type == TextureType::TEX_2D)
	{
		glTexImage2D(textureType, 0, glFormat, width, height, 0, glFormat, GL_FLOAT, 0);
	}
	if (type == TextureType::TEX_3D)
	{
		//Not implemented
		//glTexImage3D(textureType, 0, glDepth, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}
	if (type == TextureType::TEX_CUBE_MAP)
	{
		for (unsigned int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat,
			width, height, 0, glFormat, GL_FLOAT,
			NULL);
	}
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Set texture to framebuffer based on texture type
	if (type == TextureType::TEX_2D)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTexture, 0);
	if (type == TextureType::TEX_3D)
		glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, colorBufferTexture, 0, 0);
	if (type == TextureType::TEX_CUBE_MAP)
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBufferTexture, 0);

	//Draw to the first color attachment
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// Check that the buffer is OK
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "RenderTexture framebuffer encountered an error!"
			<< std::endl;

	// Unbind
	glBindTexture(textureType, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::BindGL()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glClearDepth(1.0f);
	glClearColor(1, 1, 1, 1);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void RenderTexture::FreeGL()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::DestroyGL()
{
	glDeleteTextures(1, &depthBufferTexture);
	glDeleteTextures(1, &colorBufferTexture);

	glDeleteFramebuffers(1, &frameBuffer);
}

#endif