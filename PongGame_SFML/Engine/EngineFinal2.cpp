//******************************************************************
// Engine.cpp : Defines the entry point for the console application.
//My First Pong Game
//******************************************************************


//Known Problems:
/*
when setting origin for paddles, collision detection does not work properly otherwise works fine
Code is too still too cluttery - need to factorize further
when putting collision detection as a function that returns/takes in a single sf::RectangleShape* paddles argument/parameter does not work

*/


#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <SFML/Audio.hpp>
#include <string>

using namespace std;

// ****************************
// *****Global Variables*******
// ****************************
const::sf::Vector2f PONG_RESET(512, 256);
const::sf::Vector2f RED_RESET(24.0f, 256);
const::sf::Vector2f BLUE_RESET(985.0f,256);
const int MAX_SCORE = 5;
const float RESET = 0.0f;

int player1 = 0;
int player2 = 0;
bool Play = true;
bool Start = false;
bool Begin = false;
bool Quit = false;
bool Easy = false;
bool Medium = false;
bool Hard = false;
bool Hell = false;
bool diffChosen = false;
bool AIChosen = false;
bool player2Chosen = false;



sf::Music* backgroundMusic;
sf::Music* victoryMusic;
sf::SoundBuffer* wallBounceBuffer;
sf::SoundBuffer* scoreBuffer;
sf::SoundBuffer* bounceBuffer;
sf::Sound* bounceSound;
sf::Sound* wallBounceSound;
sf::Sound* scoreSound;
sf::Vector2f velocity;
sf::Vector2f b1Velocity;
sf::Vector2f b2Velocity;
sf::Vector2f b3Velocity;
sf::Vector2f b4Velocity;


sf::CircleShape shape(20.f); // pong
sf::RectangleShape red(sf::Vector2f(20.0f, 100.0f)); // red paddle (player1)
sf::RectangleShape blue(sf::Vector2f(20.0f, 100.0f)); //blue paddle (player2)
sf::RectangleShape mLine(sf::Vector2f(5.0f, 520)); //MiddleLine
sf::RectangleShape block1(sf::Vector2f(20.0f, 60.0f)); // obstacle
sf::RectangleShape block2(sf::Vector2f(20.0f, 60.0f)); // obstacle
sf::RectangleShape block3(sf::Vector2f(20.0f, 60.0f)); // obstacle
sf::RectangleShape block4(sf::Vector2f(20.0f, 60.0f)); // obstacle

ostringstream reusableStr;
ostringstream WinnerStr;
sf::Text winner;
sf::Font font;
sf::Text score1;
sf::Text score2;
sf::Text PlayAgain;
sf::Text WelcomeText;
sf::RectangleShape border1; // Text Border(for player1)
sf::RectangleShape border2; // Text Border(for player2)

// ****************************
// ****FUNCTION PROTOTYPES*****
// ****************************

bool AI(bool, bool, int* , sf::Time*);
bool InitSounds();
bool InitText();
bool leftScore(); 
bool rightScore(); 
void Init();
void wallBounce();
void RandomizeBlocks();
void blockBounce();


// passed in as pointers
void Update(sf::RenderWindow*, sf::Time*); 
void Render(sf::RenderWindow* ); 
void ConstrainPaddles(sf::RectangleShape*); 
void CollisionPaddles(sf::RectangleShape*, sf::RectangleShape*, bool,bool,bool,bool); //needs refactoring
void blockCollision(sf::RectangleShape*);
void menu(sf::Event*, sf::RenderWindow*);






int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 512), "Welcome to SDVA 203!");
	srand((time(0)));
	Init();
	sf::Clock clock;

	
	
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		
		Update(&window, &dt);
		Render(&window);
		
		
	}
	
	return 0;
}



// ****************************
// ***FUNCTION DEFINITION*****
// ****************************

