#ifndef GAME_H
#define GAME_H

#include "engine.h"
#include "Timer.h"
#include "MathEngine.h"
#include "FileSystem.h"

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
	virtual void ClearBuffers() override;
	virtual void UnloadContent() override;
	virtual void CheckInput();

	virtual void OnKeyDown( WPARAM keyState ) override;
	virtual void OnKeyUp( WPARAM keyState ) override;

private:
	FileHandle fh;
	Timer FullTimer;
	Timer intervalTimer;
	Vect bgColor;
	
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
};
#endif

