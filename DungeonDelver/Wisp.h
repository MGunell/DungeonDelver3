#pragma once
#include "LTexture.h"
class Wisp {
public:

	Wisp()
	{
		animXFactor = 4;
		animYFactor = 5;

		xangle = 0.0f;
		yangle = 0.0f;
		
		x = 300;
		y = 300;
	};

	void move()
	{
		xangle += (2 * pi / 360)*animXFactor;
		yangle += (2 * pi / 360)*animYFactor;
		x = 64 * (xangle);
		y = 64 * (yangle);
	};

	void render(SDL_Renderer* gRenderer, SDL_Rect& camera)
	{
		SDL_SetRenderDrawColor(gRenderer, 255, 120, 120, 255);
		SDL_RenderDrawPoint(gRenderer, x, y);
	}

	int animXFactor, animYFactor;
	float xangle, yangle;
	int x, y;
	int pi = 3.14;

};