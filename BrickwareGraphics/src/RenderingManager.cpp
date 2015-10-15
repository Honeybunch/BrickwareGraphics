#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RenderingManager.hpp"
#include "BrickwareGraphics/GraphicsSettings.hpp"

using namespace Brickware;
using namespace Graphics;

//Statics
Material* RenderingManager::currentMaterial;
Material* RenderingManager::currentShadowMaterial;

std::vector<Renderable> RenderingManager::renderables;
std::vector<InternalLight*> RenderingManager::lights;

#ifdef D3D_SUPPORT
ID3D11Device* RenderingManager::device;
ID3D11DeviceContext* RenderingManager::deviceContext;
IDXGIDevice* RenderingManager::dxgiDevice;
IDXGIAdapter* RenderingManager::dxgiAdapter;
#endif

void(*RenderingManager::Render)();

Shader* RenderingManager::DirectionalShadowShader = nullptr;
Shader* RenderingManager::PointShadowShader = nullptr;

void RenderingManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	RenderingAPI renderer = GraphicsSettings::Renderer;

	if (GraphicsSettings::Shadows)
	{
		if (DirectionalShadowShader == nullptr)
			DirectionalShadowShader = new Shader("Shaders/DirectionalShadowVertex");
		if (PointShadowShader == nullptr)
			PointShadowShader = new Shader("Shaders/PointShadowGeometry", "Shaders/PointShadowVertex", "Shaders/PointShadowPixel");
	}

	//Setup function pointers based on rendering API
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			RenderingManager::Render = &RenderingManager::RenderGL;
		}
	}
	else if (renderer = RenderingAPI::DirectX)
	{
#ifdef D3D_SUPPORT
		RenderingManager::device = device;
		RenderingManager::deviceContext = deviceContext;

		RenderingManager::device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		RenderingManager::dxgiDevice->GetAdapter(&dxgiAdapter);
#endif

		if(RendererInfo::GetAPIMajorVersion() == 11)
		{
#ifdef D3D_SUPPORT
			RenderingManager::Render = &RenderingManager::RenderD3D;
			RenderingManager::RenderPass = &RenderingManager::RenderPassD3D;
#endif
		}
	}
}

void RenderingManager::AddLight(InternalLight* light)
{
	lights.push_back(light);
}
void RenderingManager::UseMaterial(Material* material)
{
	currentMaterial = material;
}
void RenderingManager::UseShadowMaterial(Material* shadowMaterial)
{
	currentShadowMaterial = shadowMaterial;
}
void RenderingManager::DrawMesh(Mesh* mesh)
{
	Renderable renderable;
	renderable.mesh = mesh;
	renderable.material = currentMaterial;
	renderable.shadowMaterial = currentShadowMaterial;

	renderables.push_back(renderable);
}

void RenderingManager::Destroy()
{

}
