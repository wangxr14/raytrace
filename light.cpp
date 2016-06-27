#include "light.h"
#define ran() ( double( rand() % 32768 ) / 32768 )
#define eps 1e-6

Light::Light()
{
	sample = rand();
	next = NULL;
}

void Light::Input( std::string var , std::stringstream& fin )
{
	if ( var == "color=" ) color.Input( fin );
}

void PointLight::Input( std::string var , std::stringstream& fin )
{
	if ( var == "O=" ) O.Input( fin );
	Light::Input( var , fin );
}

double PointLight::CalnShade( Vector3 C , Object* object_head , int shade_quality )
{
	Vector3 V = O - C;
	double dist = V.Module();
	for ( Object* now = object_head ; now != NULL ; now = now->GetNext() )
		if ( now->Collide( C , V ) && ( now->crash.dist < dist ) ) return 0;
	return 1;
}

void AreaLight::Input( std::string var , std::stringstream& fin )
{
	if ( var == "O=" ) O.Input( fin );
	if ( var == "Dx=" ) Dx.Input( fin );
	if ( var == "Dy=" ) Dy.Input( fin );
	Light::Input( var , fin );
}

bool AreaLight::Collide( Vector3 ray_O , Vector3 ray_V )
{
	ray_V = ray_V.GetUnitVector();
	Vector3 N = ( Dx * Dy ).GetUnitVector();
	double d = N.Dot( ray_V );
	if ( fabs( d ) < eps ) return false;
	double l = ( N * O.Dot( N ) - ray_O ).Dot( N ) / d;
	if ( l < eps ) return false;

	Vector3 C = ( ray_O + ray_V * l ) - O;
	if ( fabs( Dx.Dot( C ) ) > Dx.Dot( Dx ) ) return false;
	if ( fabs( Dy.Dot( C ) ) > Dy.Dot( Dy ) ) return false;

	crash_dist = l;
	return true;
}
//软阴影的实现
//随机选取shade计算阴影系数
double AreaLight::CalnShade( Vector3 C , Object* object_head , int shade_quality )
{
	int shade = 0;
	
	for ( int k = 0 ; k < shade_quality ; k++ ) 
	{
		Vector3 V = O - C + Dx * ( ( ran() ) / 2 ) + Dy * ( ( ran()  ) / 2 );
		double dist = V.Module();

		for ( Object* now = object_head ; now != NULL ; now = now->GetNext() )
			if ( now->Collide( C , V ) && ( now->crash.dist < dist ) ) {
				shade++;
				break;
			}
	}
	
	return 1 - ( double ) shade / (  shade_quality );
}
