// These "include" code from the C++ library and SFML too
#include "stdafx.h"
#include "bat.h"
#include "ball.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <Windows.h>


// Avoid having to put sf in front of all the SFML classes and functions
using namespace sf;

void displayLoseMessage(RenderWindow &window, Text message) {

	window.clear(Color(26, 128, 182, 255));
	window.draw(message);
	window.display();
	Sleep(2000);

	for (unsigned int i = 5; i > 0; i--) { //countdown to restart
		String currentCount = std::to_string(i);
		String msg = "Restarting in ";
		msg += currentCount + "...";
		message.setString(msg);
		window.clear(Color(26, 128, 182, 255));
		window.draw(message);
		window.display();
		Sleep(1000);
	}	

}

int main()
{
	int windowWidth = 1024;
	int windowHeight = 768;
	// Make a window that is 1024 by 768 pixels
	// And has the title "SimplePong"
	RenderWindow window(VideoMode(windowWidth, windowHeight), "SimplePong");

	int score = 0;
	int lives = 3;

	// create a bat
	Bat bat(windowWidth / 2, windowHeight - 20);

	// create a ball
	int ballStartingX = windowWidth / 2;
	Ball ball(ballStartingX, 1);

	Text hud;
	Text loseMessage;

	// We need to choose a font
	Font font;
	font.loadFromFile("Chunkfive.otf");

	// Set the font to our message
	hud.setFont(font);
	loseMessage.setFont(font);

	// Make it really big
	hud.setCharacterSize(75);
	loseMessage.setCharacterSize(75);

	// Choose a color
	hud.setFillColor(sf::Color::White);
	loseMessage.setFillColor(sf::Color::White);

	loseMessage.setString("You lose!");


	while (window.isOpen())
	{
		//Handle the player input

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				// Someone closed the window- bye
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			bat.moveLeft();
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			bat.moveRight();
		}
		else if (Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			// quit.
			window.close();
		}

		// Handle ball hitting the bottom
		if (ball.getPosition().top > windowHeight)
		{
			// reverse the ball direction
			ball.hitBottom();

			// Remove a life
			lives--;

			// Check for zero lives
			if (lives < 1) {
				// reset the score and lives
				score = 0;
				lives = 3;

				displayLoseMessage(window, loseMessage);
			}

		}

		// Handle ball hitting top
		if (ball.getPosition().top < 0)
		{
			ball.reboundBatOrTop();
			score++;

		}

		// Handle ball hitting sides
		if (ball.getPosition().left < 0 || ball.getPosition().left + 10 > windowWidth)
		{
			ball.reboundSides();
		}

		// Has the ball hit the bat?
		if (ball.getPosition().intersects(bat.getPosition()))
		{
			// Hit detected so reverse the ball and score a point
			ball.reboundBatOrTop();
		}

		ball.update();
		bat.update();

		// Update the HUD text
		std::stringstream ss;
		ss << "Score:" << score << "    Lives:" << lives;
		hud.setString(ss.str());


		// Clear everything from the last frame
		window.clear(Color(26, 128, 182, 255));

		window.draw(bat.getShape());

		window.draw(ball.getShape());

		// Draw our score
		window.draw(hud);

		// Show everything we just drew
		window.display();
	}// This is the end of the "while" loop

	return 0;
}
