#version 300 es
//#ifdef GL_ES
precision highp float;
//#endif
int formuleLength = 2;
int coordFormuleLength = 2;
uniform ivec2 windowSize;
vec2 scale = vec2(1000,1000);
vec2 camPos = vec2(0,0);
const float pi = 3.14159265;
int cutModule = 256; //power of two
int aaPow = 4;
bool cutInvert = false;
bool coloring = false;
float hueValue = 0.;
ivec2 f1 = ivec2(9, 3);
ivec2 f2 = ivec2(19,16);

out vec4 fragColor;

vec3 hsl_to_rgb(int h, float s, float l)
{
  float c = (1. - abs(2. * l - 1.)) * s;
  float x = c * (1. - abs( mod(float(h) / 60., 2.) - 1.));
  float m = l - c / 2.;

  float dr, dg, db;

  if (h >= 300)
  {
    dr = c;
    dg = 0.;
    db = x;
  }
  else if (h >= 240)
  {
    dr = x;
    dg = 0.;
    db = c;
  }
  else if (h >= 180)
  {
    dr = 0.;
    dg = x;
    db = c;
  }
  else if (h >= 120)
  {
    dr = 0.;
    dg = c;
    db = x;
  }
  else if (h >= 60)
  {
    dr = x;
    dg = c;
    db = 0.;
  }
  else
  {
    dr = c;
    dg = x;
    db = 0.;
  }

  vec3 ans = vec3(0., 0., 0.);

  return vec3(dr, dg, db);
}

float hypot(float x, float y)
{ 
  return sqrt(x*x+y*y);
}

float coordFunc(int x, int y, int action)
{
  //switch(action)
  {
    if(action== 0)
      return float(x);
    else if(action==  1)
      return float(y);
    else if(action==  2)
      return float(x+y);
    else if(action== 3)
      return float(x-y);
    else if(action== 4)
      return float(x*y);
    else if(action== 5)
      return float(x/y);
    else if(action== 6)
      return float(x%y);
    else if(action== 7)
      return float(y%x);
    else if(action== 8)
      return float((x|y));
    else if(action== 9)
      return sqrt(float(x));
    else if(action== 10)
      return sqrt(float(y));
    else if(action== 11)
      return log(float(x));
    else if(action== 12)
      return log(float(y));
    else if(action== 13)
      return exp(float(x));
    else if(action== 14)
      return exp(float(y));
    else if(action== 15)
      return float(x^y);
    else if(action== 16)
      return float(x&y);
    else if(action== 17)
      return (sin(float(x)*0.01) + 1.) * float(cutModule);
    else if(action== 18)
      return (sin(float(y)*0.01) + 1.) * float(cutModule);
    else if(action== 19)
      return (cos(float(x)*0.01) + 1.) * float(cutModule);
    else if(action== 20)
      return (cos(float(y)*0.01) + 1.) * float(cutModule);
    else if(action== 21)
      return hypot(float(x), float(y));
    else if(action== 22)
      return float(cutModule)*(float(x)/hypot(float(x), float(y+1))/4.*float(y/abs(y)));
    else if(action== 23)
      return 1.;
  }
}

int coordAction(int i, int ci)
{
  //return int(texelFetch(coordFormule, ivec2(ci, 0), 0).r * 255);
  if(ci==0)
    return f2.x;
  if(ci==1)
    return f2.y;
}

int func(int x, int y, int a, int b) 
{
  int z = 1;
  int ci = 0;
  for (int i = a; i < b; i++) 
  {

    //int action = int(texelFetch(formule, ivec2(i, 0), 0).r * 255);
    int action = 1;
    if(i==0)
      action = f1.x;
    if(i==1)
      action = f1.y;
    int h = int(hypot(float(x),float(y)));
    int x1 = x;
    int y1 = y;

    switch(action/5)
    {
      case 0:
        x1 = x;
        y1 = y;
        break;
      case 1:
        x1 = int(hypot(float(x),float(y)));
        y1 = y;
        break;
      case 2:
        x1 = int(coordFunc(x, y, coordAction(i, ci)));
        ci++;
        y1 = int(coordFunc(x, y, coordAction(i, ci)));
        ci++;
        break;
    }

    switch (action%5) 
    {
    //linear
    case 0:
      z = int(float(z)*coordFunc(x1, y1, coordAction(i, ci)));
      ci++;
      //ci = (ci+coordFormuleLength)%coordFormuleLength;
      i++;
      break;
    case 1:
      z = int(float(z)/coordFunc(x1, y1, coordAction(i, ci)));
      ci++;
      //ci = (ci+coordFormuleLength)%coordFormuleLength;
      i++;
      break;
    case 2:
      z = z%int(coordFunc(x1, y1, coordAction(i, ci)));
      ci++;
      //ci = (ci+coordFormuleLength)%coordFormuleLength;
      i++;
      break;
    case 3:
      z = int(float(z)+coordFunc(x1, y1, coordAction(i, ci)));
      ci++;
      //ci = (ci+coordFormuleLength)%coordFormuleLength;
      i++;
      break;
    case 4:
      z = z-int(coordFunc(x1, y1, coordAction(i, ci)));
      ci++;
      //ci = (ci+coordFormuleLength)%coordFormuleLength;
      i++;
      break;
    }

  }
  return z;
}

vec2 Pos;
vec2 Ac;

float render(vec2 lPos) 
{
  return float(int(func(int(Pos.x + Ac.x * lPos.x), int(Pos.y + Ac.y * lPos.y), 0, formuleLength)) & (cutModule-1)) / float(cutModule-1);
}

void main() 
{
  vec2 wSize = vec2(windowSize);
  Pos = (gl_FragCoord.xy - wSize * 0.5) / wSize.y * 2. * scale + vec2(camPos.x, -camPos.y) * 0.1;
  Ac = 1. / wSize.y * 2. * scale;


  float pixel = 0.;

  float s = 1./float(aaPow);

  for (int i=0; i<aaPow; i++)
    for (int j=0; j<aaPow; j++)
      pixel+= render(vec2(float(i)*s, float(j)*s));

  pixel/=float(aaPow*aaPow);

  //if(!coloring)
    fragColor = vec4(vec3(pixel), 1.);
  //else
  {
    //vec3 col = hsl_to_rgb(int(pixel*360.*hueValue), pixel*1.4, pixel/2.);
    //fragColor = vec4(col.x, col.y, col.z, 1.);
  }
  //fragColor = vec4(0., 0., 1., 0.);
 }