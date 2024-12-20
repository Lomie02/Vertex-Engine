#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

float ChromaticOffset = 0.5;
void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	
	//float r = texture(text, TexCoords + vec2(ChromaticOffset,ChromaticOffset)).x;
	//float g = texture(text, TexCoords).y;
	//float b = texture(text, TexCoords - vec2(ChromaticOffset,ChromaticOffset)).z;
	//float a = texture(text, TexCoords).w;
	
    //color = vec4(r,g,b,a) * sampled;
	
	color = vec4(textColor) * sampled;
}  