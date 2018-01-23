#version 330 core

// Ouput data
out vec4 color;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(){
	color = texture(ourTexture, TexCoord);

}