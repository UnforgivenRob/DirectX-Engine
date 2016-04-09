#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
public:
	Engine();
	~Engine();

	void run();

	virtual void Initialize() = 0;
	virtual void LoadContent() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void UnloadContent() = 0;

private:
	void PreLoad();
	void PreInit();
	void ClearBuffers();
	void SwapBuffers();

	bool bRun;
};

#endif

