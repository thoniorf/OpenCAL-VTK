#include "calvtklookuptable.h"
calvtkLookupTable * calvtkLookupTable::New()
{
    return new calvtkLookupTable();
}
void calvtkLookupTable::Delete()
{
    Delete();
}

void calvtkLookupTable::SetHueRange(double hueMin, double hueMax)
{
    vtkLookupTable::SetHueRange(hueMin,hueMax);
}

void calvtkLookupTable::SetSaturationRange(double saturationMin, double saturationMax)
{
    vtkLookupTable::SetSaturationRange(saturationMin,saturationMax);
}

void calvtkLookupTable::SetValueRange(double valueMin, double valueMax)
{
    vtkLookupTable::SetValueRange(valueMin,valueMax);
}

void calvtkLookupTable::SetAboveRangeColor(double red, double green, double blue, double alpha)
{
    vtkLookupTable::UseAboveRangeColorOn();
    vtkLookupTable::SetAboveRangeColor(red,green,blue,alpha);
}

void calvtkLookupTable::SetBelowRangeColor(double red, double green, double blue, double alpha)
{
    vtkLookupTable::UseBelowRangeColorOn();
    vtkLookupTable::SetBelowRangeColor(red,green,blue,alpha);
}

void calvtkLookupTable::SetScalarRange(double minScalar, double maxScalar)
{
    vtkLookupTable::SetRange(minScalar,maxScalar);
}

void calvtkLookupTable::SetScalarRange(double values[])
{
    SetScalarRange(values[0],values[1]);
}

void calvtkLookupTable::Build()
{
    vtkLookupTable::Build();
}

calvtkLookupTable::calvtkLookupTable()
{

}

calvtkLookupTable::~calvtkLookupTable()
{

}
