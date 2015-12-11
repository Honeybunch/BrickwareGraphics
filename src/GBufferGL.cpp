#include "BrickwareGraphics/GBuffer.hpp"

using namespace Brickware;
using namespace Graphics;

void GBuffer::InitGL()
{
	//Create Framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Create textures
	colorBufferTextures = new GLuint[colorBufferCount];
	glGenTextures(colorBufferCount, colorBufferTextures);

	//Position texture
	glBindTexture(GL_TEXTURE_2D, colorBufferTextures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTextures[0], 0);

	//Normal texture
	glBindTexture(GL_TEXTURE_2D, colorBufferTextures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorBufferTextures[1], 0);

	//Color texture
	glBindTexture(GL_TEXTURE_2D, colorBufferTextures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorBufferTextures[2], 0);

	//Make sure to draw all color attachments
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Check that the buffer is OK
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Utility::Logger::Log("GBuffer encountered an error!");
}

void GBuffer::WriteBindGL()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glClearDepth(1.0f);
	glClearColor(1, 1, 1, 1);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void GBuffer::ReadBindGL() 
{
	for (GLuint i = 0; i < colorBufferCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, colorBufferTextures[i]);
	}
}

void GBuffer::FreeGL()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::DestroyGL() 
{
	glDeleteTextures(colorBufferCount, colorBufferTextures);

	glDeleteFramebuffers(1, &frameBuffer);
}