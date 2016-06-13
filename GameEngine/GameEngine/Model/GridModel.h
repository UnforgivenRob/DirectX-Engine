#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include "Primitive.h"

class GridModel : public Primitive
{
public:
	GridModel( Model_ID id, int width, int length, int step );
	~GridModel();
private:
	virtual void initModel() override;
	//prevent from being called
	GridModel();
	GridModel( GridModel& inModel );
	GridModel& operator= ( GridModel& inModel );

	int width;
	int length;
	int step;
};


#endif