#define BRICKWARE_GRAPHICS_EXPORTS

#define _CRT_SECURE_NO_WARNINGS

#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"

#include "BrickwareUtils/StringUtils.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

Shader* Shader::ActiveShader = nullptr;

Shader::Shader(std::string vertexShaderFileName)
{
	//TODO: Strip existing file extension if any
	std::string trimmedVertexFileName = StringUtils::trimToLastChar(vertexShaderFileName, '/');

	//Setup function pointers based on rendering API
	RenderingAPI renderer = GraphicsSettings::Renderer;
#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			loadGLSL(vertexShaderFileName);

			setGLFunctionPointers();
		}
		else
		{
			std::cout << "Error loading Shader: Your card does not support OpenGL 3+" << std::endl;
		}
	}
#endif

#ifdef D3D_SUPPORT
	if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
			loadHLSL(trimmedVertexFileName);

			setD3DFunctionPointers();
		}
	}
#endif
}

Shader::Shader(std::string vertexShaderFileName, std::string pixelShaderFileName)
{
	//TODO: Strip existing file extension if any
	std::string trimmedVertexFileName = StringUtils::trimToLastChar(vertexShaderFileName, '/');
	std::string trimmedPixelFileName = StringUtils::trimToLastChar(pixelShaderFileName, '/');

	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Setup function pointers based on rendering API

#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			loadGLSL(vertexShaderFileName, pixelShaderFileName);

			setGLFunctionPointers();
		}
		else
		{
			std::cout << "Error loading Shader: Your card does not support OpenGL 3+" << std::endl;
		}
	}
#endif

#ifdef D3D_SUPPORT
	if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
			loadHLSL(trimmedVertexFileName, trimmedPixelFileName);

			setD3DFunctionPointers();
		}
	}
#endif
}

Shader::Shader(std::string geometryShaderFileName, std::string vertexShaderFileName, std::string pixelShaderFileName)
{
	//TODO: Strip existing file extension if any
	std::string trimmedGeometryFileName = StringUtils::trimToLastChar(geometryShaderFileName, '/');
	std::string trimmedVertexFileName = StringUtils::trimToLastChar(vertexShaderFileName, '/');
	std::string trimmedPixelFileName = StringUtils::trimToLastChar(pixelShaderFileName, '/');

	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Setup function pointers based on rendering API

#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			loadGLSL(geometryShaderFileName, vertexShaderFileName, pixelShaderFileName);

			setGLFunctionPointers();
		}
		else
		{
			std::cout << "Error loading Shader: Your card does not support OpenGL 3+" << std::endl;
		}
	}
#endif

#ifdef D3D_SUPPORT
	if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
			loadHLSL(geometryShaderFileName, trimmedVertexFileName, trimmedPixelFileName);

			setD3DFunctionPointers();
		}
	}
#endif
}

void Shader::bindShader()
{
	ActiveShader = this;
	(this->*bindShaderPtr)();
}

void Shader::freeShader()
{
	ActiveShader = nullptr;
	(this->*freeShaderPtr)();
}

void Shader::setGlobalVector4(std::string valueName, Vector4 value)
{
	(this->*setGlobalVector4Ptr)(valueName, value);
}
void Shader::setGlobalVector3(std::string valueName, Vector3 value)
{
	(this->*setGlobalVector3Ptr)(valueName, value);
}
void Shader::setGlobalVector2(std::string valueName, Vector2 value)
{
	(this->*setGlobalVector2Ptr)(valueName, value);
}

void Shader::setGlobalInt(std::string valueName, int value)
{
	(this->*setGlobalIntPtr)(valueName, value);
}
void Shader::setGlobalFloat(std::string valueName, float value)
{
	(this->*setGlobalFloatPtr)(valueName, value);
}
void Shader::setGlobalDouble(std::string valueName, double value)
{
	(this->*setGlobalDoublePtr)(valueName, value);
}

void Shader::setGlobalMatrix4(std::string valueName, Matrix4 value)
{
	(this->*setGlobalMatrix4Ptr)(valueName, value);
}
void Shader::setGlobalMatrix3(std::string valueName, Matrix3 value)
{
	(this->*setGlobalMatrix3Ptr)(valueName, value);
}

void Shader::setMultipleGlobalMatrix4(std::string valueName, std::vector<Matrix4> values)
{
	(this->*setMultipleGlobalMatrix4Ptr)(valueName, values);
}

void Shader::setGlobalTexture(std::string valueName, Texture* value)
{
	if (textureMap.find(valueName) != textureMap.end())
	{
		value->bindTexture(textureMap[valueName]);
	}
}

Shader::~Shader()
{
#ifdef D3D_SUPPORT
	ReleaseMacro(vsBlob);
	ReleaseMacro(psBlob);

	ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(inputLayout);
	ReleaseMacro(vsConstantBuffer);
#endif
}
