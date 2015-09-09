#ifdef GL_SUPPORT
#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;

void RenderingManager::RenderGL()
{
	Shader* activeShader = nullptr;

	//Render every renderable object
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
		Renderable renderable = renderables[i];

		if (renderable.material->shader != activeShader)
		{
			if (activeShader != nullptr)
				activeShader->freeShader();
			activeShader = renderable.material->shader;

			activeShader->bindShader();

			//Send light data to the shader
			for (unsigned int j = 0; j < lights.size(); j++)
			{
				lights[j]->Render(activeShader);
			}
		}

		Mesh* mesh = renderable.mesh;
		Material* material = renderable.material;

		material->sendDataToGPU();

		GLint shaderProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

		GLuint vPosition = glGetAttribLocation(shaderProgram, "vPosition");
		GLuint vNormal = glGetAttribLocation(shaderProgram, "vNormal");
		GLuint vTexCoord = glGetAttribLocation(shaderProgram, "vTexCoord");

		glEnableVertexAttribArray(vPosition);
		glEnableVertexAttribArray(vNormal);
		glEnableVertexAttribArray(vTexCoord);

		int normalOffset = mesh->getPointSize();
		int texCoordOffset = normalOffset + mesh->getNormalSize();

		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBO());
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(texCoordOffset));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIBO());

		//Draw Shape
		glDrawElements(GL_TRIANGLES, mesh->getNumberOfVerts(), GL_UNSIGNED_SHORT, (void *)0);


	}

	lights.clear();
	renderables.clear();
}
#endif