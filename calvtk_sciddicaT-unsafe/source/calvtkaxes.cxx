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

void calvtkAxes::SetInteractor(calvtkRun *run)
{
    widget->SetInteractor(run->renderInteractor);
}

void calvtkAxes::SetViewport(double arg0, double arg1, double arg2, double arg3)
{
    widget->SetViewport(arg0,arg1,arg2,arg3);
}

void calvtkAxes::SetEnabled(int value)
{
    widget->SetEnabled(value);
}

void calvtkAxes::InteractiveOn()
{
    widget->InteractiveOn();
}

void calvtkAxes::InteractiveOff()
{
    widget->InteractiveOff();
}

calvtkAxes::calvtkAxes()
{
    Transform = vtkTransform::New();

    widget = vtkOrientationMarkerWidget::New();
    widget->SetOutlineColor(outlineRed,outlineGreen,outlineBlue);
    widget->SetOrientationMarker(this);

}

calvtkAxes::~calvtkAxes()
{
    Transform->Delete();
}
