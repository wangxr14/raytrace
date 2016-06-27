#ifndef __Ray_Tracing__Bmp__
#define __Ray_Tracing__Bmp__

#include "color.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include <string>
#include <iostream>
using namespace std;

//利用opencv来显示图像、保存图像
class Bmp
{
    IplImage *image;
    string file;
public:
    int Width() const;
    int Height() const;
    string Filename() const;
    void Create(const int &width, const int &height);
    bool Load(const string file);
    void Save(const string file) const;
    void Show(const string title = "") const;
    Color GetPixel(const double u, const double v, const bool repeat) const;
    Color GetPixel(const int x, const int y) const;
    void SetPixel(const int x, const int y, const Color &c) const;
  
};

#endif /* defined(__Ray_Tracing__Bmp__) */
