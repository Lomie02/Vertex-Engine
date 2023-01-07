#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 Colour;

void main()
{    
    color = vec4(Colour) * texture(image, TexCoords);
}  