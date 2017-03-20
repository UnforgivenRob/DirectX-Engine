#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include "Material.h"
#include "GraphicsObject.h"

class RenderGroup
{
public:
	RenderGroup();
	RenderGroup(Material_ID mat_id);
	~RenderGroup();

	void add(GraphicsObject* graphObj);
	void remove(GraphicsObject* graphObj);

	void draw(Matrix& proj_matrix);

private:
	Material* material;
	ManagerList * graphObj_List;

	int nodeCnt;

};

#endif