bool InitText()
{
	// SFML Text

		if (!font.loadFromFile("Content/arial.ttf"))
		{
			cout << "font not loaded" << endl;
			return false;
		}

		score1.setFont(font);
		score2.setFont(font);
		score1.setString("Player 1 Score is: 0 ");
		score1.setCharacterSize(24);
		score1.setStyle(sf::Text::Bold);
		score1.setPosition(200, 10);
		score2.setString("Player 2 Score is: 0");
		score2.setCharacterSize(24);
		score2.setStyle(sf::Text::Bold);
		score2.setPosition(600, 10);
		score1.setColor(sf::Color::Red);
		score2.setColor(sf::Color::Blue);
	
	
	//Border
	border1.setSize(sf::Vector2f(score1.getGlobalBounds().width + 10, score1.getGlobalBounds().height + 15));
	border1.setPosition(score1.getPosition().x - 5,score1.getPosition().y);
	border1.setFillColor(sf::Color::Black);
	border1.setOutlineThickness(5);
	border1.setOutlineColor(sf::Color::White);

	border2.setSize(sf::Vector2f(score2.getGlobalBounds().width + 10, score2.getGlobalBounds().height + 15));
	border2.setPosition(score2.getPosition().x - 5, score2.getPosition().y);
	border2.setFillColor(sf::Color::Black);
	border2.setOutlineThickness(5);
	border2.setOutlineColor(sf::Color::White);


	winner.setFont(font);
	winner.setCharacterSize(100);
	winner.setStyle(sf::Text::Bold);
	winner.setString("");
	winner.setPosition(512, 256);


	PlayAgain.setFont(font);
	PlayAgain.setCharacterSize(50);
	PlayAgain.setStyle(sf::Text::Bold);
	PlayAgain.setString(" ");
	PlayAgain.setPosition(512, 180);

	WelcomeText.setFont(font);
	WelcomeText.setCharacterSize(50);
	WelcomeText.setStyle(sf::Text::Bold);
	WelcomeText.setString("Welcome to my first Pong game! \n\n Press 1 for AI and 2 for Player 2");
	WelcomeText.setPosition(505, 250);
	WelcomeText.setOrigin(WelcomeText.getGlobalBounds().width / 2, WelcomeText.getGlobalBounds().height / 2);


	return true;
}

bool leftScore()
{
	if (shape.getPosition().x < shape.getRadius())
	{
		scoreSound->play();
		RandomizeBlocks();
		shape.setPosition(PONG_RESET);
		velocity.x = RESET; //0
		velocity.y = RESET; //0
		red.setPosition(RED_RESET);
		blue.setPosition(BLUE_RESET);
		player2++;
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
			victoryMusic->play();
			backgroundMusic->stop();
			PlayAgain.setString("Do you want to play again? Y/N? ");
			PlayAgain.setOrigin(PlayAgain.getGlobalBounds().width / 2, PlayAgain.getGlobalBounds().height / 2);
			Play = false;
			//TODO add ask difficulty function
		}

		return true;
	}

}

bool rightScore()
{
	if (shape.getRadius() + shape.getPosition().x > 1024)
	{
		scoreSound->play();
		RandomizeBlocks();
		shape.setPosition(PONG_RESET);
		velocity.x = RESET;
		velocity.y = RESET;
		red.setPosition(RED_RESET);
		blue.setPosition(BLUE_RESET);
		player1++;
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
			victoryMusic->play();
			backgroundMusic->stop();
			PlayAgain.setString("Do you want to play again? Y/N? ");
			PlayAgain.setOrigin(PlayAgain.getGlobalBounds().width / 2, PlayAgain.getGlobalBounds().height / 2);
			Play = false;
			
			//TODO add ask difficulty function
		}
		return true;
	}

}



bool InitSounds()
{

	//Score Sound
	scoreBuffer = new sf::SoundBuffer();
	if (!scoreBuffer->loadFromFile("Audio/ScoreSound.wav"))
	{
		cout << "ScoreSound audio was not loaded" << endl;
		return false;
	}

	//PaddleBounce Sound
	bounceBuffer = new sf::SoundBuffer();
	if (!bounceBuffer->loadFromFile("Audio/Bounce.wav"))
	{
		cout << "Bounce audio was not loaded" << endl;
		return false;
	}

	//WallBounceSound
	wallBounceBuffer = new sf::SoundBuffer();
	if (!wallBounceBuffer->loadFromFile("Audio/WallBounceSound.wav"))
	{
		cout << "WallBounceSound audio was not loaded" << endl;
		return false;
	}
	scoreSound = new sf::Sound();
	scoreSound->setBuffer(*scoreBuffer);

	bounceSound = new sf::Sound();
	bounceSound->setBuffer(*bounceBuffer);

	wallBounceSound = new sf::Sound();
	wallBounceSound->setBuffer(*wallBounceBuffer);

	//Background Music
	backgroundMusic = new sf::Music();
	backgroundMusic->openFromFile("Audio/BackgroundMusic01.wav");
	backgroundMusic->setVolume(20);
	backgroundMusic->play();

	victoryMusic = new sf::Music();
	victoryMusic->openFromFile("Audio/VictoryFanfare.wav");

	return true;
}

