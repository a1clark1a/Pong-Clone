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
const::sf::Vector2f BLUE_RESET(1000.0f,256);
const int MAX_SCORE = 5;
const float RESET = 0.0f;

int player1 = 0;
int player2 = 0;
bool Play = true;
bool Start = false;
bool Begin = false;
bool Quit = false;
bool AIchosen = false;
bool p2Chosen = false;



sf::Music* backgroundMusic;
sf::Music* victoryMusic;
sf::SoundBuffer* wallBounceBuffer;
sf::SoundBuffer* scoreBuffer;
sf::SoundBuffer* bounceBuffer;
sf::Sound* bounceSound;
sf::Sound* wallBounceSound;
sf::Sound* scoreSound;
sf::Vector2f velocity;



sf::CircleShape shape(20.f); // pong
sf::RectangleShape red(sf::Vector2f(20.0f, 100.0f)); // red paddle (player1)
sf::RectangleShape blue(sf::Vector2f(20.0f, 100.0f)); //blue paddle (player2)
sf::RectangleShape mLine(sf::Vector2f(5.0f, 520)); //MiddleLine

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

bool AI(bool, bool, sf::Time*);
bool InitSounds();
bool InitText();
bool leftScore(); 
bool rightScore(); 
void Init();
void wallBounce();


// passed in as pointers
void Update(sf::RenderWindow*, sf::Time*); 
void Render(sf::RenderWindow* ); 
void ConstrainPaddles(sf::RectangleShape*); 
void CollisionPaddles(sf::RectangleShape*, sf::RectangleShape*, bool,bool,bool,bool); //needs refactoring






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
	WelcomeText.setString("Welcome to my first Pong game! \n\n        Press Enter to Play");
	WelcomeText.setPosition(512, 180);
	WelcomeText.setOrigin(WelcomeText.getGlobalBounds().width / 2, WelcomeText.getGlobalBounds().height / 2);
	


	return true;
}

bool leftScore()
{
	if (shape.getPosition().x < shape.getRadius())
	{
		scoreSound->play();
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



	mLine.setPosition(PONG_RESET);
	mLine.setFillColor(sf::Color::White);
	mLine.setOrigin(mLine.getGlobalBounds().width / 2, mLine.getGlobalBounds().height / 2);


}

void Update(sf::RenderWindow* window, sf::Time* dt)
{
	
	sf::Event event;
	
	ConstrainPaddles(&red);
	ConstrainPaddles(&blue);
	bool AIchosen = false;
	bool p2Chosen = false;
	

	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed || Quit == true)
			window->close();
		
		if (Begin == false )
		{
			if (event.KeyPressed == sf::Event::KeyPressed)
			{
				//TODO ask player if they want multiplayer or AI

				
				int response;
				{
					if (event.key.code == sf::Keyboard::Return)
					{

						WelcomeText.setString(" ");
						WelcomeText.setString("Press 1 for AI, Press 1 for 2 Players");
						WelcomeText.setPosition(512, 180);

					}

					if (event.KeyPressed == sf::Event::KeyPressed)
					{

						if (event.key.code == sf::Keyboard::Num1)
						{
							response = 1;

						}
						if (event.key.code == sf::Keyboard::Num2)
						{

							response = 2;

						}

						switch (response)
						{
						case 1:
							// set AI to true
							//TODO make choosing difficulty into a function so that difficulty function can be called whenever
							//player is asked to play again
							cout << "You chose AI!" << endl;
							cout << "Choose Difficulty : Press 1.Easy, 2. Medium, 3.Hard " << endl;
							AIchosen = true;
							break;

						case 2:
							cout << " you chose multiplayer " << endl;
							//set player 2 to true
							p2Chosen = true;
							break;
						default:
							return;
						}

					}
					AIchosen = true;
					WelcomeText.setString(" ");
				}
			}
		}




		if (Play != true) // check if player wants to play again - always starts true
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
			//TODO if AI ask for difficulty
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
	
	//TODO turn into function
	if (Play == true || Start == true)
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
		

		//TODO Set and check difficulty level
		//TODO change AI y velocity according to difficulty
		//AI
		AI(player2MovingUp, player2MovingDown, dt); //TODO

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

	CollisionPaddles(&red, &blue, player1MovingUp, player1MovingDown, player2MovingUp, player2MovingDown);
	wallBounce();
	leftScore();
	rightScore();



	if (Start == true)
	{
		shape.move(velocity.x * dt->asSeconds(), velocity.y * dt->asSeconds());
	}
	
}


void Render(sf::RenderWindow* window)
{
	window->clear();
	window->draw(border1);
	window->draw(border2);
	window->draw(mLine);
	window->draw(shape);
	window->draw(red);
	window->draw(blue);
	window->draw(score1);
	window->draw(score2);
	window->draw(winner);
	window->draw(PlayAgain);
	window->draw(WelcomeText);
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
			paddle->setPosition(paddle->getPosition().x, 512 - paddle->getSize().y );
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
			cout << "p1 is moving up" << endl;

		}
		else if (movingDown1 == true)
		{
			velocity.y = -150 * power;
			cout << "p1 is moving down" << endl;

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
			cout << "p2 is moving up" << endl;
		}
		else if (movingDown2)
		{
			velocity.y = -150 * power;
			cout << "p2 is moving down" << endl;
		}


	}
}




bool AI(bool player2MovingUp, bool player2MovingDown, sf::Time* dt)
{
	bool moveUP = false;
	if (shape.getPosition().y <= blue.getPosition().y )
	{
		moveUP = true;
	}

	if (moveUP == true)
	{

		player2MovingUp = true;
		blue.move(0, -350 * dt->asSeconds());
		
	}

	else
	{

		player2MovingDown = true;
		blue.move(0, 350 * dt->asSeconds());
		
	}

	return true;
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






