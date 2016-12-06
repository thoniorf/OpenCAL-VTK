#include "sciddicatvtkrender.h"
SciddicaTVTKRender::SciddicaTVTKRender()
{
    VTK_INI(vtkPolyData,polyoutput);

    VTK_DEF(vtkTransform,trans);
    VTK_INI(vtkTransform,trans);
    trans->Scale(COLS*CELLSIZE,ROWS*CELLSIZE,1);

    VTK_INI(vtkPlaneSource,source);
    source->SetResolution(COLS,ROWS);
    source->Update();


    VTK_DEF(vtkTransformPolyDataFilter,polytrans);
    VTK_INI(vtkTransformPolyDataFilter,polytrans);
    polytrans->SetInputConnection(source->GetOutputPort());
    polytrans->SetTransform(trans);
    polytrans->Update();

    source->GetOutput()->DeepCopy(polytrans->GetOutput());

    ComputeExtremes(sciddicaT,Q.z,&z_min,&z_Max);

    scaleFactor = -1;//-0.0009;
    ComputeElevation();
    cout<<"End Elevating"<<endl;

    VTK_INI(vtkUnsignedCharArray,colorData);
    colorData->SetNumberOfComponents(3);
    colorData->SetNumberOfTuples(source->GetOutput()->GetNumberOfCells());
    cout<<"Color array setted"<<endl;
    ComputeColors();
    cout<<"End Coloring"<<endl;
    source->GetOutput()->GetCellData()->SetScalars(colorData);
    ClipPolyData();
}
SciddicaTVTKRender::~SciddicaTVTKRender()
{
    colorData->Delete();
    source->Delete();
    mapper->Delete();
    actor->Delete();
    renderer->Delete();
    renderWindow->Delete();
    renderWindowInteractor->Delete();
}
void SciddicaTVTKRender::SetScaleFactor(double scaleValue){
    scaleFactor = scaleValue;
}

void SciddicaTVTKRender::ComputeExtremes(CALModel2D* ca2D, CALSubstate2Dr* Q,double * m,double*M){
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
void SciddicaTVTKRender::ComputeColors()
{
    double z,h,t=0;
    double color[3];
    ComputeExtremes(sciddicaT,Q.h,&h_min,&h_Max);
    for(int i = 0; i< sciddicaT->rows; i++){
        for(int j = 0; j <sciddicaT->columns;j++){
            h = calGet2Dr(sciddicaT,Q.h,i,j);
            z = calGet2Dr(sciddicaT,Q.z,i,j);

            if(h>0){
                color[0] = 255;
                color[1] = ((h-h_min)/(h_Max-h_min))*255;
                color[2] = 0;
            }else if(z>0){
                color[0] = ((z-z_min)/(z_Max-z_min))*255;
                color[1] = ((z-z_min)/(z_Max-z_min))*255;
                color[2] = ((z-z_min)/(z_Max-z_min))*255;

            }
            colorData->SetTuple(t,color);
            t++;
        }
    }
    colorData->Modified();
}

void SciddicaTVTKRender::ComputeElevation()
{
    // read and store elevation
    VTK_DEF(vtkDoubleArray,elevation);
    VTK_INI(vtkDoubleArray,elevation);
    elevation->SetNumberOfValues(source->GetOutput()->GetNumberOfCells());
    int valueIndex = 0;
    double z = 0;
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            z = calGet2Dr(sciddicaT,Q.z,i,j);
            if(z > 0)
            elevation->SetValue(valueIndex,z);
            valueIndex ++;
        }
    }
    source->GetOutput()->GetCellData()->SetScalars(elevation);
    // average cell data to point data
    VTK_DEF(vtkCellDataToPointData,cellToPoint);
    VTK_INI(vtkCellDataToPointData,cellToPoint);
    cellToPoint->SetInputData(source->GetOutput());
    cellToPoint->Update();
    source->GetOutput()->GetPointData()->SetScalars(cellToPoint->GetPolyDataOutput()->GetPointData()->GetScalars());
    // warp point scalars
    VTK_DEF(vtkWarpScalar,warper);
    VTK_INI(vtkWarpScalar,warper);
    warper->SetInputData(source->GetOutput());
    warper->SetScaleFactor(scaleFactor);
    warper->UseNormalOn();
    warper->SetNormal(.0,.0,1);
    warper->Update();
    source->GetOutput()->DeepCopy(warper->GetOutput());

    //elevation->Delete();
    //cellToPoint->Delete();
    //warper->Delete();
}

void SciddicaTVTKRender::ClipPolyData(){
    VTK_DEF(vtkPlane,clipPlane);
    VTK_INI(vtkPlane,clipPlane);
    clipPlane->SetOrigin(0,0,z_min*scaleFactor);
    clipPlane->SetNormal(0,0,-1);
    VTK_DEF(vtkClipPolyData,clipper);
    VTK_INI(vtkClipPolyData,clipper);
    clipper->SetInputConnection(source->GetOutputPort());
    clipper->SetClipFunction(clipPlane);
    clipper->GenerateClipScalarsOn();
    clipper->GenerateClippedOutputOff();
    clipper->Update();
    polyoutput->DeepCopy(clipper->GetOutput());
}

void SciddicaTVTKRender::RenderInizialization()
{
    VTK_INI(vtkPolyDataMapper,mapper);
    mapper->GlobalImmediateModeRenderingOn();
    mapper->SetInputData(polyoutput);
    mapper->SetScalarModeToUseCellData();

    VTK_INI(vtkActor,actor);
    actor->SetMapper(mapper);

    VTK_INI(vtkRenderer,renderer);
    renderer->AddActor(actor);
    renderer->SetBackground(.1,.2,.3);
    renderer->SetViewport(0,0,1,1);
    renderer->GetActiveCamera()->SetPosition(0,0,2);
    renderer->GetActiveCamera()->SetViewUp(0,-1,0);
    renderer->GetActiveCamera()->Azimuth(180);
    renderer->ResetCamera();

    VTK_INI(vtkRenderWindow,renderWindow);
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("calvtk_sciddicaT_unsafe");
    renderWindow->SetSize(640,480);

    VTK_INI(vtkRenderWindowInteractor,renderWindowInteractor);
    renderWindowInteractor->SetRenderWindow(renderWindow);
}

void SciddicaTVTKRender::Update()
{
    ComputeColors();
    ClipPolyData();
}
void SciddicaTVTKRender::Render()
{
    renderWindow->Render();
}
void SciddicaTVTKRender::Inizialize()
{
    renderWindowInteractor->Initialize();
}
void SciddicaTVTKRender::CreateTimerEvent(unsigned long duration, void(*callbackCall)(vtkObject*, long unsigned int, void*, void* ))
{
    renderWindowInteractor->CreateRepeatingTimer(duration);
    VTK_DEF(vtkCallbackCommand,timerCallBack);
    VTK_INI(vtkCallbackCommand,timerCallBack);
    timerCallBack->SetCallback(callbackCall);
    renderWindowInteractor->AddObserver(vtkCommand::TimerEvent,timerCallBack);
}
void SciddicaTVTKRender::Start()
{
    renderWindowInteractor->Start();
}
