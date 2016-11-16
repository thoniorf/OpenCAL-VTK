#ifndef VTK_H
#define VTK_H
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
// ImageData because the sciddicaT's cellular space is 2-dimensional matrix like
#include <vtkImageData.h>
// LookupTable for topological value ( gray gradient) and debris value ( red gradient)
#include<vtkLookupTable.h>

#define VTK_SP(type,name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#endif
// update the scalar value of cell and render
