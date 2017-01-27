#ifndef CALVTKLAYER2D_H
#define CALVTKLAYER2D_H

extern "C" {
#include <OpenCAL/cal2D.h>
#include <OpenCAL/cal2DIO.h>
}

#include "calvtkabstractlayer.h"
#include "calvtklookuptable.h"

#include <vtkActor.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#define Z_OFFSET_STEP 100

enum calvtkLayerType { CALVTK_SINGLE_LAYER, CALVTK_MULTI };

class calvtkLayer2D : public calvtkAbstractLayer
{
public:
    calvtkLayer2D();
    ~calvtkLayer2D();

    void SetCALModel2D(CALModel2D* const model);
    CALModel2D* GetCALModel2D();

    void SetCALSubstate2D(CALSubstate2Dr* const substate);
    CALSubstate2Dr* GetCALSubState2D();

    void SetCellSize(int size);
    int GetCellSize();

    void SetLayerType(calvtkLayerType type);

    void GenerateDataSet();

    void GenerateScalarValues();
    void UpdateScalarValues();

    void Update();

    calvtkLookupTable* GetLookupTable();

    vtkPolyData* GetPolyData();
    vtkPolyDataMapper* GetMapper();
    vtkActor* GetActor();

    void SetLookupTable(calvtkLookupTable* const lookuptable);

protected:
    void ComputeExtremes();

private:
    static double z_offset;

    CALModel2D* model;
    CALSubstate2Dr* substate;
    int cols, rows, cellsize;
    calvtkLayerType type;

    calvtkLookupTable* lookuptable;

    vtkDoubleArray* celldata;
    vtkPolyData* polydata;
    vtkPolyDataMapper* mapper;
    vtkActor* actor;


};
#endif // CALVTKLAYER2D_H
