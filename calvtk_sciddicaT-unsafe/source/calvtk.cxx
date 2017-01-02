#include "calvtk.h"
void CALVTKRender::calvtkAddAxes()
{
    axes = vtkAxesActor::New();

    if(datasets.size() > 0){
        double bounds[6];
        datasets[0]->GetBounds(bounds);
        VTK_DEF(vtkTransform,transform);
        VTK_INI(vtkTransform,transform);
        transform->Translate(bounds[0], bounds[2], bounds[4]);
        transform->Scale(rows,cols,(rows+cols)/2);
        axes->SetUserTransform(transform);
    }
}

void CALVTKRender::calvtkGenerateNextLayerPolyData()
{
    // geometric and topological source
    vtkPlaneSource* plane = vtkPlaneSource::New();
    plane->SetResolution(cols,rows);
    plane->Update();
    // transformations
    vtkTransform* transformation = vtkTransform::New();
    transformation->Translate(.0,.0,z_offset);
    transformation->RotateX(180);
    transformation->Scale(cols*cellsize,rows*cellsize,1);

    // dataset z offset stacking
    z_offset += Z_OFFSET_STEP;

    // actual tranformation filter, it's doing the transformation
    vtkTransformPolyDataFilter* transformFilter = vtkTransformPolyDataFilter::New();
    transformFilter->SetInputConnection(plane->GetOutputPort());
    transformFilter->SetTransform(transformation);
    transformFilter->Update();

    // push the output dataset in our vector
    vtkPolyData *resultPolyData = vtkPolyData::New();
    resultPolyData->DeepCopy(transformFilter->GetOutput());

    this->datasets.push_back(resultPolyData);
    plane->Delete();
    transformation->Delete();
    transformFilter->Delete();
}

void CALVTKRender::calvtkGenerateAllLayersPolyData()
{
    for(int i = 0; i < layers.size(); i++)
    {
        calvtkGenerateNextLayerPolyData();
    }
}

void CALVTKRender::calvtkAddLayer(CALSubstate2Dr *substate)
{
    layers.push_back(substate);
}
void CALVTKRender::calvtkAddLayer(CALSubstate2Dr *substate, std::string layerName)
{
    calvtkAddLayer(substate);
    layersNames.push_back(layerName);
}

int CALVTKRender::calvtkGetLayerId(CALSubstate2Dr *substate)
{
    if(layers.size() == 0)
        return -1;
    for(int i = 0; i < layers.size(); i++)
    {
        if(substate == layers[i])
            return i;
    }
}

void CALVTKRender::calvtkGenerateLayerScalarValues(int layer_id)
{
    int dataset_id = layer_id;
    extremes.push_back(new double[2]{0});
    ComputeExtremes(model,layers[layer_id],&extremes[layer_id][0],&extremes[layer_id][1]);
    // array for cell scalars
    VTK_DEF(vtkDoubleArray,celldata);
    VTK_INI(vtkDoubleArray,celldata);
    celldata->SetName(layersNames[layer_id].c_str());
    celldata->SetNumberOfComponents(1);
    celldata->SetNumberOfValues(cols*rows);
    int valueIndex = 0;
    double z = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            z = calGet2Dr(model,layers[layer_id],i,j);
            celldata->SetValue(valueIndex,z);
            valueIndex ++;
        }
    }
    // save scalars
    datasets[dataset_id]->GetCellData()->SetScalars(celldata);
}
void CALVTKRender::calvtkGenerateAllLayersScalarsValues()
{
    for(int i = 0; i < layers.size(); i++)
    {
        calvtkGenerateLayerScalarValues(i);
    }
}

void CALVTKRender::calvtkSetBaseRendering(calvtkBaseRendering baseRender)
{
    this->baseRendering = baseRender;
}
void CALVTKRender::calvtkUpdateLayerScalarValues(int layer_id)
{
    int dataset_id = layer_id;
    ComputeExtremes(model,layers[layer_id],&extremes[layer_id][0],&extremes[layer_id][1]);
    vtkDoubleArray *scalars = vtkDoubleArray::FastDownCast(datasets[dataset_id]->GetCellData()->GetScalars());
    int valueIndex = 0;
    double z = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            z = calGet2Dr(model,layers[layer_id],i,j);
            scalars->SetValue(valueIndex,z);
            valueIndex ++;
        }
    }
    scalars->Modified();
}

