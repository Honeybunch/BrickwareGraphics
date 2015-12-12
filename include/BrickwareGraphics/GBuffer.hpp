/**
* \class GBuffer
* \ingroup BrickwareGraphics
*
* \brief A graphics buffer that's rendered to as part of a deferred render step
*
* This should not be exposed to the end user. The RenderingManager should use this
* as a target for the position, texture coordinate and color data that is going to 
* be combined for the final frame.
* 
* The GBuffer is essentially a collection of textures in a frame buffer object that
* represent a bunch of data used for rendering the final frame such as normals,
* world position and color.
*/

#ifndef GBUFFER_H
#define GBUFFER_H

#include "BrickwareGraphics/RendererInfo.hpp"
#include "BrickwareGraphics/GraphicsSettings.hpp"

#include "BrickwareUtils/Logger.hpp"

namespace Brickware {
	namespace Graphics {
		class GBuffer
		{
		public:
			/** Creates a graphics buffer of a given width and height
			 * \param width The width of the textures used in the buffer
			 * \param height The height of the textures used in the buffer
			 */
			GBuffer(unsigned int width, unsigned int height);

			///Binds the GBuffer to be written to
			void WriteBind();

			///Binds the GBuffer to be read from
			void ReadBind();

			///Frees the GBuffer from being written to
			void Free();

			///Destroy the GBuffer on the CPU and GPU side
			virtual ~GBuffer();

		private:
			unsigned int width;
			unsigned int height;

			void(GBuffer::*WriteBindPtr)();
			void(GBuffer::*ReadBindPtr)();
			void(GBuffer::*FreePtr)();
			void(GBuffer::*DestroyPtr)();

			//Inits the GBuffer based on rendering API
			void Init();

			const unsigned int colorBufferCount = 3;

			//GL specific methods
#ifdef GL_SUPPORT
			GLuint frameBuffer;
			GLuint depthBuffer;
			GLuint* colorBufferTextures;

			///Init the GBuffer with OpenGL
			void InitGL();

			///Bind the GBuffer to be drawn to with OpenGL
			void WriteBindGL();

			///Bind the GBuffer to be read from with OpenGL
			void ReadBindGL();

			///Free the GBuffer from being drawn to with OpenGL
			void FreeGL();

			///Free the GBuffer from OpenGL's GPU memory
			void DestroyGL();
#endif

#ifdef D3D_SUPPORT
			///Init the GBuffer with DirectX 11
			void InitD3D();

			///Bind the GBuffer for drawing with DirectX 11
			void BindD3D();

			///Free the GBuffer from being drawn to with DirectX 11
			void FreeD3D();

			///Free the GBuffer from DirectX 11's GPU memory
			void DestroyD3D();
#endif
		};
	}
}
#endif