#pragma once
typedef class GameTimer
{
public:
	GameTimer();
	GameTimer(const GameTimer& rhs) = delete;
	~GameTimer();

	float GetDeltaTime() const;
	float GetTimeNow() const;
	
	void Reset();
	void Start();
	void Stop();

	void SetDeltaTime(float deltaTime);
	void SetTime(float time);

	bool Running() const;

	// Don't touch this variable!
	float frameStart;

	// Don't touch this variable!
	float frameEnd;
private:
	bool running;
	float deltaTime;
	float time;
	float frameTime;


} GameTimer;