void CALVTKRender::calvtkWarpScalar()
{
    // average cell data to point data
    VTK_DEF(vtkCellDataToPointData,cellToPoint);
    VTK_INI(vtkCellDataToPointData,cellToPoint);
    cellToPoint->SetInputData(datasets[0]);
    cellToPoint->Update();

    vtkWarpScalar* warper;
    for(int i = 0; i < datasets.size(); i++){
        // save point data
        datasets[i]->GetPointData()->SetScalars(cellToPoint->GetPolyDataOutput()->GetPointData()->GetScalars());
        // warp point scalars
        warper = vtkWarpScalar::New();
        warper->SetInputData(datasets[i]);
        warper->SetScaleFactor(scaleFactor);
        warper->UseNormalOn();
        warper->SetNormal(.0,.0,1);
        warper->Update();
        datasets[i]->DeepCopy(warper->GetOutput());
        warper->Delete();
    }
}

CALVTKRender::CALVTKRender(CALModel2D *model,int cellsize)
{
    this->model = model;
    this->cols = model->columns;
    this->rows = model->rows;
    this->cellsize = cellsize;
    scaleFactor = 1;
    z_offset = .0;

    axes = NULL;
}


CALVTKRender::~CALVTKRender()
{
    renderingTimer->Delete();
    renderWindowInteractor->Delete();
    renderWindow->Delete();
    renderer->Delete();
    axes->Delete();
    for(int i = 0; i < actors.size(); i++){
        actors[i]->Delete();
    }
    actors.clear();
    for(int i = 0; i < mappers.size(); i++){
        mappers[i]->Delete();
    }
    mappers.clear();
    for(int i = 0; i < datasets.size(); i++){
        datasets[i]->Delete();
    }
    datasets.clear();
    for(int i = 0; i < lookupTables.size(); i++){
        lookupTables[i]->Delete();
    }
    lookupTables.clear();
    for(int i = 0; i < extremes.size(); i++){
        delete extremes[i];
    }
    extremes.clear();
    layersNames.clear();

}

void CALVTKRender::ComputeExtremes(CALModel2D* ca2D, CALSubstate2Dr* Q,double * m,double*M){
    int i, j;
    for (i=0; i<ca2D->rows; i++)
        for (j=0; j<ca2D->columns; j++)
            if (calGet2Dr(ca2D,Q,i,j) > 0)
            {
                *m = calGet2Dr(ca2D,Q,i,j);
                *M = calGet2Dr(ca2D,Q,i,j);
            }
    for (i=0; i<ca2D->rows; i++)
        for (j=0; j<ca2D->columns; j++)
        {
            if (*M < calGet2Dr(ca2D,Q,i,j) && calGet2Dr(ca2D,Q,i,j) > 0)
                *M = calGet2Dr(ca2D,Q,i,j);
            if (*m > calGet2Dr(ca2D,Q,i,j) && calGet2Dr(ca2D,Q,i,j) > 0)
                *m = calGet2Dr(ca2D,Q,i,j);
        }
}

void CALVTKRender::ClipPolyData(){
    //    VTK_DEF(vtkPlane,clipPlane);
    //    VTK_INI(vtkPlane,clipPlane);
    //    clipPlane->SetOrigin(0,0,z_min*scaleFactor);
    //    clipPlane->SetNormal(0,0,-1);
    //    VTK_DEF(vtkClipPolyData,clipper);
    //    VTK_INI(vtkClipPolyData,clipper);
    //    clipper->SetInputConnection(source->GetOutputPort());
    //    clipper->SetClipFunction(clipPlane);
    //    clipper->GenerateClipScalarsOn();
    //    clipper->GenerateClippedOutputOff();
    //    clipper->Update();
    //    polyoutput->DeepCopy(clipper->GetOutput());
}

void CALVTKRender::calvtkGenerateAllLayerLookupTable()
{
    for(int i = 0; i < layers.size(); i++)
    {
        calvtkGenerateLayerLookupTable(i);
    }
}
void CALVTKRender::calvtkBuildAllLayerLookupTable()
{
    for(int i = 0; i < layers.size(); i++)
    {
        lookupTables[i]->Build();
    }
}

