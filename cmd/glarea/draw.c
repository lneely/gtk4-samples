#include <stdio.h>
#include <epoxy/gl.h>
#include <math.h>
#include <stddef.h>
#include "shaders.h"
#include "draw.h"

static GLuint program;			// shader program
static GLuint mvpidx;			// MVP matrix uniform location
static GLuint posidx;			// position attr location
static GLuint coloridx;			// color attr location
static GLuint vao;				// vertex array object
static GLfloat mvp[16];			// the mvp matrix

const float vertices[] = {
	// rect 1
	-1.00f,  1.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // top left
	-1.00f,  0.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // bottom left
	 0.00f,  0.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // bottom right
	 0.00f,  1.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // top right
	// rect 2
	-1.00f,  0.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // top left
	-1.00f, -1.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // bottom left
	 0.00f, -1.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // bottom right
	 0.00f,  0.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // top right
	// rect 3
	 0.00f,  1.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // top left
	 0.00f,  0.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // bottom left
	 1.00f,  0.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // bottom right
	 1.00f,  1.00f,  0.00f,   0.98f, 1.00f, 0.78f,   // top right
	// rect 4
	 0.00f,  0.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // top left
	 0.00f, -1.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // bottom left
	 1.00f, -1.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // bottom right
	 1.00f,  0.00f,  0.00f,   1.00f, 0.98f, 0.88f,   // top right
};

const unsigned int indices[] = {
	3, 2, 1,	// triangle 1
	1, 3, 0,	// triangle 2
	7, 6, 5,	// triangle 3
	5, 7, 4,	// triangle 4
	11, 10, 9,
	9, 11, 8,
	15, 14, 13,
	13, 15, 12
};

// draw_matrix_reset resets the values of a matrix to the identity matrix
static void
draw_matrix_reset(float* mtrx) {
	printf("%s\n", __func__);
	mtrx[0] = 1.f; mtrx[4] = 0.f;  mtrx[8] = 0.f; mtrx[12] = 0.f;
	mtrx[1] = 0.f; mtrx[5] = 1.f;  mtrx[9] = 0.f; mtrx[13] = 0.f;
	mtrx[2] = 0.f; mtrx[6] = 0.f; mtrx[10] = 1.f; mtrx[14] = 0.f;
	mtrx[3] = 0.f; mtrx[7] = 0.f; mtrx[11] = 0.f; mtrx[15] = 1.f;
}

// thanks: emanuelle bassi <https://github.com/ebassi>
static void 
draw_buffers_init(GLuint posidx, GLuint coloridx) {
	printf("%s\n", __func__);
	GLuint vbo;
	GLuint ebo;

	// create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// initialize the GL buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// store vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	// store index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

	/* enable and set the position attribute */
	glEnableVertexAttribArray (posidx);
	glVertexAttribPointer (posidx, 3, GL_FLOAT, GL_FALSE, 
		6 * sizeof(float), (GLvoid*)0);

	// /* enable and set the color attribute */
	glEnableVertexAttribArray (coloridx);
	glVertexAttribPointer (coloridx, 3, GL_FLOAT, GL_FALSE, 
		6 * sizeof(float), (GLvoid*)(3*sizeof(float)));

	// reset state and cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &vao);
}

void 
draw_render() {
	printf("%s\n", __func__);
	if(!vao) {
		printf("couldn't get vao\n");
		return;
	}

	glUseProgram(program);
	glUniformMatrix4fv(mvpidx, 1, GL_FALSE, &mvp[0]);
	glBindVertexArray(vao);
	// glDrawArrays(GL_TRIANGLES, 0, sizeof vertices / (sizeof(float)*6));
	glDrawElements(GL_TRIANGLES, sizeof indices / (sizeof(unsigned int)), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void 
draw_shutdown() {
	// destroy created objects
	if (vao)
		glDeleteVertexArrays (1, &vao);
	if(program)
		glDeleteProgram(program);
}

void 
draw_init() {
	int success;
	const char* devicename;

	// initialize the shaders
	// TODO: bubble up error message

	success = shaders_load(&program, &mvpidx, &posidx, &coloridx);
	if (!success) {
		printf("error initializing shaders\n");
		return;
	}

	devicename = (char *) glGetString (GL_RENDERER);
	printf("devicename=%s", devicename);

	// reset the mvp matrix and initialize the buffers
	draw_matrix_reset(mvp);
	draw_buffers_init(posidx, coloridx);
}
