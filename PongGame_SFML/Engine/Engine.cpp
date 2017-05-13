// Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <sstream>
#include <ctime>

using namespace std;


const::sf::Vector2f PONG_RESET(512, 256);
const::sf::Vector2f RED_RESET(10.0f, 256.0f);
const::sf::Vector2f BLUE_RESET(990.0f, 256.0f);
const float RESET = 0.0f;

int player1 = 0;
int player2 = 0;
void PrintScore();



int main()
{
	
	srand((time(0)));






	sf::Clock clock;
	sf::Vector2f velocity;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	bool Play = false;

	sf::RenderWindow window(sf::VideoMode(1024, 512), "Welcome to SDVA 203!");

	// pong
	sf::CircleShape shape(10.f);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(PONG_RESET);
	shape.setFillColor(sf::Color::White);

	// red paddle
	sf::RectangleShape red(sf::Vector2f(20.0f, 100.0f));
	red.setPosition(RED_RESET);
	red.setFillColor(sf::Color::Red);

	//blue paddle
	sf::RectangleShape blue(sf::Vector2f(20.0f, 100.0f));
	blue.setPosition(BLUE_RESET);
	blue.setFillColor(sf::Color::Blue);


	// Print Score
	sf::Font font;
	if (!font.loadFromFile("Content/arial.ttf"))
	{
		cout << "font not loaded" << endl;
	}
	sf::Text score1;
	sf::Text score2;
	score1.setFont(font);
	score2.setFont(font);
	score1.setString("Player 1 Score is: 0 ");
	score1.setCharacterSize(24);
	score1.setStyle(sf::Text::Bold);
	score1.setPosition(10, 10);
	score2.setString("Player 2 Score is: 0");
	score2.setCharacterSize(24);
	score2.setStyle(sf::Text::Bold);
	score2.setPosition(750, 10);

	ostringstream Winner;
	sf::Text winner;
	winner.setFont(font);
	winner.setCharacterSize(24);
	winner.setStyle(sf::Text::Bold);
	winner.setPosition(512, 256);
	
	


	
	

	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::W || sf::Keyboard::S || sf::Keyboard::Up || sf::Keyboard::Down)
				{	
					if (Play == false)
					{
						Play = true;
						int x;
						x = rand() %  2 + 1 ;
						switch (x)
						{
						case 1: 
							velocity.x = 200;
							//velocity.y = rand() % -100 + 101;
							break;
						case 2:
							velocity.x = -200;
							//velocity.y = rand() % 100 + -201;
							break;
						default:
							velocity.x = 200;
							//velocity.y = rand() % 0 + 201;

						}

					}
				}
			}
			cout << velocity.y << endl;
			
		}

		// red move up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			red.move(0, -350 * dt.asSeconds());
			if (red.getPosition().y < 0)
			{
				red.setPosition(red.getPosition().x, 0);
			}
		}

		// red move down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			red.move(0, 350 * dt.asSeconds());
			if (red.getSize().y + red.getPosition().y >= 512)
			{
				red.setPosition( red.getPosition().x,  512- red.getSize().y );
			}
		}







		// blue move up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			blue.move(0, -350 * dt.asSeconds());
			if (blue.getPosition().y < 0)
			{
				blue.setPosition(blue.getPosition().x, 0);
			}
		}
		
		//blue move down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			blue.move(0, 350 * dt.asSeconds());
			if (blue.getSize().y + blue.getPosition().y > 512)
			{
				blue.setPosition(blue.getPosition().x, 512 - blue.getSize().y);
				
			}
		}

		//cout << "x is: " << blue.getPosition().x << "y is : " << blue.getPosition().y << endl;
		
		sf::Vector2f pongPos = shape.getPosition();
		float radius = shape.getRadius();
		//red paddle collision
		if (pongPos.x + radius > red.getPosition().x &&
			pongPos.x - radius < red.getPosition().x + red.getSize().x &&
			pongPos.y + radius > red.getPosition().y &&
			pongPos.y + radius < red.getPosition().y + red.getSize().y)
		{
			shape.setPosition(red.getPosition().x + red.getSize().x + radius, pongPos.y);
			velocity.x *= -1.15;
			if (red.getPosition().y < 256)
			{
				velocity.y += -100;
				velocity.y *= -1; 
				
			}
			else if (red.getPosition().y > 256)
			{
				velocity.y += 100;
				velocity.y *= -1;
				
			}

			
		}
		
		//blue paddle collision
		if (pongPos.x + radius > blue.getPosition().x &&
			pongPos.x - radius < blue.getPosition().x + blue.getSize().x &&
			pongPos.y + radius > blue.getPosition().y &&
			pongPos.y + radius < blue.getPosition().y + blue.getSize().y)
		{	
			shape.setPosition(blue.getPosition().x - radius, pongPos.y);
			velocity.x *= -1.15;
			if (blue.getPosition().y < 256)
			{
				velocity.y += -100;
				velocity.y *= -1;
				
			}
			else if (blue.getPosition().y > 256)
			{
				velocity.y += 100;
				velocity.y *= -1;
				
			}

		
		}


		//cout << velocity.x << endl;
		
		//bounce top wall
		if (shape.getPosition().y <= shape.getRadius())
		{
			velocity.y *= -1;
			
		}

		//bounce bottom wall
		if (shape.getRadius() + shape.getPosition().y >= 512)
		{
			velocity.y *= -1;
			shape.setPosition(shape.getPosition().x, 512 - shape.getRadius());
		}

		// left score
		if (shape.getPosition().x < shape.getRadius())
		{
			shape.setPosition(PONG_RESET);
			velocity.x = RESET;
			velocity.y = RESET;
			red.setPosition(RED_RESET);
			blue.setPosition(BLUE_RESET);	
			player2++;
			PrintScore();
			Play = false;

			
			ostringstream Score2;
			Score2 << "Player 2 Score is: " << player2;
			score2.setString(Score2.str());
			if (player2 >= 5)
			{
				Winner << "Player 2 Wins" << endl;
				winner.setString(Winner.str());
			}
			
			


		}

		//  right score
		if (shape.getRadius() + shape.getPosition().x > 1024)
		{
			shape.setPosition(PONG_RESET);
			velocity.x = RESET;
			velocity.y = RESET;
			red.setPosition(RED_RESET);
			blue.setPosition(BLUE_RESET);
			player1++;
			PrintScore();
			Play = false;

			ostringstream Score1;
			Score1 << "Player 1 Score is: " << player1;
			score1.setString(Score1.str());
			if (player1 >= 5)
			{
				Winner << "Player 1 Wins" << endl;
				winner.setString(Winner.str());
			}
			
			
		}


		// Left and Right wall bounce
		/*
		//bounce left wall
		if (shape.getPosition().x <= shape.getRadius())
		{
			velocity.x *= -1;
			shape.setPosition(shape.getRadius(), shape.getPosition().y);
		}

		// bounce right wall
		if (shape.getRadius() + shape.getPosition().x >= 1024)
		{
			velocity.x *= -1;
			shape.setPosition(1024 - shape.getRadius(), shape.getPosition().y);
		}
		*/


		
		
		if (Play == true)
		{
			shape.move(velocity.x * dt.asSeconds(), velocity.y * dt.asSeconds());
			
		}
		else if (player1 >= 5)
		{
		
			//TODO reset score
			//TODO stop game
			//TODO ask player if they want to go again
		
			
			
		}
		else if (player2 >= 5)
		{
			
			//TODO reset score
			//TODO stop game
			//TODO ask player if they want to go again
			
			
		}

		window.clear();
		window.draw(shape);
		window.draw(red);
		window.draw(blue);
		window.draw(score1);
		window.draw(score2);
		window.draw(winner);
		window.display();
		
	}

	
	return 0;
}

void PrintScore()
{
	cout << "Player 1 score is: " << player1 << endl;
	cout << endl;
	cout << "Player 2 score is: " << player2 << endl;

}
