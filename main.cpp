#include<cv.h>
#include<highgui.h>
#include<opencv2\opencv.hpp>
#include"raytracer.h"
using namespace std;


int main()
{
	Raytracer* raytracer = new Raytracer;
	raytracer->SetInput("scene.txt");
	raytracer->SetOutput("picture.bmp");
	raytracer->Run();

	return 0;
}