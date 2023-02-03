#include "Game.hpp"

Game::Game(const char* title, int x, int y, int w, int h)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0 && SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << "Subsystem initialized." << std::endl;
		scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);
	    menuFont = TTF_OpenFont("OpenSans-Regular.ttf", 15);

		if (scoreFont && menuFont)
		{
			std::cout << "Fonts loaded successfully." << std::endl;
		}
		else
			std::cout << "Error: " << SDL_GetError() << std::endl;

		window = SDL_CreateWindow(title, x, y, w, h, 0);
		if (window)
		{
			std::cout << "Window created successfully." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer created successfully." << std::endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}
}

Game::~Game()
{
	SDL_DestroyWindow(window);
	std::cout << "Window destroyed." << std::endl;
	SDL_DestroyRenderer(renderer);
	std::cout << "Renderer destoyed." << std::endl;
	SDL_Quit();
	std::cout << "Game cleaned." << std::endl;
}

Contact CheckPaddleCollision(Ball* const& ball, Paddle* const& paddle)
{
	float ballLeft = ball->position.getX();
	float ballRight = ball->position.getX() + B_WIDTH;
	float ballTop = ball->position.getY();
	float ballBottom = ball->position.getY() + B_HEIGHT;

	float paddleLeft = paddle->position.getX();
	float paddleRight = paddle->position.getX() + P_WIDTH;
	float paddleTop = paddle->position.getY();
	float paddleBottom = paddle->position.getY() + P_HEIGHT;

	Contact contact{};

	if (ballLeft >= paddleRight)
	{
		return contact;
	}

	if (ballRight <= paddleLeft)
	{
		return contact;
	}

	if (ballTop >= paddleBottom)
	{
		return contact;
	}

	if (ballBottom <= paddleTop)
	{
		return contact;
	}

	float paddleRangeUpper = paddleBottom - (2.0f * P_HEIGHT / 3.0f);
	float paddleRangeMiddle = paddleBottom - (P_HEIGHT / 3.0f);

	if (ball->velocity.getX() < 0)
	{
		// Left paddle
		contact.penetration = paddleRight - ballLeft;
	}
	else if (ball->velocity.getX() > 0)
	{
		// Right paddle
		contact.penetration = paddleLeft - ballRight;
	}

	if ((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper))
	{
		contact.type = CollisionType::Top;
	}
	else if ((ballBottom > paddleRangeUpper) && (ballBottom < paddleRangeMiddle))
	{
		contact.type = CollisionType::Middle;
	}
	else
	{
		contact.type = CollisionType::Bottom;
	}

	return contact;
}

Contact CheckWallCollision(Ball* const& ball)
{
	float ballLeft = ball->position.getX();
	float ballRight = ball->position.getX() + B_WIDTH;
	float ballTop = ball->position.getY();
	float ballBottom = ball->position.getY() + B_HEIGHT;

	Contact contact{};

	if (ballLeft < 0.0f)
	{
		contact.type = CollisionType::Left;
	}
	else if (ballRight > W_WIDTH)
	{
		contact.type = CollisionType::Right;
	}
	else if (ballTop < 0.0f)
	{
		contact.type = CollisionType::Top;
		contact.penetration = -ballTop;
	}
	else if (ballBottom > W_HEIGHT)
	{
		contact.type = CollisionType::Bottom;
		contact.penetration = W_HEIGHT - ballBottom;
	}

	return contact;
}

