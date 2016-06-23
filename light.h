#pragma once

#include "color.h"
#include "object.h"
#include "vector3.h"
#include <string>
#include <sstream>
#include <math.h>

class Light
{
	int sample;
	Color color;
	Light* next;
public:
	double crash_dist;

	Light();
	
	virtual bool IsPointLight() = 0;
	int GetSample() { return sample; }
	Color GetColor() { return color; }
	Light* GetNext() { return next; }
	void SetNext( Light* light ) { next = light; }

	virtual void Input( std::string , std::stringstream& );
	virtual Vector3 GetO() = 0;
	virtual bool Collide( Vector3 ray_O , Vector3 ray_V ) = 0;
	virtual double CalnShade( Vector3 C , Object* object_head , int shade_quality ) = 0;
};

class PointLight : public Light
{
	Vector3 O;
public:
	Vector3 GetO() {return O;}
	bool IsPointLight() {return true;}
	void Input(std::string var, std::stringstream&);
	bool Collide(Vector3 ray_O, Vector3 ray_V) {return false;}
	double CalnShade(Vector3 C, Object* object_head, int shade_quality);
};

class AreaLight : public Light
{
	Vector3 O;
	Vector3 Dx, Dy;
public:
	Vector3 GetO() {return O;}
	virtual bool IsPointLight() {return false;}
	virtual void Input(std::string var, std::stringstream&);
	virtual bool Collide(Vector3 ray_O, Vector3 ray_V);
	virtual double CalnShade(Vector3 C, Object* object_head, int shade_quality);
};
