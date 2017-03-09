#include "calvtkoutline.h"
calvtkOutline* calvtkOutline::New()
{
    return new calvtkOutline();
}
void calvtkOutline::Delete()
{
    mapper->Delete();
    actor->Delete();
}

void calvtkOutline::SetLayer(calvtkLayer2D * const layer)
{
    this->layer = layer;
    SetInputData(layer->GetPolyData());
    mapper->SetInputConnection(this->GetOutputPort());
    actor->SetMapper(mapper);
}

calvtkLayer2D* calvtkOutline::GetLayer()
{
    return layer;
}

void calvtkOutline::SetColor(double r, double g, double b)
{
    actor->GetProperty()->SetColor(r,g,b);
}

calvtkOutline::calvtkOutline(){
    mapper = vtkPolyDataMapper::New();
    actor = vtkActor::New();
}

calvtkOutline::~calvtkOutline()
{

}
