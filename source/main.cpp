#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <box.h>
#include <camera.h>
#include <light_shape.h>
#include <key_listener.h>

const GLuint SCR_WIDTH = 1200;
const GLuint SCR_HEIGHT = 1200;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLAPIENTRY
messageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s source = 0x%x, type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		source, type, severity, message);
}

GLFWwindow* initWindow()
{
	GLFWwindow* window;
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 3 - Textures, Shading, Lighting", NULL, NULL);
	if (NULL == window)
	{
		fprintf(stderr, "Failed to create GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	GLenum glerr = glewInit();
	if (GLEW_OK != glerr)
	{
		fprintf(stderr, "glewInit Error: %s\n", glewGetErrorString(glerr));
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);

	glDebugMessageCallback(messageCallback, 0);

	return window;
}

int main(int argc, char **argv)
{
	GLFWwindow* window = initWindow();

	std::string textureNames[NUM_BOX_SIDES] = {
		"texture\\cat.jpg",
		"texture\\nyan.jpg",
		"texture\\woman.jpg",
		"texture\\sax.jpg",
		"texture\\gandalf.jpg",
		"texture\\table.jpg",
	};

	ColorBox box1(glm::vec3(1.0f, 0, 0), glm::vec3(1.0f));
	box1.init();
	box1.translate(glm::vec3(2.0f, 0, 0));
	ShapeKeyListener<ColorBox>(window, box1);

	ColorBox box2(glm::vec3(0, 1.0f, 0), glm::vec3(1.0f));
	box2.init();
	box2.translate(glm::vec3(0, 2.0f, 0));
	ShapeKeyListener<ColorBox>(window, box2);

	ColorBox box3(glm::vec3(0, 0, 1.0f), glm::vec3(1.0f));
	box3.init();
	box3.translate(glm::vec3(-2.0f, 0, 0));
	ShapeKeyListener<ColorBox>(window, box3);

	ColorBox box4(glm::vec3(0, 1.0f, 1.0f), glm::vec3(1.0f));
	box4.init();
	box4.translate(glm::vec3(0, -2.0f, 0));
	ShapeKeyListener<ColorBox>(window, box4);

	MultiTextureBox box5(textureNames, glm::vec3(1.0f));
	box5.init();
	ShapeKeyListener<MultiTextureBox>(window, box5);

	LightBox light;
	light.init();
	light.orbit(glm::vec3(0, 0, 1.0f), glm::vec3(0, 0, 0.0f), 3.0f, 3.0f);

	PerspectiveCamera camera(SCR_WIDTH, SCR_HEIGHT, 45.0f, 0.1f, 50.0f);
	camera.init();
	camera.translate(glm::vec3(0, 0, 10.0f));
	camera.lookAt(glm::vec3(0, 0, 0));
	CameraKeyListener(window, camera);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.update();

		box1.update(camera, light);
		box2.update(camera, light);
		box3.update(camera, light);
		box4.update(camera, light);
		box5.update(camera, light);

		light.update(camera);

		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
