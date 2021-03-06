#include "calvtkscalarbar.h"

calvtkScalarBar* calvtkScalarBar::New()
{
    return new calvtkScalarBar();
}

void calvtkScalarBar::Delete()
{
    vtkScalarBarActor::Delete();
}

void calvtkScalarBar::SetTitle(std::string title)
{
    vtkScalarBarActor::SetTitle(title.c_str());
}

char* calvtkScalarBar::GetTitle()
{
    return vtkScalarBarActor::GetTitle();
}

void calvtkScalarBar::SetLayer(calvtkLayer2D * const layer)
{
    this->layer = layer;
    SetTitle(layer->GetName());
    vtkScalarBarActor::SetLookupTable(layer->GetLookupTable());
}

calvtkLayer2D* calvtkScalarBar::GetLayer()
{
    return layer;
}

void calvtkScalarBar::SetPosition(double x,double y)
{
    vtkScalarBarActor::SetPosition(x,y);
}

void calvtkScalarBar::SetSize(double width, double height)
{
    vtkScalarBarActor::SetWidth(width);
    vtkScalarBarActor::SetHeight(height);
}

void calvtkScalarBar::SetOrientationToHorizontal()
{
    vtkScalarBarActor::SetOrientationToHorizontal();
    double width = GetWidth();
    double height = GetHeight();
    SetSize(height,width);
}

void calvtkScalarBar::SetOrientationToVertical()
{
    vtkScalarBarActor::SetOrientationToVertical();
}

calvtkScalarBar::calvtkScalarBar():vtkScalarBarActor()
{

}

calvtkScalarBar::~calvtkScalarBar()
{

}
