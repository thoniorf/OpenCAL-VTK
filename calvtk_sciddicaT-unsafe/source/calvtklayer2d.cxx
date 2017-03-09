#include "calvtklayer2d.h"

double calvtkLayer2D::z_offset = 0;
double calvtkLayer2D::scaleFactor = 1;

calvtkLayer2D* calvtkLayer2D::New()
{
    return new calvtkLayer2D();
}

void calvtkLayer2D::Delete()
{
    polydata->Delete();
    mapper->Delete();
    actor->Delete();
    celldata->Delete();
}

calvtkLayer2D::calvtkLayer2D():model(NULL),substate(NULL),cols(0),rows(0),cellsize(0),type(CALVTK_MULTI),calvtkAbstractLayer()
{
    polydata = vtkPolyData::New();

    celldata = vtkDoubleArray::New();

    mapper = vtkPolyDataMapper::New();
    mapper->SetInputData(polydata);
    mapper->SetScalarModeToUseCellData();
    mapper->GlobalImmediateModeRenderingOn();

    actor = vtkActor::New();
    actor->SetMapper(mapper);

    p_value = 0.0;
}

void calvtkLayer2D::SetCALModel2D(CALModel2D* const model)
{
    this->model = model;
    this->rows = model->rows;
    this->cols = model->columns;
}

CALModel2D* calvtkLayer2D::GetCALModel2D()
{
    return model;
}

void calvtkLayer2D::SetCALSubstate2D(CALSubstate2Dr* const substate)
{
    this->substate = substate;
    ComputeExtremes();
    mapper->SetScalarRange(extremes);
}

CALSubstate2Dr* calvtkLayer2D::GetCALSubState2D()
{
    return substate;
}

void calvtkLayer2D::SetCellSize(int size)
{
    this->cellsize = size;
}

int calvtkLayer2D::GetCellSize()
{
    return cellsize;
}

int calvtkLayer2D::GetRows()
{
    return rows;
}

int calvtkLayer2D::GetCols()
{
    return cols;
}
void calvtkLayer2D::SetPvalue(double p_value)
{
    this->p_value = p_value;
}

double* calvtkLayer2D::GetBounds(double* bounds)
{
    polydata->GetBounds(bounds);
}

void calvtkLayer2D::SetLayerType(calvtkLayerType type)
{
    this->type = type;
}

void calvtkLayer2D::WarpScalar()
{
    vtkCellDataToPointData* cellToPoint = vtkCellDataToPointData::New();
    cellToPoint->SetInputData(polydata);
    cellToPoint->Update();

    polydata->GetPointData()->DeepCopy(cellToPoint->GetPolyDataOutput()->GetPointData());
    vtkWarpScalar* warper = vtkWarpScalar::New();
    warper->SetInputData(polydata);
    warper->SetScaleFactor(scaleFactor);
    warper->UseNormalOn();
    warper->SetNormal(0,0,1);
    warper->Update();

    polydata->DeepCopy(warper->GetOutput());

    cellToPoint->Delete();
    warper->Delete();
}

void calvtkLayer2D::CopyDataSetFromLayer(calvtkLayer2D * const layer)
{
    polydata->DeepCopy(layer->GetPolyData());
}


void calvtkLayer2D::GenerateDataSet()
{
    // geometric and topological source
    vtkPlaneSource* sourcePlane = vtkPlaneSource::New();
    sourcePlane->SetResolution(cols,rows);
    sourcePlane->Update();

    // transformations
    vtkTransform* transformation = vtkTransform::New();
    transformation->Translate(.0,.0,z_offset);
    transformation->RotateX(180);
    transformation->Scale(cols*cellsize,rows*cellsize,1);

    // actual tranformation filter, it's doing the transformation
    vtkTransformPolyDataFilter* transformFilter = vtkTransformPolyDataFilter::New();
    transformFilter->SetInputConnection(sourcePlane->GetOutputPort());
    transformFilter->SetTransform(transformation);
    transformFilter->Update();

    polydata->DeepCopy(transformFilter->GetOutput());

    // dataset z offset stacking
    z_offset += Z_OFFSET_STEP;

    transformFilter->Delete();
    transformation->Delete();
    sourcePlane->Delete();
}

void calvtkLayer2D::GenerateScalarValues()
{
    celldata->SetName(name.c_str());
    celldata->SetNumberOfComponents(1);
    celldata->SetNumberOfTuples(rows*cols);
    int tupleIndex = 0;
    double value = 0;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            value = calGet2Dr(model,substate,i,j);
            celldata->SetComponent(tupleIndex,0,value);
            tupleIndex++;
        }
    }
    polydata->GetCellData()->SetScalars(celldata);

}

void calvtkLayer2D::UpdateScalarValues()
{
    double value = 0;
    double tupleIndex = 0;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            value = calGet2Dr(model,substate,i,j);
            if(value >= p_value)
            {
                celldata->SetComponent(tupleIndex,0,calGet2Dr(model,substate,i,j));
            }
            tupleIndex++;
        }
    }


}
void calvtkLayer2D::UpdateScalarValues(int index_start, int index_end)
{
    double value = 0;
    for(int tupleIndex = index_start; tupleIndex < index_end; tupleIndex++)
    {
            value = calGet2Dr(model,substate,tupleIndex/rows,tupleIndex%cols);
            if(value != 0)
            {
                celldata->SetComponent(tupleIndex,0,calGet2Dr(model,substate,tupleIndex/rows,tupleIndex%cols));
            }
    }
}

void calvtkLayer2D::ComputeExtremes()
{
    int i, j;
    double *m = &extremes[0];
    double *M = &extremes[1];
    for (i=0; i<rows; i++)
        for (j=0; j<cols; j++)
            if (calGet2Dr(model,substate,i,j) > 0)
            {
                *m = calGet2Dr(model,substate,i,j);
                *M = calGet2Dr(model,substate,i,j);
            }
    for (i=0; i<rows; i++)
        for (j=0; j<cols; j++)
        {
            if (*M < calGet2Dr(model,substate,i,j) && calGet2Dr(model,substate,i,j) > 0)
                *M = calGet2Dr(model,substate,i,j);
            if (*m > calGet2Dr(model,substate,i,j) && calGet2Dr(model,substate,i,j) > 0)
                *m = calGet2Dr(model,substate,i,j);
        }
}

void calvtkLayer2D::Update()
{
    if(type != CALVTK_SINGLE_LAYER) {
        ComputeExtremes();

        lookuptable->SetScalarRange(extremes);
        lookuptable->Build();

        mapper->SetScalarRange(extremes);

        UpdateScalarValues();

    }
}

calvtkLookupTable* calvtkLayer2D::GetLookupTable()
{
    return lookuptable;
}

vtkPolyData* calvtkLayer2D::GetPolyData()
{
    return polydata;
}

vtkPolyDataMapper* calvtkLayer2D::GetMapper()
{
    return mapper;
}

vtkActor* calvtkLayer2D::GetActor()
{
    return actor;
}

void calvtkLayer2D::SetLookupTable(calvtkLookupTable* const lookuptable)
{
    this->lookuptable = lookuptable;
    mapper->SetLookupTable(lookuptable);
}
