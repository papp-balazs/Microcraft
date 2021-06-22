#pragma once

#include "WinConfig.h"
#include "Window.h"
#include "Timer.h"
#include "Camera.h"

#include <memory>

class Game
{
public:
	Game(size_t width, size_t height);
	Game(Game&) = delete;
	Game& operator=(Game&) = delete;
	~Game();
	int Start();
private:
	void DoFrame();
	Window wnd;
	Timer timer;
	Camera cam;
	bool exit = false;
	bool showCursor = true;
	float skyIntensity = 1.0f;
};