#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Screen.hpp"

using namespace Brickware;
using namespace Graphics;

int Screen::width;
int Screen::height;
float Screen::aspectRatio;
bool Screen::fullscreen;

#ifdef GL_SUPPORT
GLFWwindow* Screen::glWindow;
#endif

void(*Screen::SetResolutionPtr)(int width, int height, bool fullscreen);

bool Screen::Init()
{
	//Default screen settings
	Screen::width = 800;
	Screen::height = 600;
	Screen::aspectRatio = ((float)width / (float)height);
	Screen::fullscreen = false;

	//Setup function pointers based on rendering API
	RenderingAPI renderer = GraphicsSettings::Renderer;
#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		SetResolutionPtr = &SetResolutionGL;
		return InitGL();
	}
#endif

#ifdef D3D_SUPPORT
	if (renderer = RenderingAPI::DirectX)
	{
		SetResolutionPtr = &SetResolutionD3D;
		return InitD3D();
	}
#endif

	return false;
}

int Screen::GetWidth(){ return width; }
int Screen::GetHeight(){ return height; }
float Screen::GetAspectRatio(){ return aspectRatio; }
bool Screen::GetFullscreen(){ return fullscreen; }

void Screen::SetResolution(int width, int height, bool fullscreen)
{
	Screen::width = width;
	Screen::height = height;
	Screen::aspectRatio = ((float)width / (float)height);
	Screen::fullscreen = fullscreen;

	(*SetResolutionPtr)(width, height, fullscreen);
}

bool Screen::InitGL()
{
	//Attempt initialization
	if (!glfwInit())
		return false;

	//Try to find the highest supported core profile version
	bool success = false;
	for (unsigned int major = 4; major > 0; major--)
	{
		for (unsigned int minor = 5; minor > 0; minor--)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
#ifdef BRICKWARE_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			//Create window just to test if context works
			glWindow = glfwCreateWindow(800,600, "Brickware-Test", nullptr, nullptr);

			if (glWindow != nullptr)
			{
				minor = 1;
				major = 1;
				success = true;
				break;
			}
		}
	}

	if (!success)
	{
		glfwTerminate();
		return false;
	}

	//Make Context
	glfwMakeContextCurrent(glWindow);

	return true;
}

void Screen::SetResolutionGL(int width, int height, bool fullscreen)
{
	GLFWwindow* oldWindow = glWindow;

	if (fullscreen)
		glWindow = glfwCreateWindow(width, height, "Brickware-Test", glfwGetPrimaryMonitor(), oldWindow);
	else
		glWindow = glfwCreateWindow(width, height, "Brickware-Test", nullptr, oldWindow);

	glfwMakeContextCurrent(glWindow);

	glfwDestroyWindow(oldWindow);

	//Set Vsync
	if (Graphics::GraphicsSettings::VSync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//OpenGL initialization
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

#ifdef BRICKWARE_DEBUG
	// Enable the debugging layer of OpenGL
	// GL_DEBUG_OUTPUT - Faster version but not useful for breakpoints
	// GL_DEBUG_OUTPUT_SYNCHRONUS - Callback is in sync with errors, so a breakpoint
	// can be placed on the callback in order to get a stacktrace for the GL error.
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLPrintErrorCallback, NULL);
#endif
}