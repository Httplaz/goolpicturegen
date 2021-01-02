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

int mod(int a, int b)
{
	if (b == 0)
		return 0;
	else
		return(a%b);
}

vec3 hsl_to_rgb(int h, float s, float l, float cr, float cg, float cb)
{
	float c = (1 - abs(2 * l - 1)) * s;
	float x = c * (1 - abs( mod(h / 60, 2) - 1));
	float m = l - c / 2;

	float dr, dg, db;

	if (h >= 300)
	{
		dr = c;
		dg = 0;
		db = x;
	}
	else if (h >= 240)
	{
		dr = x;
		dg = 0;
		db = c;
	}
	else if (h >= 180)
	{
		dr = 0;
		dg = x;
		db = c;
	}
	else if (h >= 120)
	{
		dr = 0;
		dg = c;
		db = x;
	}
	else if (h >= 60)
	{
		dr = x;
		dg = c;
		db = 0;
	}
	else
	{
		dr = c;
		dg = x;
		db = 0;
	}

	vec3 ans = vec3(dr*256*cr, dg*256*cg, db*256*cb);

	return ans;
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
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


	float z = 1;

	for (int i=0; i<formuleLength; i++)
	{
		int action = int(texture2D(formule, vec2(1.0f/formuleLength*i, 1)).x*255);
		//action = 100;
		//tz = action;
		switch(action)
		{
			case 0:
			z*=x;
			break;
			case 1:
			z*=y;
			break;
			case 2:
			z*=abs(x-y);
			break;
			case 3:
			z*= sin(x*pi);
			break;
			case 4:
			z*= sin(y*pi);
			break;
			case 5:
			z*= cos(x*pi);
			break;
			case 6:
			z*= cos(y*pi);
			break;
			case 7:
			z*= tan(x*pi);
			break;
			case 8:
			z*= tan(y*pi);
			break;
			case 9:
			z*= (int(x) & int(y));
			break;
			case 10:
			z*= (int(z) & int(x));
			break;
			case 11:
			z*= (int(z) & int(y));
			break;
			case 12:
			z*= (int(x) ^ int(y));
			break;
			case 13:
			z*= (int(z) ^ int(x));
			break;
			case 14:
			z*= (int(z) ^ int(y));
			break;
			case 15:
			z*= (int(x) | int(y));
			break;
			case 16:
			z*= (int(z) | int(x));
			break;
			case 17:
			z*= (int(z) | int(y));
			break;
			case 18:
			z*= (int(x) % int(y));
			break;
			case 19:
			z*= (int(z) % int(x));
			break;
			case 20:
			z*= (int(z) % int(y));
			break;
		}
	}

	pixel.xy = pixelPos;
	int fz = int(z)%360;

	pixel.z = fz/float(255);
	pixel.x = pixel.z;
	pixel.y = pixel.z;
	if(color)
	{
		vec3 hsv = vec3(fz*1.4117, fz/2.55, fz/2.55);
		vec3 rgb = hsv2rgb(vec3(hsv.x/360.0, hsv.y/100.0, hsv.z/100.0));
		pixel.xyz = rgb;
	}

    gl_FragColor = pixel;
}