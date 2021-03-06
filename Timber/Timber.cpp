// include required C++ libraries

#include "stdafx.h"
#include<sstream>
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// define namespace for simplicity of the code
using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
// Player or branche ==> Left/Right?
enum class side {LEFT , RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	// Create an object for class VideoMode to define the screen resoultion
	VideoMode vm(1920, 1080);

	// Create a window using defined resolution, name of window and it's style
	RenderWindow window(vm, "Timber Game", Style::Fullscreen);

	//Create a Texture class object
	Texture textureBackground;

	textureBackground.loadFromFile("graphics/background.png");

	// Creat a Sprite object
	Sprite spriteBackground;

	// Attach the texture to Sprite
	spriteBackground.setTexture(textureBackground);

	//Set the Sprite position
	spriteBackground.setPosition(0, 0);

	// Create a Tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Create a moving bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	float beeActive = false;
	float beeSpeed = 0.0f;

	//Create 3 clouds
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	//Check if clouds are currently on screen
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//Speed of clouds
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// Start the clock
	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track the status of game i.e. paused/not paused
	bool paused = true;

	// Text
	int score = 0;
	Text messageText;
	Text scoreText;

	// Choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	//Assign the message
	messageText.setString("Press Enter to Start!");
	scoreText.setString("Score = 0");

	//Make it big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//Create 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch Sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	//Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//At first, the player is on the left side
	side playerSide = side::LEFT;

	//Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Useful log variable
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//control the player input
	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// time out
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	//Prepare the sound
	


	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);
	// while loop to ensure the window is displayed untill "Esc" is pressed by the user
	while (window.isOpen())
	{


		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;

				//Hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			//Reset time and score
			score = 0;
			timeRemaining = 5;

			//make all the branches disapper
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			//Hide the gravestone
			spriteRIP.setPosition(675, 2000);

			//Align the player
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		//Assure that we are accepting inputs
		if (acceptInput) {
			//Handling right key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				//Make sure the player is on the right
				playerSide = side::RIGHT;
				score++;

				//Inc. the amt. of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				//update the branches
				updateBranches(score);

				//set the log flying to the left
				spriteLog.setPosition(810, 720);

				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				chop.play();
			}
		}

		if (acceptInput) {
			//Handling right key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				//Make sure the player is on the right
				playerSide = side::LEFT;
				score++;

				//Inc. the amt. of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				//update the branches
				updateBranches(score);

				//set the log flying to the left
				spriteLog.setPosition(810, 720);

				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				chop.play();
			}
		}
		// Measure time between frames
		Time dt = clock.restart();
		//Subtract the amount of time remaining

		if (!paused)
		{
			timeRemaining -= dt.asSeconds();
			//Resize the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond*timeRemaining, timeBarHeight));

		}
		

		if (timeRemaining <= 0.0f)
		{
			//Pause the game
			paused = true;

			//Change the message on screen
			messageText.setString("Time Out!!");

			//Reposition the message based on its new size
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			outOfTime.play();
		}


		if (!paused)
		{
			// Set up the bee
			if (!beeActive)
			{
				// Speed of the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200; // Random number between 199 and 399

				// Height of the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500; // Random number between 499 and 999
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				// Move the bee
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}

			}

			// Manage the clouds
			//Cloud 1
			if (!cloud1Active)
			{
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}
			// Cloud 2
			if (!cloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}
			// Cloud 3
			if (!cloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}
		//Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			//Update the branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					//Move the Sprite to the left side
					branches[i].setPosition(610, height);
					//Flip the Sprite the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					//Move the sprite to right side
					branches[i].setPosition(1330, height);
					//Flip the Sprite to normal
					branches[i].setRotation(0);
				}
				else
				{
					//Hide
					branches[i].setPosition(3000, height);
				}
			}

			//Handle flying log

			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				//check if the log reached right edge
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					//new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			//Player Squished??

			if (branchPositions[5] == playerSide)
			{
				//death
				paused = true;
				acceptInput = false;

				//draw gravestone
				spriteRIP.setPosition(525, 760);
				//hide the player
				spritePlayer.setPosition(2000, 660);

				//change the text of the message
				messageText.setString("Oops! Squished!");

				//centre it on screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				death.play();
			}
		} // End if (paused)

			// clear the last frame
			window.clear();

			//Draw current game scene
			window.draw(spriteBackground);

			// Draw clouds
			window.draw(spriteCloud1);
			window.draw(spriteCloud2);
			window.draw(spriteCloud3);

			//Draw branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				window.draw(branches[i]);
			}

			// Draw Tree
			window.draw(spriteTree);

			//Draw the player
			window.draw(spritePlayer);

			//Draw the axe
			window.draw(spriteAxe);

			//Draw the flying log
			window.draw(spriteLog);

			//Draw the gravestone
			window.draw(spriteRIP);

			// Draw Bee
			window.draw(spriteBee);

			//Draw score
			window.draw(scoreText);

			//Draw time bar
			window.draw(timeBar);


			if (paused)
			{ 
				// Draw the message
				window.draw(messageText);
			}


			//Display current game scene
			window.display();
		
	}
    return 0;
}

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
 	}

	//New branch at position 0
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{ 
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}