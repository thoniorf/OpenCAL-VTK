#ifndef SCIDDICATVTKRENDER_H
#define SCIDDICATVTKRENDER_H

extern "C" {
#include <OpenCAL/cal2D.h>
#include <OpenCAL/cal2DIO.h>
}

#include "sciddicaT.h"

#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCellData.h>
#include <vtkCellDataToPointData.h>
#include <vtkCommand.h>
#include <vtkDoubleArray.h>
#include <vtkGenericDataArray.h>
#include <vtkPlaneSource.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>
#include <vtkWarpScalar.h>
#include <vtkClipPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <stdio.h>
#define VTK_DEF(type,name) vtkSmartPointer<type> name
#define VTK_INI(type,name) name = vtkSmartPointer<type>::New()

class SciddicaTVTKRender
{
public:
    SciddicaTVTKRender();
    ~SciddicaTVTKRender();
    void Update();
    void Render();
    void Start();
    void RenderInizialization();
    void Inizialize();
    void CreateTimerEvent(unsigned long duration, void(*callbackCall)(vtkObject *, unsigned long, void *, void *));
    void SetScaleFactor(double scaleValue);

private:
    void ComputeExtremes(CALModel2D* ca2D,CALSubstate2Dr *substate,double *m,double *M);
    void ComputeColors();
    void ComputeElevation();
    void ClipPolyData();

    double h_min,h_Max,z_min,z_Max;
    double scaleFactor;

    VTK_DEF(vtkPolyData,polyoutput);
    VTK_DEF(vtkPlaneSource,source);
    VTK_DEF(vtkUnsignedCharArray,colorData);

    VTK_DEF(vtkPolyDataMapper,mapper);
    VTK_DEF(vtkActor,actor);
    VTK_DEF(vtkRenderer,renderer);
    VTK_DEF(vtkRenderWindow,renderWindow);
    VTK_DEF(vtkRenderWindowInteractor,renderWindowInteractor);
};

#endif // SCIDDICATVTKRENDER_H
