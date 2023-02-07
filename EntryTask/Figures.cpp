#include"Figures.hpp"
#include <chrono>

Figure::Figure(Coordinates position, Coordinates velocity) : position(position), velocity(velocity)
{
	this->rect.x = static_cast<int>(position.getX());
	this->rect.y = static_cast<int>(position.getY());
	this->rect.w = 10;
	this->rect.h = 10;
}

void Figure::Update(float dt)
{
	this->position += this->velocity * dt;
}

void Figure::Draw(SDL_Renderer* renderer)
{
	this->rect.x = static_cast<int>(this->position.getX());
	this->rect.y = static_cast<int>(this->position.getY());

	SDL_RenderFillRect(renderer, &rect);
}

void Paddle::Update(float dt)
{
	this->position += this->velocity * dt;

	if (this->position.getY() < 0)
	{
		// Restrict to top of the screen
		this->position.setY(0.f);
	}
	else if (this->position.getY() > (W_HEIGHT - P_HEIGHT))
	{
		// Restrict to bottom of the screen
		this->position.setY(W_HEIGHT - P_HEIGHT);
	}
}

void Paddle::Draw(SDL_Renderer* renderer)
{
	this->rect.y = static_cast<int>(this->position.getY());

	SDL_RenderFillRect(renderer, &rect);
}

void Paddle::SetStartCoordinates(Coordinates position, Coordinates velocity)
{
	this->rect.x = static_cast<int>(position.getX());
	this->rect.y = static_cast<int>(position.getY());
}

Paddle::Paddle(Coordinates position, Coordinates velocity) : Figure(position, velocity)
{
	this->rect.w = P_WIDTH;
	this->rect.h = P_HEIGHT;
}

void Ball::CollideWithPaddle(Contact const& contact)
{
	this->position.setX(this->position.getX() + contact.penetration);
	this->velocity.setX(-this->velocity.getX());

	if (contact.type == CollisionType::Top)
	{
		this->velocity.setY(- 0.75f * BALL_SPEED);
	}
	else if (contact.type == CollisionType::Bottom)
	{
		this->velocity.setY(0.75f * BALL_SPEED);
	}
}

void Ball::CollideWithWall(Contact const& contact)
{
	if ((contact.type == CollisionType::Top) || (contact.type == CollisionType::Bottom))
	{
		this->position.setY(this->position.getY() + contact.penetration);
		this->velocity.setY(-this->velocity.getY());
	}
	else if (contact.type == CollisionType::Left)
	{
		this->position.setX(W_WIDTH / 2.0f);
		this->position.setY(W_HEIGHT / 2.0f);
		this->velocity.setX(BALL_SPEED);
		this->velocity.setY(((rand() / (RAND_MAX + 0.5)) - 0.5)* BALL_SPEED);
	}
	else if (contact.type == CollisionType::Right)
	{
		this->position.setX(W_WIDTH / 2.0f);
		this->position.setY(W_HEIGHT / 2.0f);
		this->velocity.setX(-BALL_SPEED);
		this->velocity.setY(((rand() / (RAND_MAX + 0.5)) - 0.5) * BALL_SPEED);
	}
}

Ball::Ball(Coordinates position, Coordinates velocity) : Figure(position, velocity)
{
	this->rect.w = B_WIDTH;
	this->rect.h = B_HEIGHT;
}

BotPaddle::BotPaddle(Coordinates position, Coordinates velocity) : Paddle(position, velocity)
{
}

void BotPaddle::Tracking(Coordinates ball)
{
	this->position.setX(W_WIDTH - 50);
	if (ball.getY() > position.getY())
	{
		this->velocity.setY(P_AI_SPEED);
	}
	else if (ball.getY() <= position.getY())
	{
		this->velocity.setY(-P_AI_SPEED);
	}
}