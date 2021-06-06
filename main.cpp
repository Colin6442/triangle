#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include<glad/glad.h>
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include<GLFW/glfw3.h>
#include<math.h>

#define PI 3.14159265

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.3f, 0.8f, 0.02f, 1.0f);\n"
"}\n\0";


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



int main(){
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1600, 900, "Test Thing", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glViewport(0, 0, 900, 900);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLfloat vertices[] = {
		-0.05f, 0.1f, 0.0f, // Lower left corner	PointA
		0.05f, 0.1f, 0.0f, // Lower right corner	PointB
		0.0f, 0.0f, 0.0f // Upper corner			PointC
	};

	float currentAngle = 270;
	float length = sqrt(pow((float)vertices[0], 2.0f) + pow((float)vertices[1], 2.0f));
	float angleA = acos(vertices[0] / length) * 180.0/PI;
	float angleB = acos(vertices[3] / length) * 180.0/PI;
	float slope = 0;

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	float one = 0.07f, two = 0.13f, three = 0.17f;
	glClearColor(one, two, three, 1.0f);
	//loop
	//0,1
	//3,4
	//6,7
	float x = -(vertices[0] + vertices[3]) / 2;
	float y = -(vertices[1] + vertices[4]) / 2;

	while (!glfwWindowShouldClose(window)){

		//acos(val * PI / 180.0);
		//cos(angle * PI / 180.0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			vertices[0] += 0.01 * cos(currentAngle * PI / 180.0); vertices[1] += 0.01 * sin(currentAngle * PI / 180.0);
			vertices[3] += 0.01 * cos(currentAngle * PI / 180.0); vertices[4] += 0.01 * sin(currentAngle * PI / 180.0);
			vertices[6] += 0.01 * cos(currentAngle * PI / 180.0); vertices[7] += 0.01 * sin(currentAngle * PI / 180.0);

		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			currentAngle += 5;
			angleA += 5;
			angleB += 5;
			vertices[0] = (float)length * cos(angleA * PI / 180.0) + vertices[6];
			vertices[1] = (float)length * sin(angleA * PI / 180.0) + vertices[7];
			vertices[3] = (float)length * cos(angleB * PI / 180.0) + vertices[6];
			vertices[4] = (float)length * sin(angleB * PI / 180.0) + vertices[7];

		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			//std::cout << "s press" << std::endl;
			vertices[0] = -0.05f; vertices[1] = 0.1f;
			vertices[3] = 0.05f; vertices[4] = 0.1f;
			vertices[6] = 0; vertices[7] = 0;

		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			currentAngle -= 5;
			angleA -= 5;
			angleB -= 5;
			vertices[0] = (float)length * cos(angleA * PI / 180.0) + vertices[6];
			vertices[1] = (float)length * sin(angleA * PI / 180.0) + vertices[7];
			vertices[3] = (float)length * cos(angleB * PI / 180.0) + vertices[6];
			vertices[4] = (float)length * sin(angleB * PI / 180.0) + vertices[7];
		}

		

		glBindBuffer(GL_ARRAY_BUFFER, VBO); //puts VBO in
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //sets value
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(one, two, three, 1.0f);		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(10);
		//CalculateFrameRate();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}