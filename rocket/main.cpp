// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
using namespace glm;

#include "controls.hpp"
#include "model.h"
#include "shader.hpp"


#include <vector>

GLfloat power = 0.0;
bool isLaunched = false;
bool isUnfoldedParachute = false;
bool isSafeToUnfold = false;

int main(void) {
  // Initialise GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Rocket Launch", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are "
            "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return -1;
  }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwPollEvents();
  glfwSetCursorPos(window, 1024 / 2, 768 / 2);
  glClearColor(0.7, 0.9, 1.0, 0.0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  //GLuint programID = LoadShaders("TransformVertexShader.vertexshader",
  //                               "ColorFragmentShader.fragmentshader");
  GLuint programID = LoadShaders("StandardShading.vertexshader",
                                 "StandardShading.fragmentshader");
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
  GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

  glUseProgram(programID);
  GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

  graphic_object ground_object(ground, 18 * sizeof(GLfloat), ground_color,
                               18 * sizeof(GLfloat), 6);
  graphic_object cloud_object(cloud, 18 * sizeof(GLfloat), cloud_color,
                              18 * sizeof(GLfloat), 6);
  graphic_object tree_trunk_object(tree_trunk, 36 * 18 * sizeof(GLfloat),
                                   tree_trunk_color,
                                   36 * 6 * 3 * sizeof(GLfloat), 36 * 18);

  graphic_object rocket_body_object(rocket_body, 36 * 18 * sizeof(GLfloat),
                                    rocket_body_color,
                                    36 * 6 * 3 * sizeof(GLfloat), 36 * 18);
  graphic_object rocket_side_object(rocket_side, 18 * sizeof(GLfloat),
                                    rocket_side_color, 18 * sizeof(GLfloat), 6);
  graphic_object rocket_head_object(rocket_head, 36 * 9 * sizeof(GLfloat),
                                    rocket_head_color, 36 * 9 * sizeof(GLfloat),
                                    36 * 9);
  graphic_object rocket_bottom_object(rocket_bottom, 36 * 9 * sizeof(GLfloat),
                                      rocket_bottom_color,
                                      36 * 9 * sizeof(GLfloat), 36 * 9);

  graphic_object parachute_object(parachute, 9 * 8 * sizeof(GLfloat),
                                  parachute_color, 9 * 8 * sizeof(GLfloat),
                                  3 * 8);
  graphic_object parachute_line_object(
      (GLfloat *)parachute_line, sizeof(parachute_line),
      (GLfloat *)parachute_line_color, 16 * 3 * sizeof(GLfloat), 2 * 8,
      GL_LINES);

  double last_time = 0.0;
  double cur_time = 0.0;

  double rotate = 0.0;
  double move_x = 0.0;
  double move_y = 0.0;
  double move_z = 0.0;
  
  do {
    cur_time = glfwGetTime();
    double delta = cur_time - last_time;
    last_time = cur_time;

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // Compute the MVP matrix from keyboard and mouse input
    computeMatricesFromInputs();
    glm::mat4 ProjectionMatrix = getProjectionMatrix();
    glm::mat4 ViewMatrix = getViewMatrix();
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    
		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

    //
    //  Draw Ground
    //
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    ground_object.draw();

    //
    //  Draw Cloud
    //
    for (auto pos : cloud_pos) {
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(MVP * pos)[0][0]);
      cloud_object.draw();
    }

    //
    //  Draw Tree
    //
    auto t1 = translate(mat4(), glm::vec3(5, 0, 0));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(MVP * t1)[0][0]);
    tree_trunk_object.draw();

    //
    //  Draw Rocket
    //
    glm::mat4 rocket_trans = translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 rocket_rotate;

    if (isLaunched) {
      if (rotate >= pi / 2.0)
        isSafeToUnfold = true;
      if (!isUnfoldedParachute) {
        if (rotate < pi - 0.2)
          rotate += delta / pi;
        else
          isSafeToUnfold = false;

        if (sin(rotate) >= 0)
          move_y = power * sin(rotate);

        move_x = rotate * power / 32 + rotate * 16;
      } else {
        if (rotate < pi)
          rotate += delta / pi;

        if (move_y > 12.0) {
          move_y -= power / 1024;
          move_x += power / 512;
        }
      }

      rocket_trans = translate(mat4(), glm::vec3(move_x, move_y, move_z));
      rocket_rotate = eulerAngleXYZ(0.0, 0.0, -rotate);
    }

    auto rocket_tt = MVP * rocket_trans * rocket_rotate;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &rocket_tt[0][0]);
    rocket_body_object.draw();
    rocket_side_object.draw();
    rocket_head_object.draw();
    rocket_bottom_object.draw();

    if (isUnfoldedParachute) {
      parachute_object.draw();
      parachute_line_object.draw();
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

  // Cleanup VBO and shader
  // glDeleteBuffers(1, &vertexbuffer);
  // glDeleteBuffers(1, &uvbuffer);
  ground_object.dispose();
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}