void wallBounce()
{
	//bounce top wall
	if (shape.getPosition().y <= shape.getRadius())
	{
		wallBounceSound->play();
		velocity.y *= -0.9f;
		velocity.x *= 0.9f;

		shape.setPosition(shape.getPosition().x, shape.getRadius());

	}

	//bounce bottom wall
	if (shape.getRadius() + shape.getPosition().y >= 512)
	{
		wallBounceSound->play();
		velocity.y *= -0.9f;
		velocity.x *= 0.9f;

		shape.setPosition(shape.getPosition().x, 512 - shape.getRadius());
	}
}



void Init()
{
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	b1Velocity.x = 0;
	b1Velocity.y = -150;
	b2Velocity.x = 0;
	b2Velocity.y = -150;
	b3Velocity.x = 0;
	b3Velocity.y = -150;
	b4Velocity.x = 0;
	b4Velocity.y = -150;
	

	
	if (!InitSounds())
	{
		cout << "Failed to initialized Sounds" << endl;
		//return 0;
	}

	if (!InitText())
	{
		cout << "Failed to initialized Text" << endl;
		//return 0;
	}


	
	shape.setOrigin(shape.getRadius(),shape.getRadius());
	shape.setPosition(PONG_RESET);
	shape.setFillColor(sf::Color::Yellow);



	red.setPosition(RED_RESET);
	red.setFillColor(sf::Color::Red);
	red.setOutlineThickness(3);
	red.setOutlineColor(sf::Color::White);
	//red.setOrigin(red.getSize().x /2, red.getSize().y / 2);




	blue.setPosition(BLUE_RESET);
	blue.setFillColor(sf::Color::Blue);
	blue.setOutlineThickness(3);
	blue.setOutlineColor(sf::Color::White);
	//blue.setOrigin(blue.getSize().x / 2, blue.getSize().y / 2);

	block1.setPosition(200,400);
	block1.setFillColor(sf::Color::Green);
	block1.setOutlineThickness(3);
	block1.setOutlineColor(sf::Color::White);

	block2.setPosition(700, 100);
	block2.setFillColor(sf::Color::Green);
	block2.setOutlineThickness(3);
	block2.setOutlineColor(sf::Color::White);

	block3.setPosition(300, 250);
	block3.setFillColor(sf::Color::Green);
	block3.setOutlineThickness(3);
	block3.setOutlineColor(sf::Color::White);

	block4.setPosition(600, 450);
	block4.setFillColor(sf::Color::Green);
	block4.setOutlineThickness(3);
	block4.setOutlineColor(sf::Color::White);

	mLine.setPosition(PONG_RESET);
	mLine.setFillColor(sf::Color::White);
	mLine.setOrigin(mLine.getGlobalBounds().width / 2, mLine.getGlobalBounds().height / 2);

	RandomizeBlocks();


}

