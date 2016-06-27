#include "camera.h"

#define STD_LENS_WIDTH 0.88
#define STD_LENS_HEIGHT 0.88
#define STD_IMAGE_WIDTH 420
#define STD_IMAGE_HEIGHT 420
#define STD_SHADE_QUALITY 4
#define STD_DREFL_QUALITY 4
#define STD_SAMPLE_DIST 1
#define STD_FOCUS_LEN 4
#define STD_APERTURE 0.8;
#define ran() ( double( rand() % 32768 ) / 32768 )

Camera::Camera()
{
	O = Vector3( 0 , 0 , 0 );
	N = Vector3( 0 , 1 , 0 );
	lens_W = STD_LENS_WIDTH;
	lens_H = STD_LENS_HEIGHT;
	W = STD_IMAGE_WIDTH;
	H = STD_IMAGE_HEIGHT;
	shade_quality = STD_SHADE_QUALITY;
	drefl_quality = STD_DREFL_QUALITY;
	sample_dist = STD_SAMPLE_DIST;

	aperture = STD_APERTURE;
	focal_len = STD_FOCUS_LEN;
}

Camera::~Camera() 
{
	
}

Vector3 Camera::Emit(double i, double j)
{
	return N + Dy * ( 2 * i / H  ) + Dx * ( 2 * j / W );
}

void Camera::DofEmit(double i, double j, Vector3* dof_O, Vector3* dof_V) {
	Vector3 focalPoint = O + Emit(i, j) * focal_len;
	double x, y;
	do {
		x = ran() * 2 - 1;
		y = ran() * 2 - 1;
	} while (x * x + y * y > 1);
	*dof_O = O + Dx * aperture * x + Dy * aperture * y;
	*dof_V = (focalPoint - *dof_O).GetUnitVector();
}

void Camera::Initialize()
{
	N = N.GetUnitVector();
	Dx = Dx.GetUnitVector();
	Dy = Dx * N;
	Dx = Dx * lens_W / 2;
	Dy = Dy * lens_H / 2;

	data.resize(H);
	for ( int i = 0 ; i < H ; i++ )
		data[i].resize(W);
}

void Camera::Input(string var, stringstream& fin)
{
	if(var == "O=") O.Input(fin);
	if(var == "N=") N.Input(fin);
	if(var == "Dx=") Dx.Input(fin);
	if(var == "lens_W=") fin >> lens_W;
	if(var == "lens_H=") fin >> lens_H;
	if(var == "image_W=") fin >> W;
	if(var == "image_H=") fin >> H;
	if(var == "shade_quality=") fin >> shade_quality;
	if(var == "drefl_quality=") fin >> drefl_quality;
	if(var == "sample_dist=") fin >> sample_dist;
}

void Camera::Output(string output)
{
	Bmp *bmp = new Bmp;
	bmp->Create( W, H );

	for ( int i = 0 ; i < H ; i++ )
		for ( int j = 0 ; j < W ; j++ )
			bmp->SetPixel( i , j , data[i][j] );
	bmp->Save(output);
	bmp->Show("picture");
}
