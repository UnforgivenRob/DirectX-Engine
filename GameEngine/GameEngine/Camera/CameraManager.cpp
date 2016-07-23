#include "CameraManager.h"

ManagerList* CameraManager::manager = 0;
CameraManager CameraManager::instance = CameraManager();

CameraManager::CameraManager()
{
	if(manager != 0)
	{
		delete manager;
	}
	this->manager = new ManagerList();
	activeCam = None;
}

CameraManager::~CameraManager()
{
	delete manager;
	manager = 0;
}

void CameraManager::Activate()
{
	if(instance.manager == 0)
	{
		manager = new ManagerList();
	}
}

void CameraManager::Deactivate()
{
	delete manager;
	manager = 0;
}


void CameraManager::create(Camera_ID id, const Vect& pos, const Vect& target, const Vect& up)
{
	CameraManager::add(new Camera(id, pos, target, up), id);
}

void CameraManager::add(Camera* inCam, Camera_ID id)
{
	instance.manager->add((Node*)inCam, id);
	if(None == instance.activeCam)
	{
		instance.activeCam = id;
	}
}
void CameraManager::update(Time currentTime)
{
	Node* tmp = instance.manager->head;

	while(tmp != 0)
	{
		((Camera*)tmp)->update(currentTime);
		tmp = tmp->getNext();
	}
}

void CameraManager::remove(Camera_ID id)
{
	instance.manager->remove(id);
}

void CameraManager::clear()
{
	instance.manager->clear();
}

Camera* CameraManager::get(Camera_ID id)
{
	return (Camera*)instance.manager->get(id);
}

void CameraManager::setActive(Camera_ID id)
{
	instance.activeCam = id;
}

Camera* CameraManager::getActive()
{
	return CameraManager::get(instance.activeCam);
}