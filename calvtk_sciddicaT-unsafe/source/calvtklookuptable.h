#ifndef CALVTKLOOKUPTABLE_H
#define CALVTKLOOKUPTABLE_H

#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>

class calvtkLookupTable : protected vtkLookupTable
{
friend class calvtkLayer2D;
friend class calvtkScalarBar;
public:
    static calvtkLookupTable* New();
    void Delete();

    void SetHueRange(double hueMin, double hueMax);
    void SetSaturationRange(double saturationMin, double saturationMax);
    void SetValueRange(double valueMin, double valueMax);
    void SetAlphaRange(double alphaMin, double alphaMax);

    void SetAboveRangeColor(double red, double green, double blue, double alpha);
    void SetBelowRangeColor(double red, double green, double blue, double alpha);

    void SetScalarRange(double minScalar, double maxScalar);
    void SetScalarRange(double values[]);
    void Build();
protected:
    calvtkLookupTable();
    ~calvtkLookupTable();

};
#endif // CALVTKLOOKUPTABLE_H
