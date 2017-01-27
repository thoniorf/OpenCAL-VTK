#include "calvtkaxes.h"

calvtkAxes* calvtkAxes::New()
{
    return new calvtkAxes();
}

void calvtkAxes::Delete()
{
    delete this;
}

void calvtkAxes::Translate(double x, double y, double z)
{
    Transform->Translate(x,y,z);
}

void calvtkAxes::Scale(double x, double y, double z)
{
    Transform->Scale(x,y,z);
}

void calvtkAxes::RotateWXYZ(double angle, double x, double y, double z)
{
    Transform->RotateWXYZ(angle,x,y,z);
}

calvtkAxes::calvtkAxes()
{
    Transform = vtkTransform::New();
}

calvtkAxes::~calvtkAxes()
{
    Transform->Delete();
}
