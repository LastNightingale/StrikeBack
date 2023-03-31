#include <SFML/Graphics.hpp>
#include <iostream> 
#include <string> 
#include <vector>
#include <ctime>
#define color_amount 5
#define skin_amount 3

using namespace sf;
using namespace std;

Texture player_skin0, player_skin1, player_skin2;
Texture *player_skins[3] = { &player_skin0, &player_skin1, &player_skin2 };

Color inner_colors[5] = {Color::Yellow, Color(75,0,130),Color(34,139,34),Color(255,218,185), Color(128,0,0)};
Font font;

sf::Keyboard::Key keys[5] = { sf::Keyboard::Key::Num1 , sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3,
sf::Keyboard::Key::Num4, sf::Keyboard::Key::Num5 };

class Bar
{	
public:
	RectangleShape bar;
	Bar() = default;
	Bar(Vector2f pos, Vector2f size, Color color)
	{
		bar.setFillColor(color);
		bar.setPosition(pos);
		bar.setSize(size);
	}
	Bar(Vector2f pos, Vector2f size, Color color, Color outline)
	{
		bar.setFillColor(color);
		bar.setPosition(pos);
		bar.setSize(size);
		bar.setOutlineColor(outline);
		bar.setOutlineThickness(4);
	}
	RectangleShape getBar()
	{
		return bar;
	}
};

class Block
{
	RectangleShape innerBlock, outerBlock;
	int number;
public:
	bool isActive;
	Block(Vector2f position, bool isActive, Color innercolor, int number)
	{
		outerBlock.setSize(Vector2f(100, 100));
		outerBlock.setOrigin(50, 50);
		outerBlock.setPosition(position);
		outerBlock.setOutlineColor(Color::White);
		outerBlock.setOutlineThickness(3);
		outerBlock.setFillColor(isActive ? Color(70, 70, 70) : Color::Black);
		innerBlock.setSize(Vector2f(50, 50));
		innerBlock.setOrigin(25, 26);
		innerBlock.setPosition(position);
		innerBlock.setFillColor(innercolor);
		this->number = number;
		this->isActive = isActive;
	}
	RectangleShape getInnerBlock()
	{
		return innerBlock;
	}
	RectangleShape getOuterBlock()
	{
		return outerBlock;
	}
	int getNumber()
	{
		return number;
	}
};

class BackStrike
{
private:
	VertexArray backstrike;	
public:
	float time;
	BackStrike(RectangleShape object1, RectangleShape object2)
	{
		time = 0;
		backstrike = VertexArray(Lines, 2);
		backstrike[0].position = object1.getPosition();
		backstrike[0].color = Color::Blue;
		backstrike[1].position = object2.getPosition();
		backstrike[1].color = Color::Cyan;
	}
	VertexArray getStrikeback()
	{
		return backstrike;
	}
};

class Bullet
{
	RectangleShape projectile;
	Color color;
public:
	Bullet(Vector2f position, Color color)
	{
		projectile.setPosition(position);
		projectile.setFillColor(color);
		projectile.setSize(Vector2f(10, 10));
		projectile.setOrigin(5, 5);
		this->color = color;
	}
	RectangleShape getProjectile()
	{
		return projectile;
	}
	Color getColor()
	{
		return color;
	}
	void Update(float dt)
	{
		projectile.move(1000 * dt, 0);
	}
};

class Enemy
{
private:
	RectangleShape enemy;
	Color color;
public:
	bool isDestroyed;
	Enemy(Vector2f position, Color color)
	{		
		isDestroyed = false;
		enemy.setPosition(position);
		enemy.setFillColor(color);
		enemy.setSize(Vector2f(75, 75));
		enemy.setOrigin(37.5, 37.5);
		this->color = color;
	}
	RectangleShape getEnemy()
	{
		return enemy;
	}
	Color getColor()
	{
		return color;
	}
	void Update(float dt)
	{
		enemy.move(-100 * dt, 0);
	}
};

