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
	Vector3 O, N, Dx, Dy;//OΪ�ӵ㣬NΪ��Ļƽ�淨����,Dx��DyΪ��Ļ��x������y����
	double lens_W, lens_H;
	int W, H;
	vector <vector< Color>> data;//��Ļ���ص�
	double shade_quality;
	double drefl_quality;
	double sample_dist;
	
public:
	double dofSample, aperture, focal_len;//��Ȧ������
	Camera();
	~Camera();
	Vector3 GetO() { return O; }
	int GetW() { return W; }
	int GetH() { return H; }
	void SetColor( int i , int j , Color color ) { data[i][j] = color; }
	double GetShadeQuality() { return shade_quality; }
	double GetDreflQuality() { return drefl_quality; }
	double GetSampleDist() { return sample_dist; }


	Vector3 Emit(double, double);
	void DofEmit(double i, double j, Vector3* dof_O, Vector3* dof_V);
	void Initialize();
	void Input(string var, stringstream&);
	void Output(string);
};
