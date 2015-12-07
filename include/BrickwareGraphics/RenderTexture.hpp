/**
* \class RenderTexture
* \ingroup BrickwareGraphics
*
* \brief A texture that can be rendered to
*
* A general purpose implementation of a RenderTarget or FrameBufferObject. 
* This is less of an asset right now and more of a non-graphics specific 
* render target.
*/

#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

#ifdef D3D_SUPPORT
#include <windows.h>
#include <d3d11.h>
#endif

//Other Project Headers
#include "BrickwareUtils/StringUtils.hpp"

//System Level Headers

//Project Headers
#include "BrickwareGraphics/Texture.hpp"
#include "BrickwareGraphics/GraphicsSettings.hpp"

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API RenderTexture
		{
			friend class InternalLight;
			friend class DirectionalLightInternal;
			friend class PointLightInternal;
			friend class RenderingManager;

		public:
			/** Creates a Render Tetxure of a given width and height
			 * Assumes a depth of 16 bits
			 * Assumes a texture format of RGBA
			 * \param type the TextureType of the texture
			 * \param width the width of the texture
			 * \param height the height of the texture
			 */
			RenderTexture(TextureType type, unsigned int width, unsigned int height);

			/** Creates a Render Tetxure of a given width, height and depth
			 * Assumes a texture format of RGBA
			 * \param type The TextureType of the texture
			 * \param width The width of the texture
			 * \param height The height of the texture
			 * \param depth The bits in the depth buffer (0, 16 or 24)
			 */
			RenderTexture(TextureType type, unsigned int width, unsigned int height, unsigned int depth);

			/** Creates a Render Tetxure of a given width, height, depth and texture format
			 * \param type The TextureType of the texture
			 * \param width The width of the texture
			 * \param height The height of the texture
			 * \param depth The bits in the depth buffer (0, 16 or 24)
		 	 * \param format The TextureFormat for the color buffer
			 */
			RenderTexture(TextureType type, unsigned int width, unsigned int height, unsigned int depth, TextureFormat format);

			///Binds the render texture to be drawn to
			void Bind();

			///Unbinds the render texture
			void Free();

			virtual ~RenderTexture();
		private:
			TextureType type;
			unsigned int width;
			unsigned int height;
			unsigned int depth;
			TextureFormat format;

			//Inits the render texture based on rendering API
			void Init();

			void(RenderTexture::*BindPtr)();
			void(RenderTexture::*FreePtr)();
			void(RenderTexture::*DestroyPtr)();

			//GL specific methods
#ifdef GL_SUPPORT
			GLuint frameBuffer;
			GLuint depthBufferTexture;
			GLuint colorBufferTexture;

			void InitGL();
			void BindGL();
			void FreeGL();

			void DestroyGL();
#endif

#ifdef D3D_SUPPORT
			void InitD3D();
			void BindD3D();
			void FreeD3D();

			void DestroyD3D();
#endif
		
			
		};
	}
}

#endif