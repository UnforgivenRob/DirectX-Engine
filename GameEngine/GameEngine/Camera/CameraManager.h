#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "ManagerList.h"
#include "Camera.h"

#include "Time.h"

class CameraManager
{
public:
	
	static void Activate();
	static void create(Camera_ID id, const Vect& pos, const Vect& target, const Vect& up);
	static void add(Camera* inCamera, Camera_ID id);
	static void update(Time currentTime);
	static void remove(Camera_ID id);
	static void clear();
	static Camera* get(Camera_ID id);
	static void setActive(Camera_ID id);
	static Camera* getActive();
	static void Deactivate();
private:
	CameraManager();
	~CameraManager();
	
	Camera_ID activeCam;
	static CameraManager instance;
	static ManagerList*  manager;

};

#endif // ! CameraMANAGER_H
