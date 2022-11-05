#include <iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <time.h>
using namespace std;
using namespace sf;

const int r = 25;
const int c = 40;
int field[r][c] = { 0 };

int l = 18;
//fan struct
struct Fan
{
	int x, y, dx, dy;
	Fan() {
		x = y = 300;
		dx = 4 - rand() % 8;
		dy = 4 - rand() % 8;
	}
	void move() {
		x += dx; if (field[y/l][x/l]==1) { dx = - dx; x += dx; }
		y += dy; if (field[y / l][x / l] == 1) { dy = - dy; y += dy; }
	}
};
void drop(int y, int x)
{
	if (field[y][x] == 0) field[y][x] = -1;
	if (field[y - 1][x] == 0) drop(y - 1, x);
	if (field[y + 1][x] == 0) drop(y + 1, x);
	if (field[y][x - 1] == 0) drop(y, x - 1);
	if (field[y][x + 1] == 0) drop(y, x + 1);
}
int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(c*l, r*l), "xonix");
	window.setFramerateLimit(60);

	Texture t1, t2;
	if (!t1.loadFromFile("C:/Users/noran/source/repos/xonix/images/tiles.png")) {
		cout << "Error tiles" << endl;
	}
	if (!t2.loadFromFile("C:/Users/noran/source/repos/xonix/images/fan.png")) {
		cout << "Error fan" << endl;
	}
	Sprite tiles(t1), fan(t2);
	SoundBuffer buffer;
	if (!buffer.loadFromFile("C:/Users/noran/source/repos/xonix/crash.wav")) {
		cout << "music not work" << endl;
	}
	Sound sound;
	sound.setBuffer(buffer);

	Music music;
	if (!music.openFromFile("C:/Users/noran/source/repos/xonix/music.ogg")) {
		cout << "music not work";
	}
	music.play();
	music.setLoop(true);
	//font 
	Font font;
	if (!font.loadFromFile("C:/Users/noran/source/repos/xonix/FontFile.ttf")) {
		cout << "error loading file" << endl;
	}
	Text text;
	text.setString("GameOver");
	text.setFillColor(Color::Red);
	text.setFont(font);
	text.setPosition(260, 225);
	text.setCharacterSize(100);
	text.setStyle(Text::Style::Bold);
	text.setOutlineColor(Color::Yellow);
	text.setOutlineThickness(10);
	fan.setOrigin(20, 20);
	int dx = 0, dy = 0;
	int x = 0; int y = 0; 
	float delay = 0.07;
	float timer = 0;
	Clock clock;
	int fanCount = 4;
	Fan f[10];
	bool game = true;
	//BOUNDRIES
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (i == 0 || j == 0 || i == r - 1 || j == c - 1) {
				field[i][j] = 1;
			}
		}
	}

	//GAMELOOP
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) {
				window.close();
			}

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Escape)
				{
					for (int i = 1; i < r - 1; i++)
						for (int j = 1; j < c - 1; j++)
							field[i][j] = 0;

					x = 10; y = 0;
					game = true;
				}
		}
			if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; };
			if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; };
			if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; };
			if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; };
		if (!game) continue;
		
		for (int i = 0; i < fanCount; i++) {
			f[i].move();
		}

		if (timer > delay) {

			x += dx;
			y += dy;
			if (x < 0) x = 0;
			if (x > c - 1) x = c - 1;
			if (y < 0) y = 0;
			if (y > r - 1) y = r - 1;
			
			if (field[y][x] == 2) game = false;
			if (field[y][x] == 0) field[y][x] = 2;
			timer = 0;
		}
		if (field[y][x] == 1)
		{
			dx = dy = 0;

			for (int i = 0; i < fanCount; i++)
			drop(f[i].y / l, f[i].x / l);

			for (int i = 0; i < r; i++)
				for (int j = 0; j < c; j++)
					if (field[i][j] == -1) field[i][j] = 0;
					else field[i][j] = 1;
		}
		for (int i = 0; i < fanCount; i++)
			if (field[f[i].y / l][f[i].x / l] == 2) game = false;
		window.clear();
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				if (field[i][j] == 0) continue;
				if (field[i][j] == 1) tiles.setTextureRect(IntRect(0, 0, l, l));
				if (field[i][j] == 2) tiles.setTextureRect(IntRect(54, 0, l, l));
				tiles.setPosition(j*l, i*l);
				window.draw(tiles);
			}
		}

		tiles.setTextureRect(IntRect(36, 0, l, l));
		tiles.setPosition(x*l, y*l);
		window.draw(tiles);

		fan.rotate(10);
		for (int i = 0; i < fanCount; i++)
		{
			fan.setPosition(f[i].x, f[i].y);
			window.draw(fan);
		}
		window.draw(fan);
		if (!game) {
			window.draw(text);
			sound.play();
		}
		window.display();
	}





	return 0;
}

