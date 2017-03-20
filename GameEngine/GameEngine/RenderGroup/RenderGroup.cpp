#include "RenderGroup.h"
#include "ManagerList.h"
#include "MaterialManager.h"

RenderGroup::RenderGroup()
	: material( nullptr ), nodeCnt(0)
{
	graphObj_List = new ManagerList();
}

RenderGroup::RenderGroup(Material_ID mat_id)
	: nodeCnt(0)
{
	material = MaterialManager::get(mat_id);
	graphObj_List = new ManagerList();
}

RenderGroup::~RenderGroup()
{
	graphObj_List->clear();
	delete graphObj_List;
}

void RenderGroup::add(GraphicsObject * graphObj)
{
	graphObj_List->add(graphObj, nodeCnt);
	nodeCnt++;
}

void RenderGroup::remove(GraphicsObject * graphObj)
{
	graphObj_List->remove(graphObj->getID());
}

void RenderGroup::draw(Matrix & proj_matrix)
{

	GraphicsObject* obj = (GraphicsObject*)graphObj_List->get(0);

	while (obj != NULL)
	{
		obj->draw(proj_matrix);

		obj = (GraphicsObject*)obj->getNext();
	}
}
