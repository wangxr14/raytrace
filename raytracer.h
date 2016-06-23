#pragma once

#include "scene.h"
#include "bmp.h"
#include <string>
#include <stdlib.h>
#include <iostream>

class Raytracer
{
	std::string input , output;
	Scene scene;
	Camera* camera;
	vector < vector<int> > sample;
	Color CalnDiffusion(Object* obj, int* hash);
	Color CalnReflection(Object* obj, Vector3 ray_V, int dep, int* hash);
	Color CalnRefraction(Object* obj, Vector3 ray_V, int dep, int* hash);
	Color RayTracing(Vector3 ray_O, Vector3 ray_V, int dep, int* hash);
public:
	void SetInput(std::string file) {input = file;}
	void SetOutput(std::string file) {output = file;}
	void Run();
};
