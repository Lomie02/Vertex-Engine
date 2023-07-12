#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 Colour;
uniform int UseDepth;

in vec3 _worldNormals;
uniform int UseLights = 0;
uniform vec3 lights;

float near = 0.1;
float far = 100.0f;
vec3 worldSpaceNormals;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{    
	 worldSpaceNormals.x = 1 ;
	worldSpaceNormals.y = 1;
	worldSpaceNormals.z = 1;
	
	if(UseDepth == 0)
	{
		if(UseLights == 1)
		{
			float lightVal = max(dot(_worldNormals, lights), 0.0);
			color += vec4(Colour) * texture(image, TexCoords) * lightVal;
		}else
		{
			color = vec4(Colour) * texture(image, TexCoords);
		}
		
	}
	else
	{
		float depth = LinearizeDepth(gl_FragCoord.z) / far;
		color = vec4(vec3(depth),1.0);
	}
}  