void CALVTKRender::calvtkGenerateLayerLookupTable(int layer_id)
{
    vtkLookupTable* lookupTable = vtkLookupTable::New();
    lookupTable->SetNumberOfTableValues(extremes[layer_id][1]+1);
    lookupTable->SetRange(extremes[layer_id][0],extremes[layer_id][1]);
    // default values
    lookupTable->UseBelowRangeColorOn();
    lookupTable->SetBelowRangeColor(0,0,0,0);
    lookupTable->UseAboveRangeColorOn();
    lookupTable->SetAboveRangeColor(0,0,0,0);
    lookupTable->SetHueRange(0.0,0.16);
    lookupTable->SetAlphaRange(1.0,1.0);

    lookupTables.push_back(lookupTable);
}
void CALVTKRender::calvtkUpdateLayerLookupTable(int layer_id){
  vtkLookupTable* lookupTable = lookupTables[layer_id];
  lookupTable->SetRange(extremes[layer_id][0],extremes[layer_id][1]);
  lookupTable->Build();

  vtkPolyDataMapper * mapper = mappers[layer_id];
  mapper->SetScalarRange(extremes[layer_id][0],extremes[layer_id][1]);
}

void CALVTKRender::calvtkSetLayerHueRange(int layer_id, double minHue, double maxHue)
{
    if(layers.size() == 0)
        return;
    lookupTables[layer_id]->SetHueRange(minHue,maxHue);

}
void CALVTKRender::calvtkSetLayerSaturationRange(int layer_id, double minSaturation,double maxSaturation)
{
    if(layers.size() == 0)
        return;
    lookupTables[layer_id]->SetSaturationRange(minSaturation,maxSaturation);
}
void CALVTKRender::calvtkSetLayerValueRange(int layer_id, double minValue, double maxValue)
{
    if(layers.size() == 0)
        return;
    lookupTables[layer_id]->SetValueRange(minValue,maxValue);
}
void CALVTKRender::calvtkSetLayerBelowColor(int layer_id,double red, double green, double blue, double alpha)
{
    if(layers.size() == 0)
        return;
    lookupTables[layer_id]->UseBelowRangeColorOn();
    lookupTables[layer_id]->SetBelowRangeColor(red,green,blue,alpha);
}
void CALVTKRender::calvtkSetLayerAboveColor(int layer_id,double red, double green, double blue, double alpha)
{
    if(layers.size() == 0)
        return;
    lookupTables[layer_id]->UseAboveRangeColorOn();
    lookupTables[layer_id]->SetAboveRangeColor(red,green,blue,alpha);
}
void CALVTKRender::calvtkRenderInizialization()
{
    calvtkRenderInizialization(1);
}

void CALVTKRender::calvtkRenderInizialization(unsigned long renderingTimerDuration)
{
    vtkPolyDataMapper* mapper;
    for(int i = 0 ; i <datasets.size(); i++)
    {
        mapper = vtkPolyDataMapper::New();
        mapper->GlobalImmediateModeRenderingOn();
        mapper->SetInputData(datasets[i]);
        mapper->SetScalarModeToUseCellData();
        mapper->SetScalarVisibility(true);
        if(lookupTables.size() > 0){
            mapper->SetLookupTable(lookupTables[i]);
        }
        mapper->SetScalarRange(extremes[i][0],extremes[i][1]);
        mappers.push_back(mapper);
    }


    vtkActor* actor;
    for(int i = 0; i < mappers.size(); i++)
    {
        actor = vtkActor::New();
        actor->SetMapper(mappers[i]);
        actors.push_back(actor);
    }

    renderer = vtkRenderer::New();
    for(int i = 0; i < actors.size(); i++)
    {
        renderer->AddActor(actors[i]);
    }

    if(axes){
        renderer->AddActor(axes);
    }
    renderer->SetBackground(.1,.2,.3);
    renderer->SetViewport(0,0,1,1);
    renderer->ResetCamera();

    renderWindow = vtkRenderWindow::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("calvtk_sciddicaT_unsafe");
    renderWindow->SetSize(640,480);
    renderWindow->SetMultiSamples(0);

    renderWindowInteractor = vtkRenderWindowInteractor::New();
    renderWindowInteractor->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderWindowInteractor->Initialize();

    if(renderingTimerDuration > 0){
        renderingTimer = CALVTKRenderingTimer::New();
        renderWindowInteractor->AddObserver(vtkCommand::TimerEvent,renderingTimer);
        renderWindowInteractor->CreateRepeatingTimer(renderingTimerDuration);
    }
}

void CALVTKRender::Update()
{

    for(int i = 0; i < layers.size(); i++)
    {

        calvtkUpdateLayerScalarValues(i);
        calvtkUpdateLayerLookupTable(i);
    }
}
void CALVTKRender::Render()
{
    renderWindow->Render();
}


void CALVTKRender::Inizialize()
{
    renderWindowInteractor->Initialize();
}

void CALVTKRender::Start()
{
    renderWindowInteractor->Start();
}
