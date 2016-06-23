#pragma once
#include "vector3.h"
#include "color.h"
#include "bmp.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <vector>



class Camera
{
	Vector3 O, N, Dx, Dy;//OΪ�������ĵ㣬NΪ��Ļƽ�淨����,Dx��DyΪ��Ļ��x������y����
	double lens_W, lens_H;
	int W, H;
	vector <vector< Color>> data;
	double shade_quality;
	double drefl_quality;
	double sample_dist;
public:

	Camera();
	~Camera(void);
	Vector3 GetO() { return O; }
	int GetW() { return W; }
	int GetH() { return H; }
	void SetColor( int i , int j , Color color ) { data[i][j] = color; }
	double GetShadeQuality() { return shade_quality; }
	double GetDreflQuality() { return drefl_quality; }
	double GetSampleDist() { return sample_dist; }


	Vector3 Emit(double, double);
	void Initialize();
	void Input(string var, stringstream&);
	void Output(string);
};
