// Engine.cpp : Defines the entry point for the console application.
//My First Pong Game

#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <SFML/Audio.hpp>

using namespace std;


const::sf::Vector2f PONG_RESET(512, 256);
const::sf::Vector2f RED_RESET(24.0f, 256);
const::sf::Vector2f BLUE_RESET(1000.0f,256);
const int MAX_SCORE = 5;
const float RESET = 0.0f;

int player1 = 0;
int player2 = 0;
bool Play = true;


void PrintScore();
void ConstrainPaddles(sf::RectangleShape&);



int main()
{
	
	srand((time(0)));
	sf::Clock clock;
	sf::Vector2f velocity;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	bool Start = false;
	bool Quit = false;

	sf::RenderWindow window(sf::VideoMode(1024, 512), "Welcome to SDVA 203!");

	// pong
	sf::CircleShape shape(20.f);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(PONG_RESET);
	shape.setFillColor(sf::Color::Yellow);


	// red paddle
	sf::RectangleShape red(sf::Vector2f(20.0f, 100.0f));
	red.setPosition(RED_RESET);
	red.setFillColor(sf::Color::Red);
	red.setOrigin(red.getGlobalBounds().width / 2, red.getGlobalBounds().height / 2);
	red.setOutlineThickness(5);
	red.setOutlineColor(sf::Color::White);

	//blue paddle
	sf::RectangleShape blue(sf::Vector2f(20.0f, 100.0f));
	blue.setPosition(BLUE_RESET);
	blue.setFillColor(sf::Color::Blue);
	blue.setOrigin(blue.getGlobalBounds().width / 2, blue.getGlobalBounds().height / 2);
	blue.setOutlineThickness(5);
	blue.setOutlineColor(sf::Color::White);

	//MiddleLine
	sf::RectangleShape mLine(sf::Vector2f(5.0f, 520));
	mLine.setPosition(PONG_RESET);
	mLine.setFillColor(sf::Color::White);
	mLine.setOrigin(mLine.getGlobalBounds().width / 2, mLine.getGlobalBounds().height / 2);

	


	// SFML Text
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
	score1.setPosition(13, 10);
	score2.setString("Player 2 Score is: 0");
	score2.setCharacterSize(24);
	score2.setStyle(sf::Text::Bold);
	score2.setPosition(770, 10);
	score1.setColor(sf::Color::Red);
	score2.setColor(sf::Color::Blue);
	
	//Border
	sf::RectangleShape border1(sf::Vector2f(score1.getGlobalBounds().width + 5, score1.getGlobalBounds().height + 15));
	border1.setPosition(765, 10);
	border1.setFillColor(sf::Color::Black);
	border1.setOutlineThickness(5);
	border1.setOutlineColor(sf::Color::White);
	
	sf::RectangleShape border2(sf::Vector2f(score1.getGlobalBounds().width + 5, score1.getGlobalBounds().height + 15));
	border2.setPosition(10, 10);
	border2.setFillColor(sf::Color::Black);
	border2.setOutlineThickness(5);
	border2.setOutlineColor(sf::Color::White);
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	ostringstream WinnerStr;
	sf::Text winner;
	winner.setFont(font);
	winner.setCharacterSize(100);
	winner.setStyle(sf::Text::Bold);
	winner.setString("");
	winner.setPosition(512, 256);
	
	sf::Text PlayAgain;
	PlayAgain.setFont(font);
	PlayAgain.setCharacterSize(50);
	PlayAgain.setStyle(sf::Text::Bold);
	PlayAgain.setString(" ");
	PlayAgain.setPosition(512, 180);

	//Background Music
	sf::Music backgroundMusic;
	backgroundMusic.openFromFile("Audio/BackgroundMusic01.wav");
	backgroundMusic.setVolume(50);
	backgroundMusic.play();

	sf::Music victoryMusic;
	victoryMusic.openFromFile("Audio/VictoryFanfare.wav");
	


	//Score Sound
	sf::SoundBuffer scoreBuffer;
	if (!scoreBuffer.loadFromFile("Audio/ScoreSound.wav"))
	{
		cout << "ScoreSound audio was not loaded" << endl;
		//return 0; 
	}
	sf::Sound scoreSound;
	scoreSound.setBuffer(scoreBuffer);

	//PaddleBounce Sound
	sf::SoundBuffer bounceBuffer;
	if (!bounceBuffer.loadFromFile("Audio/Bounce.wav"))
	{
		cout << "Bounce audio was not loaded" << endl;
		//return 0;
	}
	sf::Sound bounceSound;
	bounceSound.setBuffer(bounceBuffer);
	
	//WallBounceSound
	sf::SoundBuffer wallBounceBuffer;
	if (!wallBounceBuffer.loadFromFile("Audio/WallBounceSound.wav"))
	{
		cout << "WallBounceSound audio was not loaded" << endl;
		//return 0;
	}
	sf::Sound wallBounceSound;
	wallBounceSound.setBuffer(wallBounceBuffer);


	
	

	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || Quit == true )
				window.close();

			//TODO create a start menu
			//TODO ask player if they want multiplayer or AI
			//TODO if AI ask for difficulty


			if (Play != true) // check if player wants to play again
			{
				
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Y)
					{
						Play = true;
						player1 = 0;
						score1.setString("Player 1 Score is: 0 ");
						player2 = 0;
						score2.setString("Player 2 Score is: 0");
						WinnerStr.str("");
						WinnerStr.clear();
						PlayAgain.setString(" ");
						winner.setString(WinnerStr.str());
						victoryMusic.stop();
						backgroundMusic.play();
						
						
					}
					else if (event.key.code == sf::Keyboard::N)
					{
						Quit = true;
					}
				}

			}
			else
			{
				if (event.type == sf::Event::KeyPressed) // begins the game - moves pong
				{
					if (event.key.code == sf::Keyboard::W || sf::Keyboard::S || sf::Keyboard::Up || sf::Keyboard::Down)
					{
						if (Start == false)
						{
							Start = true;
							
							int x;
							x = rand() % 2 + 1;
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

			}
			
		}


		bool player1MovingUp = false;
		bool player1MovingDown = false;
		bool player2MovingUp = false;
		bool player2MovingDown = false;


		if (Play == true || Start == true)
		{
			// red move up
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player1MovingUp = true;
				red.move(0, -350 * dt.asSeconds());
				//constrain
				if (red.getPosition().y <   red.getGlobalBounds().height / 2)
				{
					red.setPosition(red.getPosition().x,  red.getGlobalBounds().height / 2);
				}
			}

			// red move down
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				player1MovingDown = true;
				red.move(0, 350 * dt.asSeconds());
				//constrain
				if (red.getGlobalBounds().height + red.getPosition().y >= 512 + red.getGlobalBounds().height / 2)
				{
					red.setPosition(red.getPosition().x, 512 - red.getGlobalBounds().height / 2);
				}
			}






			//TODO Set and check difficulty level
			//TODO change AI y velocity accordingly
			//AI
			bool moveUP = false;
			if (shape.getPosition().y <  blue.getPosition().y )
			{
				moveUP = true;
			}

			if (moveUP == true)
			{
				player2MovingUp = true;
				blue.move(0, -350 * dt.asSeconds());
				//constrain
				if (blue.getPosition().y < blue.getGlobalBounds().height / 2)
				{
					blue.setPosition(blue.getPosition().x, blue.getGlobalBounds().height / 2);
				}
			}

			else
			{
				player2MovingDown = true;
				blue.move(0, 350 * dt.asSeconds());
				//constrain
				if (blue.getGlobalBounds().height + blue.getPosition().y >= 512 + blue.getGlobalBounds().height / 2)
				{
					blue.setPosition(blue.getPosition().x, 512 - blue.getGlobalBounds().height / 2);

				}
			}
			

			/*
			// blue move up(player 2 controlled)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				player2MovingUp = true;
				blue.move(0, -350 * dt.asSeconds());
				if (blue.getPosition().y < blue.getGlobalBounds().height / 2)
				{
					blue.setPosition(blue.getPosition().x, blue.getGlobalBounds().height / 2);
				}
			}

			//blue move down(player 2 controlled
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				player2MovingDown = true;
				blue.move(0, 350 * dt.asSeconds());
				if (blue.getGlobalBounds().height + blue.getPosition().y >= 512 + blue.getGlobalBounds().height / 2)
				{
					blue.setPosition(blue.getPosition().x, 512 - blue.getGlobalBounds().height / 2);

				}

			}*/
		}
		//cout << "x is: " << blue.getPosition().x << "y is : " << blue.getPosition().y << endl;
		


		sf::Vector2f pongPos = shape.getPosition();
		float radius = shape.getRadius();
		float power = 1.0f;
		power = abs(power);
		power = 1 - power;
		float redMidY = red.getPosition().y + red.getSize().y * 0.5f;
		float blueMidY = blue.getPosition().y + blue.getSize().y * 0.5f;
		

		
		//red paddle collision
		if (pongPos.x + radius > red.getPosition().x &&
			pongPos.x - radius < red.getPosition().x + red.getSize().x &&
			pongPos.y + radius > red.getPosition().y &&
			pongPos.y + radius < red.getPosition().y + red.getSize().y)
		{
			float ballDistance = pongPos.y - redMidY;
			power = ballDistance / (red.getSize().y * 0.5f);
			bounceSound.play();
			shape.setPosition(red.getPosition().x + red.getSize().x + radius, pongPos.y);
			velocity.x *= -1.25;
			

			if (player1MovingUp == true)
			{
				velocity.y = -100 * power;
				
				 
			}
			else if (player1MovingDown == true)
			{
				velocity.y = 100 * power;
				
				
			}

			
		}
		
		//blue paddle collision
		if (pongPos.x + radius > blue.getPosition().x &&
			pongPos.x - radius < blue.getPosition().x + blue.getSize().x &&
			pongPos.y + radius > blue.getPosition().y &&
			pongPos.y + radius < blue.getPosition().y + blue.getSize().y)
		{	
			float ballDistance = pongPos.y - blueMidY;
			power = ballDistance / (blue.getSize().y * 0.5f);
			bounceSound.play();
			shape.setPosition(blue.getPosition().x - radius, pongPos.y);
			velocity.x *= -1.25;
			if (player2MovingUp)
			{
				velocity.y = -100;			
				
			}
			else if (player2MovingDown)
			{
				velocity.y = 100;	
				
			}

		
		}


		//cout << velocity.x << endl;
		
		//bounce top wall
		if (shape.getPosition().y <= shape.getRadius())
		{
			wallBounceSound.play();
			velocity.y *= -0.9f;
			velocity.x *= 0.9f;

			shape.setPosition(shape.getPosition().x, shape.getRadius());
			
			
		}

		//bounce bottom wall
		if (shape.getRadius() + shape.getPosition().y >= 512)
		{
			wallBounceSound.play();
			velocity.y *= -0.9f;
			velocity.x *= 0.9f;
			
			shape.setPosition(shape.getPosition().x, 512 - shape.getRadius());
		}

		// left score
		if (shape.getPosition().x < shape.getRadius())
		{
			scoreSound.play();
			shape.setPosition(PONG_RESET);
			velocity.x = RESET; //0
			velocity.y = RESET; //0
			red.setPosition(RED_RESET);
			blue.setPosition(BLUE_RESET);	
			player2++;
			PrintScore();
			Start = false;

			
			ostringstream Score2;
			Score2 << "Player 2 Score is: " << player2;
			score2.setString(Score2.str());
			if (player2 >= MAX_SCORE)
			{
				WinnerStr << "Player 2 Wins" << endl;
				winner.setString(WinnerStr.str());
				winner.setOrigin(winner.getGlobalBounds().width / 2.0f, 0);
				winner.setColor(sf::Color::Blue);
				victoryMusic.play();
			}
			
			


		}

		//  right score
		if (shape.getRadius() + shape.getPosition().x > 1024)
		{
			scoreSound.play();
			shape.setPosition(PONG_RESET);
			velocity.x = RESET;
			velocity.y = RESET;
			red.setPosition(RED_RESET);
			blue.setPosition(BLUE_RESET);
			player1++;
			PrintScore();
			Start = false;

			ostringstream Score1;
			Score1 << "Player 1 Score is: " << player1;
			score1.setString(Score1.str());
			if (player1 >= MAX_SCORE)
			{
				WinnerStr << "Player 1 Wins" << endl;
				winner.setString(WinnerStr.str());
				winner.setOrigin(winner.getGlobalBounds().width / 2.0f, 0);
				winner.setColor(sf::Color::Red);
				victoryMusic.play();
				
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


		
		
		if (Start == true)
		{
			shape.move(velocity.x * dt.asSeconds(), velocity.y * dt.asSeconds());
			
		}
		else 	if (player1 >= MAX_SCORE || player2 >= MAX_SCORE)
		{

			backgroundMusic.stop();
			
			Play = false;
			PlayAgain.setString("Do you want to play again? Y/N? ");
			PlayAgain.setOrigin(PlayAgain.getGlobalBounds().width / 2, PlayAgain.getGlobalBounds().height / 2);
		}
		
	

		window.clear();
		window.draw(border1);
		window.draw(border2);
		window.draw(mLine);
		window.draw(shape);
		window.draw(red);
		window.draw(blue);
		window.draw(score1);
		window.draw(score2);
		window.draw(winner);
		window.draw(PlayAgain);
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

