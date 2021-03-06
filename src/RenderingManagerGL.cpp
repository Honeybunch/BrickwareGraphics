#ifdef GL_SUPPORT
#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"
#include "BrickwareGraphics/RenderTexture.hpp"
#include "BrickwareGraphics/Screen.hpp"
#include "BrickwareGraphics/PrimitiveManager.hpp"

using namespace Brickware;
using namespace Graphics;

void RenderingManager::RenderGL()
{
	PreRenderGL();
	
	PreScenePassGL();
	
	ScenePassGL();
	
	// Cleanup
	lights.clear();
	renderables.clear();
	preScenePasses.clear();
	scenePasses.clear();
}

void RenderingManager::PreRenderGL()
{
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderingManager::PreScenePassGL()
{
	for (size_t i = 0; i < preScenePasses.size(); i++)
	{
		RenderPass pass = preScenePasses[i];
		RenderTexture* renderTexture = pass.renderTexture;
		Material* material = pass.material;
		Shader* shader = material->shader;

		renderTexture->Bind();
		shader->bindGLSL();

		material->sendDataToGPU();

		shader->setMatrix4GL("viewMatrix", pass.view);
		shader->setMatrix4GL("projectionMatrix", pass.projection);

		// Render every renderable object
		for (unsigned int i = 0; i < renderables.size(); i++) {
			Renderable renderable = renderables[i];
			renderable.shadowMaterial->setShader(shader);
			
			// Render object
			RenderObjectGL(renderable.mesh, renderable.shadowMaterial);
		}

		shader->freeGLSL();
		renderTexture->Free();
	}
}

void RenderingManager::ScenePassGL()
{
	for (unsigned int i = 0; i < scenePasses.size(); i++)
	{
		RenderPass pass = scenePasses[i];
		RenderTexture* renderTexture = pass.renderTexture;

		GraphicsBuffer->WriteBind();

		// Render every renderable object
		for (unsigned int j = 0; j < renderables.size(); j++) {
			Renderable renderable = renderables[j];
			Shader *shader = renderable.material->shader;

			if (shader != Shader::ActiveShader) {
				Shader::ActiveShader = shader;
				shader->bindShader();

				shader->setMatrix4GL("viewMatrix", pass.view);
				shader->setMatrix4GL("projectionMatrix", pass.projection);
			}

			// Render object
			RenderObjectGL(renderable.mesh, renderable.material);
		}

		renderTexture->Bind();

		GraphicsBuffer->ReadBind();

		//Render primitives
		PrimitiveManager::DrawPrimitives(pass.view, pass.projection);

		//Render to texture
		deferredShader->bindGLSL();

		// Send light data to the shader
		for (unsigned int j = 0; j < lights.size(); j++) {
			lights[j]->RenderLight(deferredShader);
			lights[j]->BindShadowMap(deferredShader);
		}

		deferredShader->setVector3GL("eyePosition", pass.eyePoint);

		// Draw nothing but 3 "verticies"
		glBindVertexArray(internalScreen->getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 3);

		deferredShader->freeGLSL();

		renderTexture->Free();
	}

	PrimitiveManager::ClearPrimitives();

	Shader::ActiveShader->freeShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//Render screen
	screenShader->bindGLSL();

	//Bind frame
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalRenderTexture->colorBufferTexture);

	// Draw nothing but 3 "verticies"
	glBindVertexArray(internalScreen->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 3);

	screenShader->freeGLSL();

	//Unbind frame
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Unbind VAO
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingManager::RenderObjectGL(Mesh *mesh, Material *material)
{
	material->sendDataToGPU();

	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	glBindVertexArray(mesh->getVAO());

	// Draw Shape
	unsigned int numOfVerts = (unsigned int)mesh->getNumberOfVerts();
	glDrawArrays(GL_TRIANGLES, 0, numOfVerts);
}

void RenderingManager::RenderSceneShadowsGL(Shader *shadowShader)
{
	// Render every renderable object for the light
	for (unsigned int i = 0; i < renderables.size(); i++) {
	  Renderable renderable = renderables[i];
	  renderable.shadowMaterial->setShader(shadowShader);
	
	  // Render object
	  RenderObjectGL(renderable.mesh, renderable.shadowMaterial);
	}
	
	glBindVertexArray(0);
}

#endif
