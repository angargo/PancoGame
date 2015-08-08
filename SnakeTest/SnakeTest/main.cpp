#include <iostream>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace std;
using namespace sf;

const int window_width = 640;
const int window_height = 480;

const int cell_width = 20;
const int cell_height = 20;

const int initial_length = 3;

const int frames_per_second = 6;

const Time time_per_frame = seconds(1.f / frames_per_second);

const static vector<int> dx{1, 0, -1, 0};
const static vector<int> dy{0, 1, 0, -1};

Texture texture;

class Direction {
	public:
	const static int Right = 0;
	const static int Down = 1;
	const static int Left = 2;
	const static int Up = 3;

	static int Opposite(int dir) {
		return (dir + 2) % 4;
	}

	static Vector2i GetVec(int dir) {
		return Vector2i(dx[dir], dy[dir]);
	}
};

class Part {
	public:
		Part(int part_type, Vector2i position) : type(part_type), pos(position) {

		}

		void draw(RenderWindow& window) {
			RectangleShape shape(Vector2f(cell_width, cell_height));
			shape.setPosition(pos.x * cell_width, pos.y * cell_height);
			//shape.setFillColor(Color(200, 0, 0));
			shape.setTexture(&texture);
			shape.setTextureRect(IntRect(type * cell_width, 0, cell_width, cell_height));
			window.draw(shape);
		}

		int type;
		Vector2i pos;

		static const int Hor = 5;

		static int Head(int dir) {
			return (1 << dir);
		}

		static int Tail(int dir) {
			return (15 ^ (1 << dir));
		}

		static int NextPart(int ptype, int htype) {
			ptype <<= 2;
			if (ptype >= 16) ptype >>= 4;
			return ptype | htype;
		}

		static int NextTail(int ttype, int ptype) {
			ttype ^= 15;
			ttype <<= 2;
			if (ttype >= 16) ttype >>= 4;
			ptype ^= ttype;
			ptype ^= 15;
			return ptype;
		}
};

class Game {
	public:
		Game() : window(VideoMode(window_width, window_height), "SnakeTest") {
			width = window_width / cell_width;
			height = window_height / cell_height;

			if (not arial_font.loadFromFile("../arial.ttf")) {
				cout << "FAIL" << endl;
			}
			scoreboard.setFont(arial_font);
			scoreboard.setCharacterSize(20);
			scoreboard.setColor(Color(200, 0, 0));
			scoreboard.setStyle(Text::Bold);
			scoreboard.setPosition(20, 10);

			prize.setRadius(cell_width / 2);
			prize.setFillColor(Color(0, 0, 200));

			start();
		}

		void run() {
			Clock clock;
			Time time_since_last_update = Time::Zero;

			while(window.isOpen()) {
				processEvents();
				time_since_last_update += clock.restart();
				while (time_since_last_update > time_per_frame) {
					time_since_last_update -= time_per_frame;
					processEvents();
					update();
				}
				render();
			}
		}

		void start() {
			curr_dir = next_dir = Direction::Right;
			snake.clear();
			occupied = vector<vector<bool>>(width, vector<bool>(height));
			score = 0;

			int stx = width / 2 - initial_length, sty = height / 2;

			snake.push_back(Part(Part::Tail(Direction::Right), Vector2i(stx + 0, sty))); // Tail
			SetOccupied(snake.back().pos, true);

			snake.push_back(Part(Part::Hor, Vector2i(stx + 1, sty))); // Body
			SetOccupied(snake.back().pos, true);

			snake.push_back(Part(Part::Head(Direction::Right), Vector2i(stx + 2, sty))); // Head
			SetOccupied(snake.back().pos, true);

			for (int i = 1; i < initial_length; ++i) Advance();

			PlacePrize();
		}

		void PlacePrize() {
			do {
				prize_pos.x = rand() % width;
				prize_pos.y = rand() % height;
			} while (IsOccupied(prize_pos));
			prize.setPosition(prize_pos.x * cell_width, prize_pos.y * cell_height);
		}

		void handlePlayerInput(Keyboard::Key key) {
			if (key == Keyboard::Escape) window.close();
			else if (key == Keyboard::W or key == Keyboard::Up) next_dir = Direction::Up;
			else if (key == Keyboard::A or key == Keyboard::Left) next_dir = Direction::Left;
			else if (key == Keyboard::S or key == Keyboard::Right) next_dir = Direction::Right;
			else if (key == Keyboard::D or key == Keyboard::Down) next_dir = Direction::Down;
		}

		void processEvents() {
			Event event;
			while (window.pollEvent(event)) {
				switch (event.type) {
					case Event::KeyPressed:
						handlePlayerInput(event.key.code);
						break;
					case Event::Closed:
						window.close();
						break;
					default:
						break;
				}
			}
		}

		void module(int& p, int mod) {
			if (p >= mod) p -= mod;
			if (p < 0) p += mod;
		}

		void update() {
			if (next_dir != Direction::Opposite(curr_dir)) curr_dir = next_dir;

			Vector2i next_pos = snake.back().pos + Direction::GetVec(curr_dir);
			module(next_pos.x, width);
			module(next_pos.y, height);
			if (IsOccupied(next_pos)) start(); // Game Over
			else {
				Advance();
				if (next_pos == prize_pos) {
					++score;
					PlacePrize();
				}
				else Recoil();
			}
			scoreboard.setString(to_string(score));
		}

		void Advance() {
			Part p = snake.back();
			snake.pop_back();
			snake.push_back(Part(Part::NextPart(p.type, Part::Head(curr_dir)), p.pos));
			p.pos += Direction::GetVec(curr_dir);
			module(p.pos.x, width);
			module(p.pos.y, height);
			p.type = Part::Head(curr_dir);
			SetOccupied(p.pos, true);
			snake.push_back(p);
		}

		void Recoil() {
			SetOccupied(snake.front().pos, false);
			int tail_type = snake.front().type;
			snake.pop_front();

			Part p = snake.front();
			snake.pop_front();
			p.type = Part::NextTail(tail_type, p.type);
			snake.push_front(p);
		}

		bool IsOccupied(Vector2i pos) {
			return occupied[pos.x][pos.y];
		}

		void SetOccupied(Vector2i pos, bool state) {
			occupied[pos.x][pos.y] = state;
		}

		void render() {
			window.clear(Color(200, 200, 200));
			for (Part& part : snake) part.draw(window);
			window.draw(scoreboard);
			window.draw(prize);
			window.display();
		}

		RenderWindow window;
		int width, height, curr_dir, next_dir;
		deque<Part> snake;
		vector<vector<bool>> occupied;
		Font arial_font;
		Text scoreboard;
		int score;
		Vector2i prize_pos;
		CircleShape prize;
};

int main() {
	srand(time(0));
	texture.loadFromFile("../texture.png");
	Game().run();
}

