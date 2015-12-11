#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RenderTexture.hpp"
#include "BrickwareUtils/Logger.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

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
			Utility::Logger::Log("RenderTexture depth buffer encountered an error!");
	}


	glGenTextures(1, &colorBufferTexture);
	glBindTexture(textureType, colorBufferTexture);

	GLint glInternalFormat = GL_RGB16F;
	GLint glInputFormat = GL_RGBA;

	switch (format)
	{
	case TextureFormat::RGBA:
		glInputFormat = GL_RGBA;
		glInternalFormat = GL_RGBA;
		break;
	case TextureFormat::BGRA:
		glInputFormat = GL_BGRA;
		glInternalFormat = GL_BGRA;
		break;
	case TextureFormat::RGB:
		glInternalFormat = GL_RGB16F;
		glInputFormat = GL_RGB;
		break;
	case TextureFormat::BGR:
		glInternalFormat = GL_RGB16F;
		glInputFormat = GL_RGB;
		break;
	}

	//Create internal texture based on settings
	if (type == TextureType::TEX_2D)
	{
		glTexImage2D(textureType, 0, glInternalFormat, width, height, 0, glInputFormat, GL_FLOAT, 0);
	}
	if (type == TextureType::TEX_3D)
	{
		//Not implemented
		//glTexImage3D(textureType, 0, glDepth, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}
	if (type == TextureType::TEX_CUBE_MAP)
	{
		for (unsigned int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat,
				width, height, 0, glInputFormat, GL_FLOAT,
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

	//Tell OpenGL which buffers to draw to
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// Check that the buffer is OK
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Utility::Logger::Log("RenderTexture color buffer encountered an error!");

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