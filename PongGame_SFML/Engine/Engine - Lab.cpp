// Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 512), "Welcome to SDVA 203!");
	sf::CircleShape shape(25);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(512, 100);
	shape.setFillColor(sf::Color::White);
	
	sf::Texture linkTexture;
	if (!linkTexture.loadFromFile("Content/zelda.png"))
	{
		std::cout << "Failed to load link texture" << std::endl;
		return 0;
	}
	//shape.setTexture(&linkTexture);	

	int row = 8;
	int col = 0;
	int counter = 0;
	sf::Clock clock;
	float angleInDegrees = 225;
	float angleInRadians = angleInDegrees * 2 * 3.1415 / 360;
	float xVelocity = cos(angleInRadians) * 500;
	float yVelocity = sin(angleInRadians) * 500;
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
				if (event.key.code == sf::Keyboard::Space)
				{
					yVelocity *= -1;
				}
			}
		}
						
		shape.move(xVelocity * dt.asSeconds(), yVelocity * dt.asSeconds());
		
		sf::Vector2f position = shape.getPosition();
		if (position.y <= shape.getRadius())
		{			
			yVelocity *= -1;
			shape.setPosition(position.x, shape.getRadius());
		}

		if (shape.getRadius() + position.y >= 512)
		{
			yVelocity *= -1;
			shape.setPosition(position.x, 512- shape.getRadius());
		}
		if (position.x <= shape.getRadius())
		{
			xVelocity *= -1;
			shape.setPosition(shape.getRadius(), position.y);
		}

		std::cout << "Ball position is " << position.x <<","<<position.y << std::endl;
		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}
