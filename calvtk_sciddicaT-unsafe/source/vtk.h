#ifndef VTK_H
#define VTK_H
extern "C"{
#include "sciddicaT.h"
}
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>
#include <vtkPoints.h>
#include <vtkDataSet.h>
#include <vtkImageData.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkLookupTable.h>
#include <stdio.h>

#define VTK_SP(type,name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

extern vtkSmartPointer<vtkImageData> topology;
extern vtkSmartPointer<vtkDataSetMapper> topologyMapper;

void vtkDataSetLoad();
void vtkRenderDefinition();
void vtkFinalization();

void KeypressCallbackFunction (vtkObject* caller, long unsigned int eventId, void* clientData, void* callData );

#endif
// lookuptable for gray an red scale
// update the scalar value of cell and render
