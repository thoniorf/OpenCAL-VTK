#ifndef CALVTKAXES_H
#define CALVTKAXES_H

#include <vtkAxesActor.h>
#include <vtkTransform.h>
class calvtkAxes : protected vtkAxesActor
{
friend class calvtkRender2D;
public:
    static calvtkAxes* New();
    void Delete();

    void Translate(double x, double y, double z);
    void Scale(double x, double y, double z);
    void RotateWXYZ(double angle, double x, double y, double z);

protected:
    calvtkAxes();
    ~calvtkAxes();
};
#endif // CALVTKAXES_H
