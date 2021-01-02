#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>

using namespace std;
using namespace sf;

int fsx = 200, fsy = 200, sx = 400, sy = 400, fl = 4, fb = 9, mfb = 28, mfl = 6;
float scx = 1, scy = 1;
float camX = fsx/2, camY = fsy/2;
bool randomFB = true;
bool randomFL = true;
bool coloring = true;
Texture tex;
Texture sformtex;
Sprite spr;
Uint8* pixels = new Uint8[sx * sy * 4];
Uint8* sform = new Uint8[mfl*4];
bool windowIsOpen = true;
bool active = true;
int delay = 40;

const int gs = 32;

int hyper = 0;

const int NSEED = 10000;
unsigned long long seed[NSEED];
int Gs = 0;

string pictureName;

RenderWindow window(sf::VideoMode(sx, sy), "CSPG");

Shader shader;

unsigned __int32 random()
{
	Gs = (Gs + 1) % NSEED;
	seed[Gs] = (seed[Gs] * (Gs + 13221137) + 123451 - Gs) + seed[seed[Gs%NSEED] % NSEED];
	return(seed[Gs]);
}

void take_screenshot(const sf::RenderWindow& window)
{
	sf::Texture texture;
	texture.create(window.getSize().x, window.getSize().y);
	texture.update(window);
	Image img = texture.copyToImage();
	if (img.saveToFile(pictureName))
	{
		//yeah
	}
	else
	{
		//bad(
		int f = 2 / hyper;
	}
}

void start()
{
	for (int i = 0; i < NSEED; i++)
		seed[i] = clock();

	for (int i = 0; i < fl*4; i++)
		sform[i] = 1;

	tex.create(sx, sy);
	sformtex.create(fl, 1);
	//sformtex.update(sform);
	spr.setScale({ 1,1 });
	if (!shader.loadFromFile("frag.glsl", Shader::Fragment))
	{
		//int f = 2 / hyper;
		window.close();
	}

}

void update()
{
}

void updateWindow()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 2;
	settings.majorVersion = 5;
	settings.minorVersion = 0;
	window.setVerticalSyncEnabled(true);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) 
			{
				if (randomFL)
					fl = random() % (mfl - 1) + 1;
				for (int i = 0; i < fl * 4; i+=4)
					sform[i] = random() % fb;
				if (randomFB)
					fb = random() % (mfb-1)+1;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
				scx *= 1.1, scy *= 1.1;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
				scx /= 1.1, scy /= 1.1;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
				camY -= 3*scy;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
				camX -= 3*scx;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
				camY += 3 * scy;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
				camX += 3 * scx;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) 
			{
				pictureName = "";
				for (int i = 0; i < fb * 4; i += 4)
					pictureName = pictureName + to_string(sform[i]) + " ";
				pictureName = pictureName + to_string(camX) + " " + to_string(camY) + " " + to_string(scx) + " " + to_string(scy) + ".png";
				take_screenshot(window);
			}
		}
;
		window.clear({ 200,200,200 });
		for (int i = 0; i < sx * sy * 4; i++)
			pixels[i] = 255;
		tex.update(pixels);
		sformtex.update(sform);
		spr = Sprite(tex);
		shader.setUniform("texture", tex);
		shader.setUniform("formule", sformtex);
		shader.setUniform("formuleLength", fl);
		shader.setUniform("formuleBorder", fb);
		shader.setUniform("resolution", Glsl::Ivec2(fsx, fsy));
		shader.setUniform("windowSize", Glsl::Ivec2(sx, sy));
		shader.setUniform("scale", Glsl::Vec2(scx, scy));
		shader.setUniform("color", coloring);
		shader.setUniform("camPos", Glsl::Vec2(camX, camY));
		window.draw(spr, &shader);
		window.display();
	}
	windowIsOpen = false;
}


int main()
{
	start();
	//thread thr(updateWindow);
	//thr.detach();
	while (windowIsOpen)
	{
		if (active)
		{
			sleep(milliseconds(delay));
			updateWindow();
		}
	}
	return 0;
}
