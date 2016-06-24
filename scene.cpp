#include "scene.h"
#include <iostream>
using namespace std;

Scene::Scene()
{
	object_head = NULL;
	light_head = NULL;
	background_color = Color();
	camera = new Camera;
}


Scene::~Scene()
{
	while(object_head != NULL)
	{
		Object* next_head = object_head -> GetNext();
		if(object_head -> GetMaterial() -> texture != NULL)
			delete object_head -> GetMaterial() -> texture;
		delete object_head;
		object_head = next_head;
	}

	while(light_head != NULL)
	{
		Light* next_head = light_head -> GetNext();
		delete light_head;
		light_head = next_head;
	}

	delete camera;
}

Object* Scene::GetObjectHead()
{
	return object_head;
}

Light* Scene::GetLightHead()
{
	return light_head;
}

Color Scene::GetBGColor()
{
	return background_color;
}

Camera* Scene::GetCamera()
{
	return camera;
}

void Scene::CreateScene(string file)
{
	srand(0);
	std::ifstream fin(file.c_str());

	string obj;
	while ( fin >> obj )
	{
		Object* new_object = NULL;
		Light* new_light = NULL;

		if ( obj == "object" )
		{
			std::string type; fin >> type;
			if ( type == "sphere" ) new_object = new Sphere;
			if ( type == "plane" ) new_object = new Plane;
			if ( new_object != NULL )
			{
				new_object->SetNext( object_head );
				object_head = new_object;
			}
		}
		else if ( obj == "light" )
		{
			std::string type; fin >> type;
			if ( type == "point" ) new_light = new PointLight;
			if ( type == "area" ) new_light = new AreaLight;
			if ( new_light != NULL )
			{
				new_light->SetNext( light_head );
				light_head = new_light;
			}
		}
		else if ( obj != "background" && obj != "camera" ) continue;

		fin.ignore( 1024 , '\n' );
		
		std::string order;
		while ( getline( fin , order , '\n' ) )
		{
			std::stringstream fin2( order );
			std::string var; fin2 >> var;
			if ( var == "end" ) break;

			if ( obj == "background" && var == "color=" ) background_color.Input( fin2 );
			if ( obj == "object" && new_object != NULL ) new_object->Input( var , fin2 );
			if ( obj == "light" && new_light != NULL ) new_light->Input( var , fin2 );
			if ( obj == "camera" ) camera->Input( var , fin2 );
		}
	}

	camera->Initialize();
}

Object* Scene::FindNearestObject(Vector3 ray_O, Vector3 ray_V)
{
	Object* ret = NULL;

	for ( Object* now = object_head ; now != NULL ; now = now->GetNext() )
		if ( now->Collide( ray_O , ray_V ) && ( ret == NULL || now->crash.dist < ret->crash.dist ) ) ret = now;

	return ret;
}

Light* Scene::FindNearestLight(Vector3 ray_O, Vector3 ray_V)
{
	Light* ret = NULL;

	for ( Light* now = light_head ; now != NULL ; now = now->GetNext() )
		if ( now->Collide( ray_O , ray_V ) && ( ret == NULL || now->crash_dist < ret->crash_dist ) ) ret = now;

	return ret;
}
