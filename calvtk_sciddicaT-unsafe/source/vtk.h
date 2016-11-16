#ifndef VTK_H
#define VTK_H
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#define VTK_SP(type,name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#endif