void Update(sf::RenderWindow* window, sf::Time* dt)
{
	
	sf::Event event;
	
	ConstrainPaddles(&red);
	ConstrainPaddles(&blue);
	
	

	int bluePadSpd = 350;
	bluePadSpd = 300;
	

	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed || Quit == true)
			window->close();

		menu(&event, window);

		if (Play != true) // check if player wants to play again - always starts true
		{
		
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Y)
				{
					Play = true;
					RandomizeBlocks();
					player1 = 0;
					score1.setString("Player 1 Score is: 0 ");
					player2 = 0;
					score2.setString("Player 2 Score is: 0");
					WinnerStr.str("");
					WinnerStr.clear();
					PlayAgain.setString(" ");
					winner.setString(WinnerStr.str());
					victoryMusic->stop();
					backgroundMusic->play();
					
				}
				else if (event.key.code == sf::Keyboard::N)
				{
					Quit = true;
				}
			}
		
		}
		else
		{
			
			if (event.type == sf::Event::KeyPressed)
			{
				// begins the game - moves pong
				if (event.key.code == sf::Keyboard::W || sf::Keyboard::S || sf::Keyboard::Up || sf::Keyboard::Down)
				{
					
					if (Start == false && Begin == true)
					{
						
						Start = true;
						int x;
						x = rand() % 2 + 1;
						switch (x)
						{
						case 1:
							velocity.x = 200;
							
							velocity.y = rand() % -100 + 101;
							break;
						case 2:
							velocity.x = -200;

							velocity.y = rand() % 100 + -201;
							break;
						default:
							velocity.x = 200;

							velocity.y = rand() % 0 + 201;
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
	
	//TODO turn into function
	if (Play == true && Start == true)
	{

		//TODO increase paddle movespeed as velocity increases
		// red move up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{


			player1MovingUp = true;
			red.move(0, -350 * dt->asSeconds());

		}

		// red move down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{

			player1MovingDown = true;
			red.move(0, 350 * dt->asSeconds());

		}

		
		//AI
		if (Easy)
		{
			bluePadSpd = 50;
		}

		if (Medium)
		{
			bluePadSpd = 200;
			blockCollision(&block1);
			blockCollision(&block2);
			blockCollision(&block3);
			blockCollision(&block4);


		}


		if (Hard)
		{
			bluePadSpd = 250;
			blockBounce();
			blockCollision(&block1);
			blockCollision(&block2);
			blockCollision(&block3);
			blockCollision(&block4);
			block1.move(0, b1Velocity.y * dt->asSeconds());
			block2.move(0, -b2Velocity.y * dt->asSeconds());
			block3.move(0, (b3Velocity.y / 2) * dt->asSeconds());
			block4.move(0, (b4Velocity.y * 2) * dt->asSeconds());

		}


		if (Hell)
		{
			bluePadSpd = 350;


		}



		if (AIChosen == true)
		{
			AI(player2MovingUp, player2MovingDown, &bluePadSpd, dt);
		}

		else if (player2Chosen == true)
		{
			// blue move up(player 2 controlled)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				player2MovingUp = true;
				blue.move(0, -350 * dt->asSeconds());
			
			}

			//blue move down(player 2 controlled
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				player2MovingDown = true;
				blue.move(0, 350 * dt->asSeconds());
				

			}

		}

	}

	CollisionPaddles(&red, &blue, player1MovingUp, player1MovingDown, player2MovingUp, player2MovingDown);
	wallBounce();
	leftScore();
	rightScore();



	if (Start == true)
	{
		shape.move(velocity.x * dt->asSeconds(), velocity.y * dt->asSeconds());
		//cout << "ball position  in x is: " << shape.getPosition().x << endl;
		//cout << "Ball position in y is: " << shape.getPosition().y << endl;
	}
	
}




void Render(sf::RenderWindow* window)
{
	window->clear();
	window->draw(WelcomeText);
	if (Begin == true)
	{
		window->draw(border1);
		window->draw(border2);
		window->draw(mLine);
		window->draw(shape);
		window->draw(red);
		window->draw(blue);
		if (Hard == true || Hell == true || Medium == true)
		{
			window->draw(block1);
			window->draw(block2);
			window->draw(block3);
			window->draw(block4);
		}
		window->draw(score1);
		window->draw(score2);
		window->draw(winner);
		window->draw(PlayAgain);

	}
	window->display();

}



void ConstrainPaddles(sf::RectangleShape* paddle)
{
	if (paddle != NULL)
	{
		//top
		if (paddle->getPosition().y <= 0 ) //paddle->getSize().y /2)
		{
			paddle->setPosition(paddle->getPosition().x, 0); //paddle->getSize().y / 2);
		}

		//bottom constrain
		if (paddle->getSize().y + paddle->getPosition().y >= 512 )//+ paddle->getSize().y /2 )
		{
			paddle->setPosition(paddle->getPosition().x, 512 - paddle->getSize().y); // /2);
		}
	}
}

