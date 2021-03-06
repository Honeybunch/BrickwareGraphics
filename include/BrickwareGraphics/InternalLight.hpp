#ifndef INTERNAL_LIGHT_H
#define INTERNAL_LIGHT_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "BrickwareMath/Vector3.hpp"

//System Level Headers
#include <string>

//Project Headers
#include "BrickwareGraphics/Material.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"
#include "BrickwareGraphics/RenderTexture.hpp"

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API InternalLight
		{
		public:
			InternalLight();

			void setAmbientColor(Math::Vector3 ambientColor);
			void setDiffuseColor(Math::Vector3 diffuseColor);
			void setSpecularColor(Math::Vector3 specularColor);

			void setShadowStrength(float shadowStrength);
			void setShadowBias(float shadowBias);

			void setShadowMapQuality(ShadowQuality shadowQuality);

			virtual void RenderLight(Shader* shader) = 0;
			virtual void RenderShadowMap() = 0;
			virtual void BindShadowMap(Shader* shader) = 0;

			virtual ~InternalLight(void);

		protected:
			virtual void Init() = 0;

			Math::Matrix4 depthVP;
			Math::Matrix4 depthBiasVP;

			Math::Vector3 ambientColor;
			Math::Vector3 diffuseColor;
			Math::Vector3 specularColor;

			RenderTexture* renderTexture;
			Material* shadowMaterial;

			float shadowStrength;
			float shadowBias;
			float shadowNormalBias; //TODO: Implement

			unsigned int shadowMapRes;
		};
	}
}
#endif

