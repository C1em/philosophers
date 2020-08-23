/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coremart <coremart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 03:09:32 by coremart          #+#    #+#             */
/*   Updated: 2019/12/04 08:45:34 by coremart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include "philo.h"

#define STR_TABLE_SIZE "0.6"
#define _STR(x) #x
#define STR(x) _STR(x)
#define SQRT_2 "1.414213562373095048801688724209698078569671875376948073176679738"


GLuint	compile_shaders(const char *vertex_shader, const char *fragment_shader)
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	GLint success;
	char info_log[512];
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(fs, 512, NULL, info_log);
		printf("Error in fragment shader:\n%s", info_log);
	}
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(vs, 512, NULL, info_log);
		printf("Error in vertex shader:\n%s", info_log);
	}
	glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_programme, 512, NULL, info_log);
		printf("Error in linking shader:\n%s", info_log);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return (shader_programme);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow		*init_win(void)
{
	GLFWwindow* window;

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!(window = glfwCreateWindow(640, 640, "Hello Circle", NULL, NULL)))
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD\n");
		exit(1);
	}
	return (window);
}

unsigned int	*init_tex(GLuint shader_programme)
{
	unsigned int *tex_arr;
	int width;
	int height;
	int nrChannels;
	unsigned char *data;

	if (!(tex_arr = (unsigned int*)malloc(sizeof(unsigned int) * 5)))
		exit(1);
	glGenTextures(1, &tex_arr[tex_table]);
	glBindTexture(GL_TEXTURE_2D, tex_arr[tex_table]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("asset/table.jpg", &width, &height, &nrChannels, STBI_rgb);
	if (!data)
	{
		printf("failed to load texture\n");
		exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &tex_arr[tex_sleep]);
	glBindTexture(GL_TEXTURE_2D, tex_arr[tex_sleep]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("asset/sleep.png", &width, &height, &nrChannels, STBI_rgb);
	if (!data)
	{
		printf("failed to load texture\n");
		exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &tex_arr[tex_eat]);
	glBindTexture(GL_TEXTURE_2D, tex_arr[tex_eat]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("asset/eat.png", &width, &height, &nrChannels, STBI_rgb);
	if (!data)
	{
		printf("failed to load texture\n");
		exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenTextures(1, &tex_arr[tex_think]);
	glBindTexture(GL_TEXTURE_2D, tex_arr[tex_think]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("asset/think.png", &width, &height, &nrChannels, STBI_rgb);
	if (!data)
	{
		printf("failed to load texture\n");
		exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	glUseProgram(shader_programme);

	glGenTextures(1, &tex_arr[tex_fork]);
	glBindTexture(GL_TEXTURE_2D, tex_arr[tex_fork]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("asset/fork.png", &width, &height, &nrChannels, STBI_rgb);
	if (!data)
	{
		printf("failed to load texture\n");
		exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return (tex_arr);
}

void		*window(t_table *table)
{
	GLFWwindow	*window;
	GLuint		vbo;
	GLuint		ebo;
	GLuint		vao;
	const char vertex_shader[] =
	"#version 330\n"
	"layout (location = 0) in vec2 vp;"
	"layout (location = 1) in vec2 aTexCoord;"
	"layout (location = 2) in float index;"
	"uniform int right_or_left["STR(NB_PHILO)"];"
	"out vec2 TexCoord;"
	"flat out int obj;"
	"void main() {"
	"	obj = int(index);"
	"	if (obj > "STR(NB_PHILO)" && right_or_left[obj - 1 - "STR(NB_PHILO)"] == "STR(TO_RIGHT)")"
	"	{"
	"		gl_Position = vec4(vp.x * cos("STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)"))"
	"								- vp.y * sin("STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)")),"
	"								vp.x * sin("STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)"))"
	"								+ vp.y * cos("STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)")),"
	"								1.0, 1.0);"
	"	}"
	"	else if (obj > "STR(NB_PHILO)" && right_or_left[obj - 1 - "STR(NB_PHILO)"] == "STR(TO_LEFT)")"
	"	{"
	"		gl_Position = vec4(vp.x * cos(-"STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)"))"
	"								- vp.y * sin(-"STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)")),"
	"								vp.x * sin(-"STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)"))"
	"								+ vp.y * cos(-"STR(M_PI)"* (25 -"STR(NB_PHILO)") / (25 * "STR(NB_PHILO)")),"
	"								1.0, 1.0);"
	"	}"
	"	else"
	"		gl_Position = vec4(vp, 1.0, 1.0);"
	"	TexCoord = aTexCoord;"
	"}";
	const char fragment_shader[] =
	"#version 330\n"
	"out vec4 frag_colour;"
	"uniform sampler2D tex_arr["STR(NB_PHILO * 2 + 1)"];"
	"in vec2 TexCoord;"
	"flat in int obj;"
	"void main() {"
	"	vec4 tex_color = texture(tex_arr[obj], TexCoord);"
	"	if (vec3(tex_color.x, tex_color.y, tex_color.z) == vec3(0.0, 1.0, 0.0))"
	"		discard;"
	"	frag_colour = tex_color;"
	"}";
	unsigned int *tex_arr;

	window = init_win();
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	GLfloat points[(20 * (NB_PHILO * 2 + 1))];
	fill_points(points);
	glBufferData(GL_ARRAY_BUFFER, (20 * (NB_PHILO * 2 + 1)) * sizeof(GLfloat), points, GL_STATIC_DRAW);
	GLuint indices[6 * (NB_PHILO * 2 + 1)];
	fill_indices(indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6 * (NB_PHILO * 2 + 1), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	GLuint shader_programme = compile_shaders(vertex_shader, fragment_shader);

	tex_arr = init_tex(shader_programme);
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		int		tex_idx_arr[NB_PHILO * 2 + 1];
		fill_tex_idx(tex_idx_arr, table);
		glUniform1iv(glGetUniformLocation(shader_programme, "tex_arr"), NB_PHILO * 2 + 1, tex_idx_arr);
		glUniform1iv(glGetUniformLocation(shader_programme, "right_or_left"), NB_PHILO, (const GLint *)table->chopstick);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_arr[tex_table]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_arr[tex_sleep]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex_arr[tex_eat]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex_arr[tex_think]);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tex_arr[tex_fork]);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6 * (NB_PHILO * 2 + 1), GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glfwTerminate();
	printf("end window.c\n");
	return (NULL);
}
