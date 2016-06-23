#include "bmp.h"

#define Eps 1e-6

Bmp::~Bmp()
{
    cvDestroyAllWindows();
    Destroy();
}
int Bmp::Width() const
{
    if (image == NULL) return 0;
    else return image->width;
}
int Bmp::Height() const
{
    if (image == NULL) return 0;
    else return image->height;
}
string Bmp::Filename() const
{
    return file;
}
void Bmp::Create(const int &width, const int &height)
{
    image = cvCreateImage(cvSize(width, height), 8, 3);
    cvZero(image);
}
bool Bmp::Load(const string file)
{
    this->file = file;
    image = cvLoadImage(file.data());
    if (image == NULL)
    {
        cout << "Cannot load file: " << file << endl;
        return 1;
    }
    return 0;
}
void Bmp::Save(const string file) const
{
    cvSaveImage(file.data(), image);
}
void Bmp::Show(const string title) const
{
    cvNamedWindow(title.data(), 1);
    cvShowImage(title.data(), image);
    cvWaitKey(0);
}
Color Bmp::GetPixel(const int x, const int y) const
{
    CvScalar color = cvGet2D(image, x, y);
    return Color((double)color.val[2] / 255, (double)color.val[1] / 255, (double)color.val[0] / 255);
}
Color Bmp::GetPixel(const double u, const double v, const bool repeat) const
{
    double _u = (u - floor(u)) * image->height;
    double _v = (v -  floor(v)) * image->width;
    int u1 = floor(_u + Eps);
    int u2 = u1 + 1;
    int v1 = floor(_v + Eps);
    int v2 = v1 + 1;
    _u = u2 - _u;
    _v = v2 - _v;
    if (u1 >= image->height) u1 = image->height - 1;
    if (u1 < 0)
    {
        if (repeat) u1 = image->height - 1;
        else u1 = 0;
    }
    if (u2 < 0) u2 = 0;
    if (u2 >= image->height)
    {
        if (repeat) u2 = 0;
        else u2 = image->height - 1;
    }
    if (v1 >= image->width) v1 = image->width - 1;
    if (v1 < 0)
    {
        if (repeat) v1 = image->width - 1;
        else v1 = 0;
    }
    if (v2 < 0) v2 = 0;
    if (v2 >= image->width)
    {
        if (repeat) v2 = 0;
        else v2 = image->width - 1;
    }
    Color color;
    color += GetPixel(u1, v1) * _u * _v;
    color += GetPixel(u1, v2) * _u * (1 - _v);
    color += GetPixel(u2, v1) * (1 - _u) * _v;
    color += GetPixel(u2, v2) * (1 - _u) * (1 - _v);
    return color;
}
void Bmp::SetPixel(const int x, const int y, const Color &c) const
{
    cvSet2D(image, x, y, CV_RGB(c.r * 255, c.g * 255, c.b * 255));
}
void Bmp::Destroy()
{
    if (image != NULL) cvReleaseImage(&image);
}
