#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

#define PI 3.14159265359f

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//start animation loop until escape key is pressed
	do{

    updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  return true;
}

bool initializeVertexbuffer()
{
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  vertexbuffer_size = 3;
  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
  };

  glm::vec2 triangleVertice1 = glm::vec2(-0.5f, -0.5f);
  glm::vec2 triangleVertice2 = glm::vec2(0.5f, -0.5f);
  glm::vec2 triangleVertice3 = glm::vec2(0.0f, 0.5f);
  glm::mat2 myR = glm::mat2(1.0f, 0.0f, 
                            0.0f, 1.0f);
  glm::vec2 myT = glm::vec2(1.0f, 1.0f);

  //triangleVertice1 = myR * triangleVertice1 + myT;
  //triangleVertice2 = myR * triangleVertice2 + myT;
  //triangleVertice3 = myR * triangleVertice3 + myT;

  triangleVertice1 = 0.5f * triangleVertice1;
  triangleVertice2 = 0.5f * triangleVertice2;
  triangleVertice3 = 0.5f * triangleVertice3;

  static const GLfloat g_vertex_buffer_data1[] = {
    triangleVertice1[0], triangleVertice1[1], 0.0f,
    triangleVertice2[0], triangleVertice2[1], 0.0f,
    triangleVertice3[0], triangleVertice3[1], 0.0f,
  };

  float theta = 10;
  float theta_radian = theta * 2 * PI / 180;
  glm::mat2 myR2 = glm::mat2(cos(theta_radian), -sin(theta_radian),
                             sin(theta_radian), cos(theta_radian));

  triangleVertice1 = myR2 * triangleVertice1;
  triangleVertice2 = myR2 * triangleVertice2;
  triangleVertice3 = myR2 * triangleVertice3;
  
  static const GLfloat g_vertex_buffer_data2[] = {
    triangleVertice1[0], triangleVertice1[1], 0.0f,
    triangleVertice2[0], triangleVertice2[1], 0.0f,
    triangleVertice3[0], triangleVertice3[1], 0.0f,
  };

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);

  return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