void CollisionPaddles(sf::RectangleShape* red, sf::RectangleShape* blue, bool movingUp1, bool movingDown1 , bool movingUp2, bool movingDown2)
{
	sf::Vector2f pongPos = shape.getPosition();
	float radius = shape.getRadius();
	float power = 1.0f;
	power = abs(power);
	power = 1 - power;
	float redMidY = red->getPosition().y + red->getSize().y * 0.5f;
	float blueMidY = blue->getPosition().y + blue->getSize().y * 0.5f;



	//red paddle collision
	if (pongPos.x + radius > red->getPosition().x &&
		pongPos.x - radius < red->getPosition().x + red->getSize().x  &&
		pongPos.y + radius > red->getPosition().y   &&
		pongPos.y + radius < red->getPosition().y + red->getSize().y)
	{
		float ballDistance = pongPos.y - redMidY;
		power = ballDistance / (red->getSize().y * 0.5f);
		bounceSound->play();
		shape.setPosition(red->getPosition().x + red->getSize().x + radius, pongPos.y);
		velocity.x *= -1.25;
		
		cout << "red position in x is" << red->getPosition().x << endl;
		cout << "pong position is " << pongPos.x << " " << pongPos.y << endl;
		if (movingUp1 == true)
		{
			velocity.y = 150 * power;
			//cout << "p1 is moving up" << endl;

		}
		else if (movingDown1 == true)
		{
			velocity.y = -150 * power;
			//cout << "p1 is moving down" << endl;

		}


	}

	//blue paddle collision
	if (pongPos.x + radius > blue->getPosition().x &&
		pongPos.x - radius < blue->getPosition().x + blue->getSize().x   &&
		pongPos.y + radius > blue->getPosition().y &&
		pongPos.y + radius < blue->getPosition().y + blue->getSize().y)
	{
		float ballDistance = pongPos.y - blueMidY;
		power = ballDistance / (blue->getSize().y * 0.5f);
		bounceSound->play();
		shape.setPosition(blue->getPosition().x - radius, pongPos.y);
		velocity.x *= -1.25;



		if (movingUp2)
		{
			velocity.y = 150 * power;
			//cout << "p2 is moving up" << endl;
		}
		else if (movingDown2)
		{
			velocity.y = -150 * power;
			//cout << "p2 is moving down" << endl;
		}


	}
}

void blockCollision(sf::RectangleShape* block)
{
	if (shape.getPosition().x + shape.getRadius() > block->getPosition().x &&
		shape.getPosition().x - shape.getRadius() < block->getPosition().x + block->getSize().x &&
		shape.getPosition().y + shape.getRadius() > block->getPosition().y &&
		shape.getPosition().y + shape.getRadius() < block->getPosition().y + block->getSize().y)
	{
		bounceSound->play();
		velocity.x *= -1.2;
		velocity.y *= -1.2;
		
		cout << "Ball is colliding" << endl; 
		//bounce
		if (shape.getPosition().x < block->getPosition().x )
		{
			shape.setPosition(block->getPosition().x - shape.getRadius(), shape.getPosition().y);
			cout << " left" << endl;
		}
		// bounce top
		if (shape.getPosition().y < block->getPosition().y && shape.getPosition().y < block->getPosition().y + block->getSize().x)
		{
			cout << "top" << endl;
			shape.setPosition(shape.getPosition().x, block->getPosition().y - shape.getRadius());
		}

		//bounce right
		if (shape.getPosition().x > block->getPosition().x)
		{


			shape.setPosition(block->getPosition().x + shape.getRadius() + block->getSize().x, shape.getPosition().y);
			cout << " right" << endl;
		}

		//bounce bottom
		if (shape.getPosition().y > block->getPosition().y + block->getSize().y && shape.getPosition().y > block->getPosition().y + block->getSize().y + block->getSize().x)
		{
			cout << "bottom" << endl;
			shape.setPosition(shape.getPosition().x, block->getPosition().y + block->getSize().y + shape.getRadius());
		}


	}


}