class Player
{
private:
	sf::RectangleShape player;
public:
	int health, bullets;
	Player()
	{
		health = 200;
		bullets = 10;
		player.setSize(sf::Vector2f(75, 75));
		player.setOrigin(37.5, 37.5);
		player.setPosition(sf::Vector2f(280, 450));
		player.setFillColor(sf::Color::Red);
	}
	Player(signed current_skin)
	{
		health = 200;
		bullets = 10;
		player.setSize(sf::Vector2f(75, 75));
		player.setOrigin(37.5, 37.5);
		player.setPosition(sf::Vector2f(280, 450));
		player.setFillColor(sf::Color::Blue);
		player.setTexture(player_skins[current_skin]);
	}
	RectangleShape getPlayer()
	{
		return player;
	}
	void Update(float dt)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) 
			player.move(0, -100 * dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			player.move(0, 100 * dt);
	}
};

class TextButton
{
private:
	Text textbutton;
public:
	TextButton() = default;
	TextButton(string text, Vector2f pos)
	{
		textbutton = Text(text, font);
		textbutton.setStyle(sf::Text::Bold);
		textbutton.setPosition(pos);
	}
	Text getText()
	{
		return textbutton;
	}
	void SetBig(Vector2f origin, int size, Color color)
	{
		textbutton.setCharacterSize(size);
		textbutton.setOrigin(origin);
		textbutton.setFillColor(color);
	}
	void SetSmall(Vector2f origin, int size, Color color)
	{
		textbutton.setCharacterSize(size);
		textbutton.setOrigin(origin);
		textbutton.setFillColor(color);
	}
};

class StartMenu
{
private:
	TextButton startgame, bestscores, exitgame;
	Vector2i cursor;
	bool start, scores, exit;
public:
	StartMenu()
	{
		start = scores = exit = true;
		startgame = TextButton("Start Game", Vector2f(700, 300));
		bestscores = TextButton("Best Scores", Vector2f(700, 450));
		exitgame = TextButton("Exit Game", Vector2f(700, 600));
	}
	bool StartGameUpdate(RenderWindow* window)
	{		
		cursor = sf::Mouse::getPosition(*window);
		if (startgame.getText().getGlobalBounds().contains(cursor.x, cursor.y))
		{
			startgame.SetBig(Vector2f(186.5, 31), 72, Color::Green);
			if (start)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return 1;
			}
			else
			{
				start = !Mouse::isButtonPressed(sf::Mouse::Button::Left);
			}				
		}
		else
		{
			startgame.SetSmall(Vector2f(168, 29), 64, Color::White);
			start = !Mouse::isButtonPressed(sf::Mouse::Button::Left);
			return 0;
		}		
	}
	bool BestScoresUpdate(RenderWindow* window)
	{
		cursor = sf::Mouse::getPosition(*window);
		if (bestscores.getText().getGlobalBounds().contains(cursor.x, cursor.y))
		{
			bestscores.SetBig(Vector2f(186.5, 31), 72, Color::Yellow);
			if (scores)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return 1;
			}
			else
			{
				scores = !Mouse::isButtonPressed(sf::Mouse::Button::Left);
			}
		}
		else
		{
			bestscores.SetSmall(Vector2f(168, 29), 64, Color::White);
			scores = !Mouse::isButtonPressed(sf::Mouse::Button::Left);
			return 0;
		}
	}
	bool ExitGameUpdate(RenderWindow* window)
	{
		cursor = sf::Mouse::getPosition(*window);
		if (exitgame.getText().getGlobalBounds().contains(cursor.x, cursor.y))
		{
			exitgame.SetBig(Vector2f(161, 31), 72, Color::Red);
			if (exit)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return 1;
			}
			else
			{
				exit = !Mouse::isButtonPressed(sf::Mouse::Button::Left);
			}
		}
		else
		{
			exitgame.SetSmall(Vector2f(141, 29), 64, Color::White);
			exit = !Mouse::isButtonPressed(sf::Mouse::Button::Left);
			return 0;
		}
	}
	void StartMenuDraw(RenderWindow *window)
	{
		(*window).clear();
		(*window).draw(startgame.getText());
		(*window).draw(bestscores.getText());
		(*window).draw(exitgame.getText());
		(*window).display();
	}
};

