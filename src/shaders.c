
const char *vertexSource = R"(
	#version 150

	in vec2 position;

	uniform mat4 viewport;

	void main(){
		gl_Position = viewport * vec4(position, 0.0, 1.0);
	}
)";


const char *fragmentSource = R"(
	#version 150

	out vec4 outColor;

	void main(){
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
)";

