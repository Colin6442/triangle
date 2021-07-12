#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <glad/glad.h>
#include <Windows.h>
//#include <cstdlib>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#define PI 3.14159265



void CalculateFrameRate(){
	static float framesPerSecond = 0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount64() * 0.001f;
	++framesPerSecond;
	printf("Current Frames Per Second: %d\n\n", fps);
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}

void Rotate(int turn, float* length, GLfloat *vertices, float* currentAngle, double* angleA, double* angleB) {
	*currentAngle += turn;
	*angleA += turn;
	*angleB += turn;
	vertices[0] = (GLfloat)(*length * cos(*angleA * PI / 180.0) + vertices[6]);
	vertices[1] = (GLfloat)(*length * sin(*angleA * PI / 180.0) + vertices[7]);
	vertices[3] = (GLfloat)(*length * cos(*angleB * PI / 180.0) + vertices[6]);
	vertices[4] = (GLfloat)(*length * sin(*angleB * PI / 180.0) + vertices[7]);
}

void Movement(double xVel, double yVel, GLfloat *vertices, float currentAngle) {
	vertices[0] += (GLfloat)(xVel * cos(currentAngle * PI / 180.0)); vertices[1] += (GLfloat)(yVel * sin(currentAngle * PI / 180.0));
	vertices[3] += (GLfloat)(xVel * cos(currentAngle * PI / 180.0)); vertices[4] += (GLfloat)(yVel * sin(currentAngle * PI / 180.0));
	vertices[6] += (GLfloat)(xVel * cos(currentAngle * PI / 180.0)); vertices[7] += (GLfloat)(yVel * sin(currentAngle * PI / 180.0));
}

void checkBox() {

}

int main(){
	// Initialize GLFW
	int windowX = 900;
	int windowY = 900;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(windowX, windowY, "Test Thing", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glViewport(0, 0, windowX, windowY);


	GLfloat vertices[] = {
		-0.05f, 0.1f, 0.0f, // Lower left corner	PointA
		0.05f, 0.1f, 0.0f, // Lower right corner	PointB
		0.0f, 0.0f, 0.0f // Upper corner			PointC
	};

	GLfloat square[] = {
	-0.1f, 0.1f, // Lower left corner	PointA
	-0.1f, -0.1f, // Lower right corner	PointB
	0.1f, -0.1f, // Upper corner			PointC
	0.1f, -0.1f,
	};

	float currentAngle = 270;
	float length = sqrt(pow((float)vertices[0], 2.0f) + pow((float)vertices[1], 2.0f));
	double angleA = acos(vertices[0] / length) * 180.0/PI;
	double angleB = acos(vertices[3] / length) * 180.0/PI;
	float slope = 0;
	double xVel = 0;
	double yVel = 0;


	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VBO VBO1(vertices, sizeof(vertices));

	VAO1.Bind();
	VAO1.LinkVBO(VBO1, 0);

	VAO1.Unbind();
	VBO1.Unbind();

	float one = 0.07f, two = 0.13f, three = 0.17f;

	while (!glfwWindowShouldClose(window)) {
		//acos(val * PI / 180.0);
		//cos(angle * PI / 180.0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if (xVel < 0.05) {
				xVel += 0.001;
			}
			if (yVel < 0.05) {
				yVel += 0.001;
			}
		}else{
			if (xVel > 0) {
				xVel -= 0.0005;
			}
			else if (xVel < 0) {
				xVel = 0;
			}
			if (yVel > 0) {
				yVel -= 0.0005;
			}
			else if (yVel < 0) {
				yVel = 0;
			}

		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (xVel > 0 || yVel > 0) {
				Rotate(5, &length, vertices, &currentAngle, &angleA, &angleB);
			}

		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (xVel > 0 || yVel > 0) {
				Rotate(-5, &length, vertices, &currentAngle, &angleA, &angleB);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			xVel = 0;
			yVel = 0;
		}

		Movement(xVel, yVel, vertices, currentAngle);
		//x6,y7
		if (vertices[6] > 1) {
			vertices[6] = -1;//-0.001;
		}
		else if (vertices[6] < -1) {
			vertices[6] = 1;//+0.001;
		}
		if (vertices[7] > 1) {
			vertices[7] = -1;// -0.001;
		}
		else if (vertices[7] < -1) {
			vertices[7] = 1;// +0.001;
		}
		Rotate(0, &length, vertices, &currentAngle, &angleA, &angleB);


		VBO1.Bind();
		//VBOsq.Bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(one, two, three, 1.0f);		
		shaderProgram.Activate();
		VAO1.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(10);
		//CalculateFrameRate();
	}

	VAO1.Delete();
	VBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}