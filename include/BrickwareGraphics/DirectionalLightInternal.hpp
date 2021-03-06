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
#include "BrickwareGraphics/InternalLight.hpp"

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API DirectionalLightInternal : public InternalLight
		{
		public:
			DirectionalLightInternal();

			void setDirection(Math::Vector3 direction);

			virtual void RenderLight(Shader* shader);
			virtual void RenderShadowMap();
			virtual void BindShadowMap(Shader* shader);

			virtual ~DirectionalLightInternal(void);

		protected:
			Math::Vector3 direction;

			virtual void Init();

			void (DirectionalLightInternal::*InitPtr)();
			void (DirectionalLightInternal::*RenderShadowMapPtr)();
			void (DirectionalLightInternal::*BindShadowMapPtr)(Shader* shader);

#ifdef GL_SUPPORT
			void InitGL();
			void RenderShadowMapGL();
			void BindShadowMapGL(Shader* shader);
#endif
#ifdef D3D_SUPPORT
			void InitD3D();
			void RenderShadowMapD3D();
			void BindShadowMapD3D(Shader* shader);
#endif
		};
	}
}
#endif

