#pragma once
class Coordinates
{
public:
	Coordinates() : x(0.0f), y(0.0f)
	{}

	Coordinates(float x, float y) : x(x), y(y)
	{}

	Coordinates operator+(Coordinates const& rhs)
	{
		return Coordinates(x + rhs.x, y + rhs.y);
	}

	Coordinates& operator+=(Coordinates const& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	Coordinates operator*(float rhs)
	{
		return Coordinates(x * rhs, y * rhs);
	}

	float getX()
	{
		return x;
	}

	float getY()
	{
		return y;
	}

	void setX(float xc)
	{
		x = xc;
	}

	void setY(float yc)
	{
		y = yc;
	}

private:
	float x, y;
};