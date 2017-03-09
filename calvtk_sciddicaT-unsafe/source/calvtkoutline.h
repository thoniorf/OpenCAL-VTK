#ifndef CALVTKOUTLINE_H
#define CALVTKOUTLINE_H

#include "calvtklayer2d.h"

#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
class calvtkOutline : protected vtkOutlineFilter
{
    friend class calvtkRender2D;
public:
    static calvtkOutline* New();
    void Delete();

    void SetLayer(calvtkLayer2D* const layer);
    calvtkLayer2D* GetLayer();

    void SetColor(double r, double g, double b);
protected:
    calvtkOutline();
    ~calvtkOutline();
private:
    calvtkLayer2D* layer;

    vtkPolyDataMapper* mapper;
    vtkActor* actor;
};

#endif // CALVTKOUTLINE_H
