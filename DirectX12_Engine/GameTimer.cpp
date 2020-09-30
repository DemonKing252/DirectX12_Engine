#include "GameTimer.h"


GameTimer::GameTimer()
{
}

GameTimer::~GameTimer()
{
}

float GameTimer::GetDeltaTime() const
{
	return deltaTime;
}

float GameTimer::GetTimeNow() const
{
	return time;
}

void GameTimer::Reset()
{
	time = 0.0f;
}

void GameTimer::Start()
{
	running = true;
}

void GameTimer::Stop()
{
	running = false;
}

void GameTimer::SetDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}

void GameTimer::SetTime(float time)
{
	this->time = time;
}

bool GameTimer::Running() const
{
	return running;
}