class Game
{
private:
	sf::RenderWindow window{ sf::VideoMode(1400,900), "Strike back", sf::Style::Default };
	Player player;	
	Bar bulletback, healthbar, healthbarback;
	void (Game::* update)();
	void (Game::* draw)();
	StartMenu startmenu;
	float dt, current_time;	
	int spawn_time, chosen;
	int spawn_time, chosen;
	bool able_to_fire, LKMpressed;
	signed char current, current_skin;
	vector<Bar> bullet;
	vector<Block> color_choice;
	vector<Bullet> bullets;
	vector<Enemy> enemies;
	vector<BackStrike> strikebacks;
public:
	Game()
	{	
		healthbar = Bar(Vector2f(800, 25), Vector2f(player.health, 50), Color::Red);
		healthbarback = Bar(Vector2f(800, 25), Vector2f(200, 50), Color::Black, Color::White);
		bulletback = Bar(Vector2f(400, 25), Vector2f(200, 50), Color::Black, Color::White);
		LKMpressed = false;
		update = &Game::StartMenuUpdate;
		draw = &Game::StartMenuDraw;
		current_time = 0;
		spawn_time = 1;
		window.setFramerateLimit(60);
		able_to_fire = true;
		current_skin = current = 2;
		player = Player(current_skin);
	}
	float Randomizer()
	{
		int x = -1;
		while (x < 112.5 || x > 787.5)
		{
			x = rand() % int(player.getPlayer().getPosition().y + 100) +
			int(player.getPlayer().getPosition().y - 100);
		}
		return x;
	}
	void BlockUpdate()
	{		
		for (int i = 0; i < color_amount; ++i)
		{
			color_choice.push_back(Block(Vector2f(140, (900 - color_amount * 100 - (color_amount - 1) * 50) / 2 + 50 + 150 * i),
				i == current ? true : false, inner_colors[i], i));
		} 
		for (auto iter : color_choice)
		{
			if (!iter.isActive && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				sf::Vector2i cursor = sf::Mouse::getPosition(window);
				if (iter.getOuterBlock().getGlobalBounds().contains(cursor.x, cursor.y))
				{
					current = iter.getNumber();
				}
			}			
		}
	}
	void StartMenuUpdate()
	{		
		if (startmenu.StartGameUpdate(&window))
		{
			update = &Game::GameUpdate;
			draw = &Game::GameDraw;
		}
		if (startmenu.BestScoresUpdate(&window))
		{
			
		}
		if (startmenu.ExitGameUpdate(&window))
		{
			window.close();
		}
	}
	void StartMenuDraw()
	{
		startmenu.StartMenuDraw(&window);
	}
	void GameUpdate()
	{			
		current_time += dt;
		if (current_time >= spawn_time)
		{
			if (spawn_time < 10) spawn_time += 5;
			else if (spawn_time <= 25) spawn_time += 4;
			else spawn_time += 3;
			enemies.push_back(Enemy(Vector2f(1400, Randomizer()), inner_colors[rand() % 5]));
			//enemies.push_back(Enemy(Vector2f(1400, rand() % int(player.getPlayer().getPosition().y + 100) +
				//int(player.getPlayer().getPosition().y - 100)), inner_colors[rand() % 5]));
		}
		for (int i = 0; i < bullets.size(); ++i)
		{
			bullets[i].Update(dt);
			if (bullets[i].getProjectile().getPosition().x > 1425)
			{
				bullets[i] = bullets.back();
				bullets.pop_back();
			}
		}
		for (int i = 0; i < enemies.size(); ++i)
		{
			enemies[i].Update(dt);
			if (enemies[i].getEnemy().getPosition().x > 1425 || enemies[i].getEnemy().getPosition().x <= 280)
			{
				enemies[i] = enemies.back();
				enemies.pop_back();
			}
		}
		for (int i = 0; i < player.bullets; ++i)
		{
			bullet.push_back(Bar(Vector2f(400 + ((2 * (i+1) - 1) * 5) + ((i)*10), 30), Vector2f(10, 40), Color::White));
		}
		for (int i = 0; i < bullets.size(); ++i)
		{
			for (int j = 0; j < enemies.size(); ++j)
			{
				if (bullets[i].getProjectile().getGlobalBounds().intersects(enemies[j].getEnemy().getGlobalBounds()))
				{
					if (bullets[i].getColor() != enemies[j].getColor())
					{
						strikebacks.push_back(BackStrike(enemies[j].getEnemy(), player.getPlayer()));
						player.health -= 20;
						healthbar.bar.setSize(Vector2f(player.health, 50));
					}
					bullets[i] = bullets.back();
					bullets.pop_back();
					enemies[j] = enemies.back();
					enemies.pop_back();					
				}
			}
		}
		for (int i = 0; i < strikebacks.size(); ++i)
		{
			strikebacks[i].time += dt;
			if (strikebacks[i].time > 0.25)
			{
				strikebacks[i] = strikebacks.back();
				strikebacks.pop_back();
			}
		}
		player.Update(dt);
		BlockUpdate();
	}
	void GameDraw()
	{
		window.clear(sf::Color::Black);		
		for (auto iter : color_choice)
		{
			window.draw(iter.getOuterBlock());
			window.draw(iter.getInnerBlock());			
		}
		for (auto& iter : bullets)
		{
			window.draw(iter.getProjectile());
		}
		for (auto& iter : strikebacks)
		{
			window.draw(iter.getStrikeback());
		}
		for (auto& iter : enemies)
		{
			window.draw(iter.getEnemy());
		}
		window.draw(player.getPlayer());
		window.draw(healthbarback.getBar());
		window.draw(healthbar.getBar());
		window.draw(bulletback.getBar());
		for (auto& iter : bullet)
		{
			window.draw(iter.bar);
		}
		bullet.clear();
		color_choice.clear();
		window.display();
	}
	void Run()
	{
		srand(time(NULL));
		Clock clock;
		while (window.isOpen())
		{
			dt = clock.getElapsedTime().asSeconds();
			clock.restart();			
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::EventType::Closed) window.close();
				if (event.type == sf::Event::EventType::MouseButtonReleased)
					if (event.key.code == sf::Mouse::Button::Left) LKMpressed = false;
				if (event.type == sf::Event::EventType::MouseButtonPressed)
					if (event.key.code == sf::Mouse::Button::Left) LKMpressed = true;
				if (event.type == sf::Event::EventType::KeyReleased)
				{					
					if (event.key.code == sf::Keyboard::Key::Space)
					{
						if (able_to_fire && player.bullets > 0)
						{
							player.bullets -= 1;
							cout << player.bullets << endl;
							bullets.push_back(Bullet(player.getPlayer().getPosition(), inner_colors[current]));
						}
					}
				}					
				if (event.type == sf::Event::EventType::KeyPressed)
				{
					for (int i = 0; i < 5; ++i)
					{
						if (event.key.code == keys[i])
						{
							current = i;
						}
					}
					
				}
			}
			(this->*update)();
			(this->*draw)();
			/*GameUpdate();
			GameDraw();*/			
		}
	}
};

int main()
{
	player_skin0.loadFromFile("textures/PlayerSkin0.png");
	player_skin1.loadFromFile("textures/PlayerSkin1.png");
	player_skin2.loadFromFile("textures/PlayerSkin2.png");
	font.loadFromFile("textures/Benae.ttf");
	Game game;
	game.Run();
}