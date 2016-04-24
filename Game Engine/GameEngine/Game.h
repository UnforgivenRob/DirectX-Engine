#ifndef GAME_H
#define GAME_H

#include "engine.h"
#include "Timer.h"

class Game :
	public Engine
{
public:
	Game( HINSTANCE hInstance );
	~Game();

	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void UnloadContent() override;

private:
	Timer FullTimer;
	Timer intervalTimer;
	float color[4];
};
#endif

