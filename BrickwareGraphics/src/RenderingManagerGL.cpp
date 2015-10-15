#ifdef GL_SUPPORT
#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;

void RenderingManager::RenderGL()
{
	PreRenderGL();

	ShadowPassGL();

	//Other user defined render passes here?

	ScenePassGL();

	//Cleanup
	lights.clear();
	renderables.clear();
}

void RenderingManager::PreRenderGL()
{
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderingManager::ShadowPassGL()
{
	//Send light data to the shader
	for (unsigned int j = 0; j < lights.size(); j++)
		lights[j]->RenderShadowMap();
}

void RenderingManager::ScenePassGL()
{
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Render every renderable object
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
		Renderable renderable = renderables[i];
		Shader* shader = renderable.material->shader;

		if (shader != Shader::ActiveShader)
		{
			Shader::ActiveShader = shader;
			shader->bindShader();

			//Send light data to the shader
			for (unsigned int j = 0; j < lights.size(); j++)
			{
				lights[j]->RenderLight(shader);
				lights[j]->BindShadowMap(shader);
			}
		}

		//Render object
		RenderObjectGL(renderable.mesh, renderable.material);
	}

	Shader::ActiveShader->freeShader();
}

void RenderingManager::RenderObjectGL(Mesh* mesh, Material* material)
{
	material->sendDataToGPU();

	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	glBindVertexArray(mesh->getVAO());

	//Draw Shape
	glDrawArrays(GL_TRIANGLES, 0, mesh->getNumberOfVerts());
}

void RenderingManager::RenderSceneShadowsGL(Shader* shadowShader)
{
	//Render every renderable object for the light
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
		Renderable renderable = renderables[i];
		renderable.shadowMaterial->setShader(shadowShader);

		//Render object
		RenderObjectGL(renderable.mesh, renderable.shadowMaterial);
	}
}

#endif
