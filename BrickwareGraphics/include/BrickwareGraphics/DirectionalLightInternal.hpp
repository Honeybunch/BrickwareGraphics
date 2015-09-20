#ifndef DIRECTIONAL_LIGHT_INTERNAL_H
#define DIRECTIONAL_LIGHT_INTERNAL_H

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
#include "BrickwareGraphics/Light.hpp"

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API DirectionalLightInternal : public Light
		{
		public:
			DirectionalLightInternal();

			void setDirection(Math::Vector3 direction);
			virtual void RenderLight(Shader* shader);
			virtual void RenderShadowMap(Shader* shadowShader);

			virtual ~DirectionalLightInternal(void);

		private:
			Math::Vector3 direction;
			void Init();

			void (DirectionalLightInternal::*RenderShadowMapPtr)(Shader* shadowShader);

#ifdef GL_SUPPORT
			void InitGL();
			void RenderShadowMapGL(Shader* shadowShader);

			GLuint shadowBuffer;
			GLuint depthTexture;
#endif
#ifdef D3D_SUPPORT
			void InitD3D();
			void RenderShadowMapD3D(Shader* shadowShader);
#endif
		};
	}
}
#endif
