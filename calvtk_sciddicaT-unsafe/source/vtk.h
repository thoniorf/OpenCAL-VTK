#ifndef VTK_H
#define VTK_H
extern "C"{
#include "sciddicaT.h"
}
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkDataSet.h>
#include <vtkImageData.h>
#include<vtkLookupTable.h>
#include<stdio.h>

#define VTK_SP(type,name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

void vtkDataSetLoad(struct CALSubstate2Dr* Q);
void vtkRenderDefinition();
void vtkFinalization();

#endif
// lookuptable for gray an red scale
// update the scalar value of cell and render
