#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
void main()
{
	gl_Position = position;
	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;

uniform vec4 u_Colour;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColour = texture(u_Texture, v_TexCoord);
	colour = texColour;
	colour = 0.5 * (texColour + u_Colour);
	colour[3] = texColour[3];
	//if(texColor[3]>0.001f)
	//{
	//	colour = 0.5 * (texColor + u_Colour);
	//}
	//else
	//{
	//	colour = texColor;
	//}
};
