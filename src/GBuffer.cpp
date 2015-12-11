#include "BrickwareGraphics/GBuffer.hpp"

using namespace Brickware;
using namespace Graphics;

GBuffer::GBuffer(unsigned int width, unsigned int height) 
{
	this->width = width;
	this->height = height;

	Init();
}

void GBuffer::WriteBind()
{
	(this->*WriteBindPtr)();
}

void GBuffer::ReadBind() 
{
	(this->*ReadBindPtr)();
}

void GBuffer::Free() 
{
	(this->*FreePtr)();
}

void GBuffer::Init() 
{
	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Setup function pointers based on rendering API

#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			InitGL();

			WriteBindPtr = &GBuffer::WriteBindGL;
			ReadBindPtr = &GBuffer::ReadBindGL;
			FreePtr = &GBuffer::FreeGL;
			DestroyPtr = &GBuffer::DestroyGL;
		}
		else
		{
			Utility::Logger::Log("Error loading GBuffer: Your card does not support OpenGL 3+");
		}
	}
#endif

#ifdef D3D_SUPPORT
	if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
			InitD3D();

			BindPtr = &GBuffer::BindD3D;
			FreePtr = &GBuffer::FreeD3D;
			DestroyPtr = &GBuffer::DestroyD3D;
		}
		else
		{
			Utility::Logger::Log("Error loading Buffer: GPU must support Direct X 11 or greater");
		}
	}
#endif
}

GBuffer::~GBuffer() 
{
	(this->*DestroyPtr)();
}