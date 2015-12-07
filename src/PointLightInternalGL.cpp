#define BRICKWARE_GRAPHICS_EXPORTS

#define _USE_MATH_DEFINES

#include "BrickwareGraphics/PointLightInternal.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"
#include <cmath>

using namespace Brickware;
using namespace Graphics;
using namespace Math;

CameraDirection PointLightInternal::CameraDirections[] = {
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vector3(1.0f, 0.0f, 0.0f),
	Vector3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vector3(-1.0f, 0.0f, 0.0f),
	Vector3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vector3(0.0f, 1.0f, 0.0f),
	Vector3(0.0f, 0.0f, -1.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vector3(0.0f, -1.0f, 0.0f),
	Vector3(0.0f, 0.0f, 1.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vector3(0.0f, 0.0f, 1.0f),
	Vector3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vector3(0.0f, 0.0f, -1.0f),
	Vector3(0.0f, -1.0f, 0.0f) } };

void PointLightInternal::InitGL()
{

}

void PointLightInternal::RenderShadowMapGL()
{
	Matrix4 depthProjection =
		Matrix4::getPerspectiveProjection((float)M_PI_2, 1.0f, 0.1f, farPlane);
	Matrix4 depthView;

	Matrix4 biasMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// Send the 6 view * proj maps to the geometry shader
	std::vector<Matrix4> values;

	for (unsigned int i = 0; i < 6; i++) {
		// Setup look at for this direction
		CameraDirection cameraDirection = CameraDirections[i];
		depthView = Matrix4::getLookAtView(
			position, position + cameraDirection.Target, cameraDirection.Up);
		depthVP = depthView * depthProjection;

		values.push_back(depthVP);
	}

	shadowMaterial->setVector3("lightPos", position);
	shadowMaterial->setFloat("farPlane", farPlane);
	shadowMaterial->setMultipleMatrix4(
		"shadowMatrices[0]", values);

	RenderPass shadowPass;
	shadowPass.view = depthView; //Irrelevant
	shadowPass.projection = depthProjection; //Irrelevant
	shadowPass.renderTexture = renderTexture;
	shadowPass.material = shadowMaterial;

	//Add pass for rendering
	RenderingManager::AddPreScenePass(shadowPass);
}

void PointLightInternal::BindShadowMapGL(Shader *shader)
{
	// Set far plane for rendering
	std::string farPlaneString =
	    "pointLights[" + std::to_string(lightIndex) + "].farPlane";
	shader->setGlobalFloat(farPlaneString.c_str(), farPlane);
	
	// Bind shadow map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderTexture->depthBufferTexture);
}