void menu(sf::Event* event, sf::RenderWindow* window)
{

	if (Begin == false)
	{
		switch (event->type)
		{
		case sf::Event::KeyPressed:
			if (AIChosen == false && player2Chosen == false)
			{
				switch (event->key.code)
				{
				case sf::Keyboard::Num1:
				{
					WelcomeText.setString(" ");
					WelcomeText.setString("         Choose difficulty: \n3. Easy, 4. Medium, 5. Hard");
					WelcomeText.setPosition(512, 180);
					WelcomeText.setOrigin(WelcomeText.getGlobalBounds().width / 2, WelcomeText.getGlobalBounds().height / 2);

					AIChosen = true;
					break;

				}
				case sf::Keyboard::Num2:
				{
					WelcomeText.setString(" ");
					WelcomeText.setString("Player 2: On ");
					WelcomeText.setCharacterSize(20);
					WelcomeText.setPosition(400, 100);

					player2Chosen = true;
					Begin = true;
					break;
				}
				default:
					window->close();
					break;


				}
			}
		}

		if (diffChosen == false && AIChosen == true)
		{
			switch (event->key.code)
			{
			case sf::Keyboard::Num3:
			{
				WelcomeText.setString("Difficulty: Easy");
				WelcomeText.setCharacterSize(20);
				WelcomeText.setPosition(330, 65);
				diffChosen = true;
				Easy = true;
				Begin = true;
				break;
			}
			case sf::Keyboard::Num4:
			{
				WelcomeText.setString("Difficulty: Medium");
				WelcomeText.setCharacterSize(20);
				WelcomeText.setPosition(330, 65);
				diffChosen = true;
				Medium = true;
				Begin = true;

				break;
			}
			case sf::Keyboard::Num5:
			{
				WelcomeText.setString("Difficulty: Hard");
				WelcomeText.setCharacterSize(20);
				WelcomeText.setPosition(330, 65);
				diffChosen = true;
				Hard = true;
				Begin = true;
				break;
			}

			}
		}

	}
}




bool AI(bool player2MovingUp, bool player2MovingDown, int* paddleSpd, sf::Time* dt)
{
	bool moveUP = false;
	if (shape.getPosition().y <= blue.getPosition().y )
	{
		moveUP = true;
	}

	if (moveUP == true)
	{
			player2MovingUp = true;
			blue.move(0, (-1 * (*paddleSpd) * dt->asSeconds()));
			//cout << " Blue pad speed while moving is: " << paddleSpd << endl;
	}

	else
	{

		player2MovingDown = true;
		blue.move(0, *paddleSpd * dt->asSeconds());
		
	}

	return true;
}

void blockBounce()
{


	if (block1.getPosition().y <= 0)
	{
		b1Velocity.y *= -1;
	}

	if (block1.getPosition().y + block1.getSize().y >= 512)
	{
		b1Velocity.y *= -1;
	}
	if (block2.getPosition().y <= 0)
	{
		b2Velocity.y *= -1;
	}

	if (block2.getPosition().y + block2.getSize().y >= 512)
	{
		b2Velocity.y *= -1;
	}
	if (block3.getPosition().y <= 0)
	{
		b3Velocity.y *= -1;
	}

	if (block3.getPosition().y + block3.getSize().y >= 512)
	{
		b3Velocity.y *= -1;
	}

	if (block4.getPosition().y <= 0)
	{
		b4Velocity.y *= -1;
	}

	if (block4.getPosition().y + block4.getSize().y >= 512)
	{
		b4Velocity.y *= -1;
	}
	

}

void RandomizeBlocks()
{
	block1.setPosition(rand() % 100 + 150, rand() % +200 + 200); // player1 side
	block2.setPosition(rand() % 200 + 550, rand() % +50 + 100);  // player2 side
	block3.setPosition(rand() % 150 + 100, rand() % +150 + 50); // player1 side
	block4.setPosition(rand() % 100 + 550, rand() % +150 + 250); // player2 side
}




//**********************************************************************************
//**********************************************************************************
//Old unused codes for referrence 
/*
//top constrain
if (blue.getPosition().y < blue.getGlobalBounds().height / 2)
{
blue.setPosition(blue.getPosition().x, blue.getGlobalBounds().height / 2);
}
// bottom constrain
if (blue.getGlobalBounds().height + blue.getPosition().y >= 512 + blue.getGlobalBounds().height / 2)
{
blue.setPosition(blue.getPosition().x, 512 - blue.getGlobalBounds().height / 2);

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


//float blueMidY = blue.getPosition().y + blue.getSize().y * 0.5f;
//blue paddle collision
if (pongPos.x + radius > blue.getPosition().x &&
pongPos.x - radius < blue.getPosition().x + blue.getSize().x &&
pongPos.y + radius > blue.getPosition().y &&
pongPos.y + radius < blue.getPosition().y + blue.getSize().y)
{
float ballDistance = pongPos.y - blueMidY;
power = ballDistance / (blue.getSize().y * 0.5f);
bounceSound->play();

velocity.x *= -1.25;
if (player2MovingUp)
{
velocity.y = -100 * power;

}
else if (player2MovingDown)
{
velocity.y = 100 * power;

}


}

*/






