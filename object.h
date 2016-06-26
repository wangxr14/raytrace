#pragma once
#include "color.h"
#include "vector3.h"
#include "bmp.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>


class Material
{
public:
    Material();
	Color color, absor;
	double refl, refr, diff, spec;//反射、折射、漫反射、镜面漫反射
	double rindex;
	double drefl;
	Bmp* texture;
	void Input(std::string var, std::stringstream&);
};

struct Crash
{
	Vector3 N, C;//相交的点、相交的这根光线
	double dist;//距离
	bool front;
};

class Object
{
protected:
	int sample;
	Material* material;
	Object* next;
public:
	Object(void);
	Object(const Object&);
	~Object();
	int GetSample();
	Material* GetMaterial();
	Object* GetNext();
	void SetNext(Object*);
	Crash crash;
	virtual void Input(std::string var, std::stringstream&);
	virtual bool Collide(Vector3 ray_O, Vector3 ray_D) = 0;
	virtual Color GetTexture() = 0;
	virtual Object* ObjectCopy() = 0;
};

class Sphere : public Object
{
public:
	Sphere();
	Vector3 O;
	Vector3 De, Dc;
	double R;
	void Input(std::string var, std::stringstream&);
	bool Collide(Vector3 ray_O, Vector3 ray_V);
	Color GetTexture();
	Object* ObjectCopy();
};

class Plane : public Object
{
public:
	Vector3 N;
	Vector3 Dx, Dy;
	double R;
	void Input(std::string var, std::stringstream&);
	bool Collide(Vector3 ray_O, Vector3 ray_V);
	Color GetTexture();
	Object* ObjectCopy();
};
