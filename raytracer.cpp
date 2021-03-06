#include "raytracer.h"

#define ran() (double(rand() % 32768) / 32768)
#define eps 1e-6
#define SPEC_POWER 20
#define MAX_DREFL_DEP 2
#define MAX_RAYTRACING_DEP 5
#define HASH_FAC 7
#define HASH_MOD 10000007

//计算漫反射光线
Color Raytracer::CalnDiffusion( Object* obj , int* hash )
{
	Color color = obj->GetMaterial()->color;
	//如果有贴图
	if ( obj->GetMaterial()->texture != NULL ) color = obj->GetTexture();
	Color ret = color * scene.GetBGColor() * obj->GetMaterial()->diff;
	for ( Light* light = scene.GetLightHead() ; light != NULL ; light = light->GetNext() )
	{
		//计算阴影系数
		double shade = light->CalnShade( obj->crash.C , scene.GetObjectHead() , scene.GetCamera()->GetShadeQuality() );
		if ( shade < eps ) continue;
		
		Vector3 R = ( light->GetO() - obj->crash.C ).GetUnitVector();
		double dot = R.Dot( obj->crash.N );
		if ( dot > eps )
		{
			if ( hash != NULL && light->IsPointLight() ) *hash = ( *hash + light->GetSample() ) & HASH_MOD;

			if ( obj->GetMaterial()->diff > eps )
			{
				double diff = obj->GetMaterial()->diff * dot * shade;
				ret += color * light->GetColor() * diff;
			}
			if ( obj->GetMaterial()->spec > eps )
			{
				double spec = obj->GetMaterial()->spec * pow( dot , SPEC_POWER ) * shade;
				ret += color * light->GetColor() * spec;
			}
		}
	}

	return ret;
}
//计算反射光线
Color Raytracer::CalnReflection( Object* obj , Vector3 ray_V , int dep , int* hash )
{
	ray_V = ray_V.Reflect( obj->crash.N );

	if ( obj->GetMaterial()->drefl < eps || dep > MAX_DREFL_DEP )
		return RayTracing( obj->crash.C , ray_V , dep + 1 , hash ) * obj->GetMaterial()->color * obj->GetMaterial()->refl;

	Vector3 Dx = ray_V * Vector3( 1 , 0 , 0 );
	if ( Dx.IsZeroVector() ) Dx = Vector3( 1 , 0 , 0 );
	Vector3 Dy = ray_V * Dx;
	Dx = Dx.GetUnitVector() * obj->GetMaterial()->drefl;
	Dy = Dy.GetUnitVector() * obj->GetMaterial()->drefl;

	Color ret;
	for ( int k = 0 ; k < 16 * scene.GetCamera()->GetDreflQuality() ; k++ )
	{
		double x , y;
		do
		{
			x = ran() * 2 - 1;
			y = ran() * 2 - 1;
		} while ( x * x + y * y > 1 );
		x *= obj->GetMaterial()->drefl;
		y *= obj->GetMaterial()->drefl;

		ret += RayTracing( obj->crash.C , ray_V + Dx * x + Dy * y , dep + MAX_DREFL_DEP , NULL );
	}

	ret = ret * obj->GetMaterial()->color * obj->GetMaterial()->refl / ( 16 * scene.GetCamera()->GetDreflQuality() );
	return ret;
}
//计算折射光线
Color Raytracer::CalnRefraction( Object* obj , Vector3 ray_V , int dep , int* hash )
{
	double n = obj->GetMaterial()->rindex;
	if ( obj->crash.front ) n = 1 / n;
	
	ray_V = ray_V.Refract( obj->crash.N , n );
	
	Color rcol = RayTracing( obj->crash.C , ray_V , dep + 1 , hash );
	if ( obj->crash.front ) return rcol * obj->GetMaterial()->refr;
	Color absor = obj->GetMaterial()->absor * -obj->crash.dist;
	Color trans = Color( exp( absor.r ) , exp( absor.g ) , exp( absor.b ) );
	return rcol * trans * obj->GetMaterial()->refr;
}

