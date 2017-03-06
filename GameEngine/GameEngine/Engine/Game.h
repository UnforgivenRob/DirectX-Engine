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

	//overload operator new and delete to ensure aligned 16
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		return _mm_free(p);
	}

private:
	FileHandle fh;
	Timer FullTimer;
	Timer intervalTimer;
	Vect bgColor;
	
	ComPtr<ID3D12Device> Device;
};
#endif