void Game::gameplay(bool b)
{
	Score playerOneScoreText(Coordinates(W_WIDTH / 4, 20), renderer, scoreFont);

	Score playerTwoScoreText(Coordinates(3 * W_WIDTH / 4, 20), renderer, scoreFont);

	//Ball ball(Coordinates(W_WIDTH / 2.0f, W_HEIGHT / 2.0f), Coordinates(BALL_SPEED, 0.0f));

	ball = new Ball(Coordinates(W_WIDTH / 2.0f, W_HEIGHT / 2.0f), Coordinates(BALL_SPEED, 0.0f));

	paddleOne = new Paddle(Coordinates(50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));

	if (b == true)
		paddleTwo = new Paddle(Coordinates(W_WIDTH - 50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));
	else
		paddleTwo = new BotPaddle(Coordinates(W_WIDTH - 50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));

	int playerOneScore = 0;
	int playerTwoScore = 0;

	bool running = true;
	bool buttons[4] = {};

	float dt = 0.0f;

	while (running)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
				else if (event.key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = true;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = true;
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = true;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = true;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = false;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = false;
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = false;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = false;
				}
			}
		}


		if (buttons[Buttons::PaddleOneUp])
		{
			paddleOne->velocity.setY(-PADDLE_SPEED);
		}
		else if (buttons[Buttons::PaddleOneDown])
		{
			paddleOne->velocity.setY(PADDLE_SPEED);
		}
		else
		{
			paddleOne->velocity.setY(0.0f);
		}

		if (b == true)
		{
			if (buttons[Buttons::PaddleTwoUp])
			{
				paddleTwo->velocity.setY(- PADDLE_SPEED);
			}
			else if (buttons[Buttons::PaddleTwoDown])
			{
				paddleTwo->velocity.setY(PADDLE_SPEED);
			}
			else
			{
				//if (b == true)
				paddleTwo->velocity.setY(0.0f);
			}
		}
		
		// Update the paddle positions
		paddleOne->Update(dt);
		paddleTwo->Update(dt);

		// Update the ball position
		ball->Update(dt);

		if (b == false)
			paddleTwo->Tracking(ball->position);

		// Check collisions
		if (Contact contact = CheckPaddleCollision(ball, paddleOne); contact.type != CollisionType::None)
		{
			ball->CollideWithPaddle(contact);
		}
		else if (contact = CheckPaddleCollision(ball, paddleTwo); contact.type != CollisionType::None)
		{
			ball->CollideWithPaddle(contact);
		}
		else if (contact = CheckWallCollision(ball); contact.type != CollisionType::None)
		{
			ball->CollideWithWall(contact);
			if (contact.type == CollisionType::Left)
			{
				++playerTwoScore;

				playerTwoScoreText.SetScore(playerTwoScore);
			}
			else if (contact.type == CollisionType::Right)
			{
				++playerOneScore;

				playerOneScoreText.SetScore(playerOneScore);
			}
		}

		// Clear the window to black
		SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(renderer);

		// Set the draw color to be white
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);


		// Draw the net
		for (int y = 0; y < W_HEIGHT; ++y)
		{
			if (y % 5)
			{
				SDL_RenderDrawPoint(renderer, W_WIDTH / 2, y);
			}
		}

		// Draw the ball
		ball->Draw(renderer);

		// Draw the paddles
		paddleOne->Draw(renderer);
		paddleTwo->Draw(renderer);

		playerOneScoreText.Draw();
		playerTwoScoreText.Draw();

		// Present the backbuffer
		SDL_RenderPresent(renderer);

		// Calculate frame time
		auto stopTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
	}

	// Cleaning screen after ending "while(running)" loop (ESC button)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void Game::printText(std::string name, int x, int y, int w, int h, TTF_Font* font)
{
	tmpSurface = TTF_RenderText_Solid(font, name.c_str(), White);

	tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_RenderCopy(renderer, tmpTexture, NULL, &rect);

	SDL_RenderPresent(renderer);

	SDL_FreeSurface(tmpSurface);

	SDL_DestroyTexture(tmpTexture);
}

void Game::handleEvents(SDL_Event* event)
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			isRunning = false;
		}
		else if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			else if (event->key.keysym.sym == SDLK_1)
			{
				gameplay(true);

			}
			else if (event->key.keysym.sym == SDLK_2)
			{
				gameplay(false);
			}
		}
	}
}

void Game::renderMenu()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	printText("Pong by Artem Kapustkin", W_WIDTH / 2.0f - 500 / 2.0f, W_HEIGHT / 2.0f - 50 - 100, 500, 100, menuFont);
	printText("1) Player vs Player", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f - 50, 250, 50, menuFont);
	printText("2) Player vs Bot", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f, 250, 50, menuFont);
	SDL_RenderPresent(renderer);
}