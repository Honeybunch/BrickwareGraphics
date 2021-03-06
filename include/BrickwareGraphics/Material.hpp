#ifndef MATERIAL_H
#define MATERIAL_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "BrickwareUtils/StringUtils.hpp"

#include "BrickwareMath/Matrix3.hpp"
#include "BrickwareMath/Matrix4.hpp"
#include "BrickwareMath/Vector2.hpp"
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"

//System Level Headers
#include <map>

//Project Headers
#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareGraphics/Texture.hpp"

namespace Brickware
{
	namespace Graphics
	{
#ifdef _WIN32
#ifdef D3D_SUPPORT
		template class BRICKWARE_GRAPHICS_API std::vector < ID3D11Buffer* >;
		template class BRICKWARE_GRAPHICS_API std::vector < char* >;
#endif
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Texture* >;
		template class BRICKWARE_GRAPHICS_API std::vector < Material* > ;
		template class BRICKWARE_GRAPHICS_API std::vector < Math::Matrix4 >;

		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Vector2 > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Vector3 > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Vector4 > ;

		template class BRICKWARE_GRAPHICS_API std::map < std::string, int>;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, float >;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, double >;
								
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Matrix3 > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, Math::Matrix4 > ;
		template class BRICKWARE_GRAPHICS_API std::map < std::string, std::vector<Math::Matrix4> >;
#endif
		class BRICKWARE_GRAPHICS_API Material
		{
			friend class RenderingManager;

		public:
			Material(Shader* shader);
			Material(const Material& material);

			static std::vector<Material*> Materials;

			void setShader(Shader* shader);
			Shader* getShader();

#ifdef D3D_SUPPORT
			std::vector<ID3D11Buffer*> getConstantBuffers();
			std::vector<char*> getConstantBufferData();
#endif
			
			void setVector4(std::string valueName, Math::Vector4 value);
			void setVector3(std::string valueName, Math::Vector3 value);
			void setVector2(std::string valueName, Math::Vector2 value);

			void setInt(std::string valueName, int value);
			void setFloat(std::string valueName, float value);
			void setDouble(std::string valueName, double value);

			void setMatrix4(std::string valueName, Math::Matrix4 value);
			void setMatrix3(std::string valueName, Math::Matrix3 value);

			void setMultipleMatrix4(std::string valueName, std::vector<Math::Matrix4> values);

			void setTexture(std::string textureName, Texture* texture);

			~Material();

		private:
			Shader* shader;

			std::map<std::string, Math::Vector2> vector2Map;
			std::map<std::string, Math::Vector3> vector3Map;
			std::map<std::string, Math::Vector4> vector4Map;

			std::map<std::string, int> intMap;
			std::map<std::string, float> floatMap;
			std::map<std::string, double> doubleMap;

			std::map<std::string, Math::Matrix3> matrix3Map;
			std::map<std::string, Math::Matrix4> matrix4Map;
			std::map <std::string, std::vector<Math::Matrix4>> matrix4BatchMap;

			std::map<std::string, Texture*> textureMap;

			void sendDataToGPU();

			//Default texture to be loaded once and passed by default to every material
			static Texture* defaultTexture;
		};
	}
}
#endif
