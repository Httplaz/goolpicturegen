#include "imgui.h"
#include "imgui-sfml.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <random>  
#include <ctime>  

using namespace std;
using namespace sf;

int fsx = 200, fsy = 200, sx = 400, sy = 400, fl = 5, cfl = 15, fb = 15, cfb = 23;
float scx = 200, scy = 200;
//float camX = fsx/2, camY = fsy/2;
float camX = 0, camY = 0;
bool randomFB = false;
bool randomFL = false;
Texture tex;
Texture sformtex;
Texture csformtex;
Sprite spr;
Uint8* pixels = new Uint8[sx * sy * 4 * 10];
Uint8* sform = new Uint8[fl * 4 * 100];
Uint8* csform = new Uint8[fl * 4 * 100];
bool windowIsOpen = true;
bool active = true;
bool aa16 = true;
int delay = 40;
int antiAliasingPow = 4;
int maxAntiAliasingPow = 4;

int cutModulePow = 8;
bool cutInvert = false;
bool coloring = false;
float hueValue = 1;

int screenshots = 0;

mt19937 random(time(0));

int hyper = 0;

string pictureName;

RenderWindow window(sf::VideoMode(sx, sy), "CSPG");

Shader shader;

string textInput;

char dd[100];

vector<int> stringToVector(String s)
{
	vector<int> a;
	a.push_back(0);
	int j = 0;
	for (int i = 0; i < s.getSize(); i++)
	{
		if (s[i] == ' ')
			j++, a.push_back(0);
		else
			a[j] = a[j] * 10 + s[i] - '0';
	}
	return a;
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
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 0;
	//settings.antialiasingLevel = 2;
	settings.majorVersion = 5;
	settings.minorVersion = 0;
	window.setVerticalSyncEnabled(true);

	for (int i = 0; i < fl * 4; i++)
		sform[i] = 1;
	for (int i = 0; i < cfl * 4; i++)
		csform[i] = 1;

	tex.create(sx, sy);
	sformtex.create(fl, 1);
	csformtex.create(cfl, 1);
	//sformtex.update(sform);
	spr.setScale({ 1,1 });
	if (!shader.loadFromFile("frag.glsl", Shader::Fragment))
	{
		//int f = 2 / hyper;
		window.close();
	}
	ImGui::SFML::Init(window);

}

void update()
{
}

void startFormula()
{
	for (int i = 0; i < fl * 4; i += 4)
		sform[i] = random() % fb;
	for (int i = 0; i < cfl * 4; i += 4)
		csform[i] = random() % cfb;
}

void saveImage()
{
	pictureName = to_string(fl) + " " + to_string(cfl) + " ";
	for (int i = 0; i < fl * 4; i += 4)
		pictureName = pictureName + to_string(sform[i]) + " ";
	for (int i = 0; i < cfl * 4; i += 4)
		pictureName = pictureName + to_string(csform[i]) + " ";
	pictureName = pictureName + ".png";
	take_screenshot(window);
}

void updateShader()
{
	tex.update(pixels);
	sformtex.update(sform);
	csformtex.update(csform);
	spr = Sprite(tex);


	//shader.setUniform("texture", tex);
	//shader.setUniform("formule", sformtex);
	//shader.setUniform("formuleLength", fl);
	//shader.setUniform("formuleBorder", fb);
	//shader.setUniform("coordFormule", csformtex);
	//shader.setUniform("coordFormuleLength", cfl);
	//shader.setUniform("coordFormuleBorder", cfb);
	//shader.setUniform("resolution", Glsl::Ivec2(fsx, fsy));
	shader.setUniform("windowSize", Glsl::Ivec2(window.getSize().x, window.getSize().y));
	//shader.setUniform("scale", Glsl::Vec2(scx, scy));
	//shader.setUniform("coloring", coloring);
	//shader.setUniform("camPos", Glsl::Vec2(camX, camY));
	//shader.setUniform("aa16", aa16);
	//shader.setUniform("cutModule", (int)(pow(2, cutModulePow)));
	//shader.setUniform("aaPow", antiAliasingPow);
	//shader.setUniform("hueValue", hueValue);
	//shader.setUniform("cutInvert", cutInvert);
}

void updateWindow()
{

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				startFormula();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
				scx *= 1.1, scy *= 1.1;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
				scx /= 1.1, scy /= 1.1;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)
				aa16 = !aa16;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
				camY -= 1 * scy;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
				camX -= 1 * scx;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
				camY += 1 * scy;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
				camX += 1 * scx;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)
			{
				screenshots++;
				//saveImage();
			}
		}


		window.clear({ 200,200,200 });
		for (int i = 0; i < sx * sy * 4; i++)
			pixels[i] = 255;

		updateShader();

		ImGui::SFML::Update(window, deltaClock.restart());
		float color[3] = { 0.f, 0.f, 0.f };
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Settings");
		ImGui::SliderInt("cutModulePow", &cutModulePow, 1, 32);
		ImGui::SliderInt("antiAliasing", &antiAliasingPow, 1, maxAntiAliasingPow);
		ImGui::SliderInt("maxAntiAliasing", &maxAntiAliasingPow, 1, 16);
		ImGui::SliderInt("formulelength", &fl, 1, 32);
		ImGui::SliderInt("coordFormulelength", &cfl, 1, 32);
		ImGui::Checkbox("coloring", &coloring);
		ImGui::SliderFloat("hueValue", &hueValue, 0, 4);
		ImGui::Checkbox("cutInvert", &cutInvert);
		ImGui::InputText("custom world", dd, 100, 0);
		if (ImGui::Button("load"))
		{
			vector<int> a = stringToVector(dd);
			fl = a[0];
			cfl = a[1];
			int j = 2;
			for (int i = 0; i < fl; i++)
				sform[i * 4] = a[j], j++;
			for (int i = 0; i < cfl; i++)
				csform[i * 4] = a[j], j++;

		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, 100));
		ImGui::Begin("Info");
		ImGui::Text("Move - WASDQE");
		ImGui::End();

		window.draw(spr, &shader);

		if (screenshots != 0)
		{
			saveImage();
			screenshots--;
		}

		ImGui::SFML::Render(window);
		window.display();
	}
	windowIsOpen = false;
	ImGui::SFML::Shutdown();
}

void updateWindow2()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };

	// здесь мы будем использовать массив char. Чтобы использовать
	// std::string нужно сделать действия, описанные во второй части
	char windowTitle[255] = "ImGui + SFML = <3";
	window.setTitle(windowTitle);

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Sample window"); // создаём окно

		// Инструмент выбора цвета
		if (ImGui::ColorEdit3("Background color", color)) {
			// код вызывается при изменении значения, поэтому всё
			// обновляется автоматически
			bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
			bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
			bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
		}

		ImGui::InputText("Window title", windowTitle, 255);

		if (ImGui::Button("Update window title")) {
			// этот код выполняется, когда юзер жмёт на кнопку
			// здесь можно было бы написать 
			// if(ImGui::InputText(...))
			window.setTitle(windowTitle);
		}
		ImGui::End(); // end window

		window.clear(bgColor); // заполняем окно заданным цветом
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}

int main()
{
	start();
	updateWindow();
	//thread thr(updateWindow);
	//thr.detach();
	/*while (windowIsOpen)
	{
		if (active)
		{
			sleep(milliseconds(delay));
			updateWindow();
		}
	}*/
	return 0;
}
