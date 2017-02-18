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
#include <vtkMultiThreader.h>

#include <vector>

#define Z_OFFSET_STEP 100

enum calvtkLayerType { CALVTK_SINGLE_LAYER, CALVTK_MULTI };

struct Cell {
    int i;
    int j;
};

class calvtkLayer2D;

struct WorkerQaud{
    int index_start = 0;
    int index_end = 0;
    calvtkLayer2D* layer;
};

class calvtkLayer2D : public calvtkAbstractLayer
{
public:
    static calvtkLayer2D * New();
    void Delete();

    void SetCALModel2D(CALModel2D* const model);
    CALModel2D* GetCALModel2D();

    void SetCALSubstate2D(CALSubstate2Dr* const substate);
    CALSubstate2Dr* GetCALSubState2D();

    void SetCellSize(int size);
    int GetCellSize();

    int GetRows();
    int GetCols();

    void SetPvalue(double p_value);

    double* GetBounds(double* bounds);

    void SetLayerType(calvtkLayerType type);

    void GenerateDataSet();

    void GenerateScalarValues();
    void UpdateScalarValues();
    void UpdateScalarValues(int index_start,int index_end);

    void Update();

    calvtkLookupTable* GetLookupTable();

    vtkPolyData* GetPolyData();
    vtkPolyDataMapper* GetMapper();
    vtkActor* GetActor();

    void SetLookupTable(calvtkLookupTable* const lookuptable);

protected:

    calvtkLayer2D();
    ~calvtkLayer2D();

    void AddActiveCell(int i, int j);
    void ComputeActiveCell(int i,int j);
    bool FindActiveCell(int i,int j);
    void ComputeExtremes();

private:
    static VTK_THREAD_RETURN_TYPE LayerUpdateFunction(void * args);
    static double z_offset;
    double p_value;

    std::vector<Cell> active_cells;

    CALModel2D* model;
    CALSubstate2Dr* substate;
    int cols, rows, cellsize;
    calvtkLayerType type;

    calvtkLookupTable* lookuptable;

    vtkDoubleArray* celldata;
    vtkPolyData* polydata;
    vtkPolyDataMapper* mapper;
    vtkActor* actor;

    vtkMultiThreader* worker;


};
#endif // CALVTKLAYER2D_H
