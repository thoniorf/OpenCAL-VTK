#ifndef VTK_H
#define VTK_H
extern "C"{
#include "sciddicaT.h"
}
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkPlaneSource.h>
#include <vtkCamera.h>
#include <stdio.h>

#define VTK_SP(type,name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
extern vtkSmartPointer<vtkRenderWindow> renderWindow;
void vtkDataSetLoad();
void vtkDataSetScalarsSet();
void vtkRenderDefinition();
void vtkFinalization();

void KeypressCallbackFunction (vtkObject* caller, long unsigned int eventId, void* clientData, void* callData );

#endif
