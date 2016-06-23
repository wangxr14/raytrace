#include "object.h"
#define eps 1e-6
#define PI 3.1415926535897932384626

#include <iostream>
using namespace std;

Material::Material()
{
	color = Color();
	absor = Color();
	refl = 0;
	refr = 0;
	diff = 0;
	spec = 0;
	rindex = 0;
	texture = NULL;
}

void Material::Input(std::string var, std::stringstream& fin)
{
	if(var == "color=") color.Input(fin);
	if(var == "absor=") absor.Input(fin);
	if(var == "refl=") fin >> refl;
	if(var == "refr=") fin >> refr;
	if(var == "diff=") fin >> diff;
	if(var == "spec=") fin >> spec;
	if(var == "rindex=") fin >> rindex;
	if(var == "texture=")
	{
		std::string file;
		fin >> file;
		texture = new Bmp;
		texture->Load( file );
	}
}

Object::Object()
{
	sample = rand();
	material = new Material;
	next = NULL;
}

Object::Object(const Object& object)
{
	*this = object;
	material = new Material;
	*material = *object.material;
}

Object::~Object()
{
	delete material;
}

int Object::GetSample()
{
	return sample;
}

Material* Object::GetMaterial()
{
	return material;
}

Object* Object::GetNext()
{
	return next;
}

void Object::SetNext(Object* object)
{
	next = object;
}

void Object::Input(std::string var, std::stringstream& fin)
{
	material -> Input(var, fin);
}

Sphere::Sphere() : Object()
{
	De = Vector3(0, 0, 1);
	Dc = Vector3(0, 1, 0);
}

void Sphere::Input(std::string var, std::stringstream& fin)
{
	if(var == "O=") O.Input(fin);
	if(var == "R=") fin >> R;
	if(var == "De=") De.Input(fin);
	if(var == "Dc=") Dc.Input(fin);
	Object::Input(var, fin);
}

bool Sphere::Collide(Vector3 ray_O, Vector3 ray_V)
{
	ray_V = ray_V.GetUnitVector();
	Vector3 P = ray_O - O;
	double b = -P.Dot( ray_V );
	double det = b * b - P.Module2() + R * R;

	if ( det > eps )
	{
		det = sqrt( det );
		double x1 = b - det  , x2 = b + det;

		if ( x2 < eps ) return false;
		if ( x1 > eps )
		{
			crash.dist = x1;
			crash.front = true;
		} 
		else
		{
			crash.dist = x2;
			crash.front = false;
		} 
	}
	else return false;

	crash.C = ray_O + ray_V * crash.dist;
	crash.N = ( crash.C - O ).GetUnitVector();
	if ( crash.front == false ) crash.N = -crash.N;
	return true;
}

Color Sphere::GetTexture()
{
	Vector3 I = ( crash.C - O ).GetUnitVector();
	double a = acos( -I.Dot( De ) );
	double b = acos( std::min( std::max( I.Dot( Dc ) / sin( a ) , -1.0 ) , 1.0 ) );
	double u = a / PI , v = b / 2 / PI;
	if ( I.Dot( Dc * De ) < 0 ) v = 1 - v;
	return material->texture->GetPixel( u , v , false );
}

Object* Sphere::ObjectCopy()
{
	Sphere* sph = new Sphere(*this);
	return sph;
}

void Plane::Input(std::string var, std::stringstream& fin)
{
	if(var == "N=") N.Input(fin);
	if(var == "Dx=") Dx.Input(fin);
	if(var == "Dy=") Dy.Input(fin);
	if(var == "R=") fin >> R;
	Object::Input(var, fin);
}

bool Plane::Collide(Vector3 ray_O, Vector3 ray_V)
{
	ray_V = ray_V.GetUnitVector();
	N = N.GetUnitVector();
	double d = N.Dot( ray_V );
	if ( fabs( d ) < eps ) return false;
	double l = ( N * R - ray_O ).Dot( N ) / d;
	if ( l < eps ) return false;

	crash.dist = l;
	crash.front = ( d < 0 );
	crash.C = ray_O + ray_V * crash.dist;
	crash.N = ( crash.front ) ? N : -N;
	return true;
}

Color Plane::GetTexture()
{
	double u = crash.C.Dot( Dx ) / Dx.Module2();
	double v = crash.C.Dot( Dy ) / Dy.Module2();
	return material->texture->GetPixel( u , v , false );
}

Object* Plane::ObjectCopy()
{
	Plane* pla = new Plane(*this);
	return pla;
}
