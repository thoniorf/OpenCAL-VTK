#include "calvtklayer2d.h"

double calvtkLayer2D::z_offset =-Z_OFFSET_STEP;

calvtkLayer2D::calvtkLayer2D():model(NULL),substate(NULL),cols(0),rows(0),cellsize(0),type(CALVTK_MULTI),calvtkAbstractLayer()
{
    polydata = vtkPolyData::New();

    mapper = vtkPolyDataMapper::New();
    mapper->SetScalarModeToUseCellData();
    mapper->GlobalImmediateModeRenderingOn();

    actor = vtkActor::New();
    actor->SetMapper(mapper);
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

void calvtkLayer2D::SetLayerType(calvtkLayerType type)
{
    this->type = type;
}


void calvtkLayer2D::GenerateDataSet()
{
    // dataset z offset stacking
    z_offset += Z_OFFSET_STEP;

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

    polydata->DeepCopy(sourcePlane->GetOutput());

    transformFilter->Delete();
    transformation->Delete();
    sourcePlane->Delete();
}

void calvtkLayer2D::GenerateScalarValues()
{
    celldata = vtkDoubleArray::New();
    celldata->SetName(name.c_str());
    celldata->SetNumberOfComponents(1);
    celldata->SetNumberOfTuples(rows*cols);

    int tupleIndex = 0;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            double tuple = calGet2Dr(model,substate,i,j);
            celldata->SetTuple(tupleIndex,&tuple);
            tupleIndex++;
        }

    }
    polydata->GetCellData()->SetScalars(celldata);

}

void calvtkLayer2D::UpdateScalarValues()
{
    int tupleIndex = 0;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            double tuple = calGet2Dr(model,substate,i,j);
            celldata->SetTuple(tupleIndex,&tuple);
            tupleIndex++;
        }

    }
    celldata->Modified();

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
