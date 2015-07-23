#ifdef BUILD_VIEWER

const char *vertexSource =
	"#version 150\n"

	"in vec2 position;"

	"uniform mat4 viewport;"

	"void main(){"
	"	gl_Position = viewport * vec4(position, 0.0, 1.0);"
	"}";


const char *fragmentSource =
	"#version 150\n"

	"uniform vec3 Color;"

	"out vec4 outColor;"

	"void main(){"
	"	outColor = vec4(Color, 1.0);"
	"}";


#endif
