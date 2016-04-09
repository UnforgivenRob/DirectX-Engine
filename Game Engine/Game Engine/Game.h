#ifndef GAME_H
#define GAME_H

#include "engine.h"

class Game :
	public Engine
{
public:
	Game();
	~Game();

	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void UnloadContent() override;
};
#endif

