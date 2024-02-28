// a simple shader utility library
// thanks: emanuelle bassi <github.com/ebassi/glarea-example>

#include <stdio.h>
#include <stdlib.h>
#include <epoxy/gl.h>

static GLuint shaders_create(int, const char*, GLuint*);

// embedded vertex shader GLSL source code
static const char* vertexsrc = "#version 130\n"
	"in vec3 position;\n"
	"in vec3 color;\n"
	"uniform mat4 mvp;\n"
	"smooth out vec4 vertexColor;\n"
	"void main() {\n"
  	"	gl_Position = mvp * vec4(position, 1.0);\n"
  	"	vertexColor = vec4(color, 1.0);\n"
	"}\n";

// embedded fragmentation shader GLSL source code
static const char* fragmentsrc = "#version 150\n"
	"smooth in vec4 vertexColor;\n"
	"out vec4 outputColor;\n"
	"void main() {\n"
	"	outputColor = vertexColor;\n"
	"}\n";

// shaders_load is the point of entry for this library
bool
shaders_load (GLuint* prog, GLuint* mvpidx, GLuint* posidx, GLuint* coloridx) {
	GLuint program = 0;
	GLuint mvp_location = 0;
	GLuint vtxtmp = 0, fragtmp = 0;
	GLuint position_location = 0;
	GLuint color_location = 0;
	int status = 0;

	// create shaders from GLSL source code
	shaders_create (GL_VERTEX_SHADER, vertexsrc, &vtxtmp);
	shaders_create (GL_FRAGMENT_SHADER, fragmentsrc, &fragtmp);

	// link vertex and fragment shaders
	program = glCreateProgram ();
	glAttachShader (program, vtxtmp);
	glAttachShader (program, fragtmp);
	glLinkProgram (program);
	
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_TRUE) {
		printf("getting mvp, position, and color indices\n");
		// get model/view/projection uniform location
		mvp_location = glGetUniformLocation (program, "mvp");

		// get "position" and "color" attribute locations
		position_location = glGetAttribLocation (program, "position");
		color_location = glGetAttribLocation (program, "color");
	} else {
		printf("failed linking vertex and fragment shaders\n");
		int log_len = 0;
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &log_len);

		char *buffer = malloc (log_len + 1);
		glGetProgramInfoLog (program, log_len, NULL, buffer);
		printf("Linking failure in program: %s", buffer);
		free (buffer);

		glDeleteProgram (program);
		program = 0;		
	}

	// detach temporary vertex and fragment shaders
	printf("cleaning up temporary shaders\n");
	glDetachShader (program, vtxtmp);
	glDetachShader (program, fragtmp);	

	printf("all should be true: %d %d %d %d %d %d\n",
		vtxtmp!=0, fragtmp!=0, prog!=NULL, 
		mvpidx!=NULL, posidx!=NULL, coloridx!=NULL);
	if (vtxtmp)
		glDeleteShader (vtxtmp);
	if (fragtmp)
		glDeleteShader (fragtmp);
	if (prog)
		*prog = program;
	if (mvpidx)
		*mvpidx = mvp_location;
	if (posidx)
		*posidx = position_location;
	if (coloridx)
		*coloridx = color_location;

	return program != 0;
}

static GLuint
shaders_create (int type, const char *source, GLuint *dest) {
	printf("compiling shader %s\n", source);
	GLuint shader = glCreateShader (type);
	glShaderSource (shader, 1, &source, NULL);
	glCompileShader (shader);

	int status;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int log_len;
		char *buffer = malloc (log_len + 1);

		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_len);
		glGetShaderInfoLog (shader, log_len, NULL, buffer);
		printf("Compilation failure in %s shader: %s",
			type == GL_VERTEX_SHADER ? "vertex" : "fragment",
			"buffer");

		free(buffer);
		glDeleteShader (shader);
		shader = 0;
	}

	if (dest != NULL) 
		*dest = shader;
	else
		printf("failed to return shader\n");

	return shader != 0;
}
