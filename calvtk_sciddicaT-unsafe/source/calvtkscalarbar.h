#ifndef CALVTKSCALARBAR_H
#define CALVTKSCALARBAR_H

#include "calvtklayer2d.h"

#include <vtkScalarBarActor.h>

class calvtkScalarBar : protected vtkScalarBarActor
{
friend class calvtkRender2D;
public:
    static calvtkScalarBar* New();
    void Delete();

    void SetTitle(std::string title);
    char* GetTitle();

    void SetLayer(calvtkLayer2D * const layer);
    calvtkLayer2D* GetLayer();

    void SetPosition(double x, double y);
    void SetSize(double width,double height);

    void SetOrientationToHorizontal ();
    void SetOrientationToVertical ();

protected:
    calvtkScalarBar();
    ~calvtkScalarBar();
private:
    calvtkLayer2D* layer;
};
#endif // CALVTKSCALARBAR_H
