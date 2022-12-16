#pragma once
const int W_WIDTH = 1920;
const int W_HEIGHT = 1090;
const int B_WIDTH = 15;
const int B_HEIGHT = 15;
const int P_WIDTH = 15;
const int P_HEIGHT = 100;
const float P_AI_SPEED = 0.2f;
const float BALL_SPEED = 0.4f;
const float PADDLE_SPEED = 0.6f;
const int T_WIDTH = 200;
const int T_HEIGHT = 25;

enum Buttons
{
	PaddleOneUp,
	PaddleOneDown,
	PaddleTwoUp,
	PaddleTwoDown,
};

enum class CollisionType
{
	None,
	Top,
	Middle,
	Bottom,
	Left,
	Right
};

class Contact
{
public:
	CollisionType type;
	float penetration;
};