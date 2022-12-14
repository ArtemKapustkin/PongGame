#include"Ball.hpp"
#include <chrono>

Ball::Ball(Coordinates position, Coordinates velocity) : position(position), velocity(velocity)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);
	rect.w = B_WIDTH;
	rect.h = B_HEIGHT;
}

void Ball::Update(float dt)
{
	position += velocity * dt;
}

void Ball::Draw(SDL_Renderer* renderer)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);

	SDL_RenderFillRect(renderer, &rect);
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