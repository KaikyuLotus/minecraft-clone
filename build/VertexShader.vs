#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = projection * view * model * vec4(vPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
