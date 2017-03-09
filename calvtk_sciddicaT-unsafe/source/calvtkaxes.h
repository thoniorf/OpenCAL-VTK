#ifndef CALVTKAXES_H
#define CALVTKAXES_H

#include "calvtkrun.h"

#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkOrientationMarkerWidget.h>

class calvtkAxes : protected vtkAxesActor
{
public:
    static calvtkAxes* New();
    void Delete();

    void Translate(double x, double y, double z);
    void Scale(double x, double y, double z);
    void RotateWXYZ(double angle, double x, double y, double z);

    void SetInteractor(calvtkRun* run);
    void SetViewport(double, double, double, double);
    void SetEnabled(int value);
    void InteractiveOn();
    void InteractiveOff();

protected:
    calvtkAxes();
    ~calvtkAxes();

    vtkOrientationMarkerWidget * widget;
private :
    double outlineRed = 0.93;
    double outlineGreen = 0.57;
    double outlineBlue = 0.13;
};
#endif // CALVTKAXES_H
