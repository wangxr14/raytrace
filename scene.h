#pragma once

#include "object.h"
#include "light.h"
#include "color.h"
#include "camera.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Scene
{
	Object* object_head;
	Light* light_head;
	Color background_color;
	Camera* camera;
public:
	Scene();
	~Scene();
	Object* GetObjectHead();
	Light* GetLightHead();
	Color GetBGColor();
	Camera* GetCamera();
	void CreateScene(std::string file);
	Object* FindNearestObject(Vector3 ray_O, Vector3 ray_V);
	Light* FindNearestLight(Vector3 ray_O, Vector3 ray_V);
};