//进行光线追踪的函数
Color Raytracer::RayTracing( Vector3 ray_O , Vector3 ray_V , int dep , int* hash )
{
	if ( dep > MAX_RAYTRACING_DEP ) return Color();

	Color ret;
	Object* nearest_object = scene.FindNearestObject( ray_O , ray_V );
	Light* nearest_light = scene.FindNearestLight( ray_O , ray_V );
	//有最近的光源直接照射的情况下
	if ( nearest_light != NULL && ( nearest_object == NULL || nearest_light->crash_dist < nearest_object->crash.dist ) )
	{
		if ( hash != NULL ) *hash = ( *hash + nearest_light->GetSample() ) % HASH_MOD;
		ret += nearest_light->GetColor();
		Color col = nearest_light->GetColor();
	}
	//有最近的物体，则计算反射折射漫反射的光
	if ( nearest_object != NULL )
	{
		if ( hash != NULL ) *hash = ( *hash + nearest_object->GetSample() ) % HASH_MOD;
		Object* object = nearest_object->ObjectCopy();
		if ( object->GetMaterial()->diff > eps || object->GetMaterial()->spec > eps ) ret += CalnDiffusion( object , hash );
		if ( object->GetMaterial()->refl > eps ) ret += CalnReflection( object , ray_V , dep , hash );
		if ( object->GetMaterial()->refr > eps ) ret += CalnRefraction( object , ray_V , dep , hash );
		delete object;
	}

	if ( hash != NULL ) *hash = ( *hash * HASH_FAC ) % HASH_MOD;
	ret.Confine();
	return ret;
}

void Raytracer::Run()
{
	camera = scene.GetCamera();
	//设置场景
	scene.CreateScene( input );
	cout << "creat scene" << endl;
	Vector3 ray_O = camera->GetO();
	//初始化sample
	sample.resize(camera->GetH());
	for ( int i = 0 ; i < sample.size() ; i++ )
	{
		sample[i].resize(camera->GetW());
	}
	//对每一个点，都发射一条光线进行追踪计算
	for (int i = 0; i < sample.size(); i++)
	{
		for (int j = 0; j < sample[i].size(); j++)
		{

			Vector3 ray_V = camera->Emit(i, j);
			Color color = RayTracing(ray_O, ray_V, 1, &sample[i][j]);
			camera->SetColor(i, j, color);
			/*Color color;
			for (int k = 0; k < count; k++)
			{
				int x = rand() % 2*R -R;
				int y = rand() % 2*R -R;
				
				Vector3 ray1 = camera->Emit(i+x, j+y);
				Vector3 ray2 = camera->focal_point - camera->GetO();

				color += RayTracing(ray_v, ray_o, 1, &sample[i][j]);	
			}
			camera->SetColor(i, j, color/count);*/
			/*if (camera->aperture < eps) 
			{
				Vector3 ray_V = camera->Emit(i, j);
				Color color = RayTracing(ray_O, ray_V, 1, &sample[i][j]);
				camera->SetColor(i, j, color);
			}
			else //景深
			{
				int dofSample = camera->dofSample;
				Color color;
				for (int k = 0; k < 100; k++) 
				{
					Vector3 dof_O, dof_V;
					camera->DofEmit(i, j, &dof_O, &dof_V);
					color += RayTracing(dof_O, dof_V, 1, NULL) / dofSample;
				}
				
				camera->SetColor(i, j, color);
			}*/
		}
		//cout << "i:" << i << endl;
	}
	cout << "emit" << endl;
	//进行抗锯齿处理，根据sample把像素拆分为九小格
	/*for (int i = 0; i < sample.size(); ++i)
	{
		for (int j = 0; j < sample[i].size(); j++)
		{
			if ((i == 0 || sample[i][j] == sample[i - 1][j]) && (i == sample.size() - 1 || sample[i][j] == sample[i + 1][j]) &&
				(j == 0 || sample[i][j] == sample[i][j - 1]) && (j == sample[i].size() - 1 || sample[i][j] == sample[i][j + 1])) 
				continue;

			Color color;
			for (int r = -1; r <= 1; r++)
				for (int c = -1; c <= 1; c++)
				{
					Vector3 ray_V = camera->Emit(i + (double)r / 3, j + (double)c / 3);
					color += RayTracing(ray_O, ray_V, 1, NULL) / 9;
				}
			camera->SetColor(i, j, color);
		}
		//cout << "i2:" << i << endl;
	}*/
	
	camera->Output(output);
}
