#version 330
uniform sampler2D texture;
uniform sampler2D formule;
uniform int formuleLength;
uniform int formuleBorder;
uniform ivec2 resolution;
uniform ivec2 windowSize;
uniform vec2 scale;
uniform vec2 camPos;
uniform bool color;
float pi = 3.14;

float module(vec2 z)
{
	return sqrt(z.x*z.x + z.y*z.y);
}

vec2 square(vec2 z)
{
	return vec2(z.x*z.x-z.y*z.y, 2*z.x*z.y);
}

vec2 sum(vec2 a, vec2 b)
{
	return a+b;
}

void main()
{
	vec2 texCoord = vec2(gl_FragCoord.x/float(windowSize.x), gl_FragCoord.y/float(windowSize.y));
    vec4 pixel = texture2D(texture, texCoord);
	//vec4 pixel = vec4(0.8,0.5,0.4,1);
	float sx = textureSize(texture, 0).x;
	float sy = textureSize(texture, 0).y;
	vec2 pixelPos = vec2((texCoord.x-0.5)*resolution.x, (texCoord.y-0.5)*resolution.y);
	float x = pixelPos.x*scale.x;
	float y = pixelPos.y*scale.y;
	x+=camPos.x;
	y-=camPos.y;

	vec2 z = vec2(x, y);

	int col = 1;
	for (int i=0; i<30; i++)
	{
		z = sum(square(z), vec2(0.33,0.33));
		if(module(z)>2)
			break;
	}
	if(module(z)<2)
		col = 0;
	pixel.xyz = vec3(col, col, col);

    gl_FragColor = pixel;
}