#ifndef VERT_STRUCTS_H
#define VERT_STRUCTS_H

#include <vector>
#include <algorithm>

//RWW

struct Vert_Skinning
{
	float vx;
	float vy;
	float vz;
	float nx;
	float ny;
	float nz;
	int	bones[4];
	float weights[4];
};

struct Vert_Slim
{
	float vx;
	float vy;
	float vz;
	float nx;
	float ny;
	float nz;
};

struct Vert
{
	float vx;
	float vy;
	float vz;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
	int slimIndex;

	bool operator == (const Vert& inV)
	{
		return	  (this->vx == inV.vx) 
				&&(this->vy == inV.vy)
				&&(this->vz == inV.vz)
				&&(this->s == inV.s)
				&&(this->t == inV.t)
				&&(this->nx == inV.nx)
				&&(this->ny == inV.ny)
				&&(this->nz == inV.nz);
	}

	bool operator != (const Vert& inV)
	{
		return	  (this->vx != inV.vx) 
				||(this->vy != inV.vy)
				||(this->vz != inV.vz)
				||(this->s != inV.s)
				||(this->t != inV.t)
				||(this->nx != inV.nx)
				||(this->ny != inV.ny)
				||(this->nz != inV.nz);
	}

	bool operator < (const Vert& inV)
	{
		if( this->vx < inV.vx)
		{
			return true;
		}
		else if (this->vx > inV.vx)
		{
			return false;
		}

		if( this->vy < inV.vy)
		{
			return true;
		}
		else if (this->vy > inV.vy)
		{
			return false;
		}

		if( this->vz < inV.vz)
		{
			return true;
		}
		else if (this->vz > inV.vz)
		{
			return false;
		}

		if( this->s < inV.s)
		{
			return true;
		}
		else if (this->s > inV.s)
		{
			return false;
		}

		if( this->t < inV.t)
		{
			return true;
		}
		else if (this->t > inV.t)
		{
			return false;
		}

		if( this->nx < inV.nx)
		{
			return true;
		}
		else if (this->nx > inV.nx)
		{
			return false;
		}

		if( this->ny < inV.ny)
		{
			return true;
		}
		else if (this->ny > inV.ny)
		{
			return false;
		}

		if( this->nz < inV.nz)
		{
			return true;
		}
		else if (this->nz > inV.nz)
		{
			return false;
		}
		else
		{
			return false;
		}
	}
};

struct Poly
{
	int i1;
	int i2;
	int i3;
};

struct Vert_Final
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
};

struct Vert_Final_Skinned
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
	float weight0;
	float weight1;
	float weight2;
	float weight3;
	int bone0;
	int bone1;
	int bone2;
	int bone3;
};



#endif