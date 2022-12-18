#include"Figures.hpp"
#include <chrono>

Figure::Figure(Coordinates position, Coordinates velocity) : position(position), velocity(velocity)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);
	rect.w = 10;
	rect.h = 10;
}

void Figure::Update(float dt)
{
	position += velocity * dt;
}

void Figure::Draw(SDL_Renderer* renderer)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);

	SDL_RenderFillRect(renderer, &rect);
}

void Paddle::Update(float dt)
{
	position += velocity * dt;

	if (position.y < 0)
	{
		// Restrict to top of the screen
		position.y = 0;
	}
	else if (position.y > (W_HEIGHT - P_HEIGHT))
	{
		// Restrict to bottom of the screen
		position.y = W_HEIGHT - P_HEIGHT;
	}
}

void Paddle::Draw(SDL_Renderer* renderer)
{
	rect.y = static_cast<int>(position.y);

	SDL_RenderFillRect(renderer, &rect);
}

void Paddle::setStartCoordinates(Coordinates position, Coordinates velocity)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);
}

Paddle::Paddle(Coordinates position, Coordinates velocity) : Figure(position, velocity)
{
	rect.w = P_WIDTH;
	rect.h = P_HEIGHT;
}

void Ball::CollideWithPaddle(Contact const& contact)
{
	position.x += contact.penetration;
	velocity.x = -velocity.x;

	if (contact.type == CollisionType::Top)
	{
		velocity.y = -0.75f * BALL_SPEED;
	}
	else if (contact.type == CollisionType::Bottom)
	{
		velocity.y = 0.75f * BALL_SPEED;
	}
}

void Ball::CollideWithWall(Contact const& contact)
{
	if ((contact.type == CollisionType::Top) || (contact.type == CollisionType::Bottom))
	{
		position.y += contact.penetration;
		velocity.y = -velocity.y;
	}
	else if (contact.type == CollisionType::Left)
	{
		position.x = W_WIDTH / 2.0f;
		position.y = W_HEIGHT / 2.0f;
		velocity.x = BALL_SPEED;
		velocity.y = ((rand() / (RAND_MAX + 0.5)) - 0.5) * BALL_SPEED;
	}
	else if (contact.type == CollisionType::Right)
	{
		position.x = W_WIDTH / 2.0f;
		position.y = W_HEIGHT / 2.0f;
		velocity.x = -BALL_SPEED;
		velocity.y = ((rand() / (RAND_MAX + 0.5)) - 0.5) * BALL_SPEED;
	}
}

Ball::Ball(Coordinates position, Coordinates velocity) : Figure(position, velocity)
{
	rect.w = B_WIDTH;
	rect.h = B_HEIGHT;
}

BotPaddle::BotPaddle(Coordinates position, Coordinates velocity) : Paddle(position, velocity)
{
}

void BotPaddle::Tracking(Coordinates ball)
{
	//std::cout << "BEFORE" << std::endl;
	//std::cout << "x: " << position.x << std::endl;
	//std::cout << "y: " << position.y << std::endl << std::endl;
	position.x = W_WIDTH - 50;
	if (ball.y > position.y)
	{
		//std::cout << "Going down " << std::endl;
		this->velocity.y = P_AI_SPEED;
	}

	else if (ball.y <= position.y)
	{
		//std::cout << "Going up " << std::endl;
		this->velocity.y = -P_AI_SPEED;
	}
	//std::cout << "AFTER" << std::endl;
	//std::cout << "x: " << position.x << std::endl;
	//std::cout << "y: " << position.y << std::endl;
}