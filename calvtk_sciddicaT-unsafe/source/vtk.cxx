#include "vtk.h"
double z_min,z_Max,h_min,h_Max;
vtkSmartPointer<vtkUnsignedCharArray> cellData;
vtkSmartPointer<vtkPlaneSource> topology;
vtkSmartPointer<vtkPolyDataMapper> topologyMapper;
vtkSmartPointer<vtkActor> topologyActor;
vtkSmartPointer<vtkRenderer> renderer;
vtkSmartPointer<vtkRenderWindow> renderWindow;
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
vtkSmartPointer<vtkInteractorStyleJoystickCamera> joystickStyleInteractor;

void vtkDataSetLoad(){
    topology = vtkSmartPointer<vtkPlaneSource>::New();
    topology->SetResolution(COLS,ROWS);
    topology->Update();
    cellData = vtkSmartPointer<vtkUnsignedCharArray>::New();
    cellData->SetName("CellData");
    cellData->SetNumberOfComponents(3);
    cellData->SetNumberOfTuples(topology->GetOutput()->GetNumberOfCells());
    // computer extremes
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            z_Max = calGet2Dr(sciddicaT,Q.z,i,j);
            if(z_Max != 0)
                z_min = z_Max;
            h_Max = calGet2Dr(sciddicaT,Q.h,i,j);
            if(h_Max != 0)
                h_min = h_Max;
        }
    }
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(calGet2Dr(sciddicaT,Q.z,i,j)>0){
                if(z_Max < calGet2Dr(sciddicaT,Q.z,i,j))
                    z_Max = calGet2Dr(sciddicaT,Q.z,i,j);
                if(z_min > calGet2Dr(sciddicaT,Q.z,i,j))
                    z_min = calGet2Dr(sciddicaT,Q.z,i,j);
            }
            if(calGet2Dr(sciddicaT,Q.h,i,j)>0){
                if(h_Max < calGet2Dr(sciddicaT,Q.h,i,j))
                    h_Max = calGet2Dr(sciddicaT,Q.h,i,j);
                if(h_min > calGet2Dr(sciddicaT,Q.h,i,j))
                    h_min = calGet2Dr(sciddicaT,Q.h,i,j);
            }
        }
    }

}
void vtkDataSetScalarsSet(){
    double z,h,t=0;
    float color[3];
    for(int i = 0; i< ROWS; i++){
        for(int j = 0; j <COLS  ;j++){
            h = calGet2Dr(sciddicaT,Q.h,i,j);
            z = calGet2Dr(sciddicaT,Q.z,i,j);
            if(h>0){
                color[0] = 1;
                color[1] = (z-z_min)/(z_Max-z_min);
                color[2] = 0;
            }else if(z>=0){
                color[0] = (z-z_min)/(z_Max-z_min);
                color[1] = (z-z_min)/(z_Max-z_min);
                color[2] = (z-z_min)/(z_Max-z_min);
            }
            cellData->InsertNextTuple(color);
        }
    }
    topology->GetOutput()->GetCellData()->SetScalars(cellData);
    topology->Update();
}

void vtkRenderDefinition(){

    topologyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    topologyMapper->SetInputConnection(topology->GetOutputPort());
    topologyMapper->SetScalarModeToUseCellData();

    topologyActor = vtkSmartPointer<vtkActor>::New();
    topologyActor->SetMapper(topologyMapper);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(topologyActor);
    renderer->SetBackground(.1,.2,.3);
    renderer->SetViewport(0,0,1,1);
    renderer->GetActiveCamera()->SetViewUp(0,-1,0);
    renderer->GetActiveCamera()->Azimuth(180);

    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("calvtk_sciddicaT_unsafe");
    renderWindow->SetSize(640,480);

    renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    VTK_SP(vtkCallbackCommand,keypressCallBack);
    keypressCallBack->SetCallback(KeypressCallbackFunction);
    renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent,keypressCallBack);
    joystickStyleInteractor = vtkSmartPointer<vtkInteractorStyleJoystickCamera>::New();
    renderWindowInteractor->SetInteractorStyle(joystickStyleInteractor);
    renderWindow->Render();
    //renderWindowInteractor->Start();
}

void vtkFinalization(){
    topology->Delete();
    topologyActor->Delete();
    topologyMapper->Delete();
    renderer->Delete();
    renderWindow->Delete();
}

void KeypressCallbackFunction(vtkObject *caller, unsigned long eventId, void *clientData, void *callData){
    std::cout<<"Exit"<<endl;
    vtkRenderWindowInteractor *iren =  static_cast<vtkRenderWindowInteractor*>(caller);
    // Close the window
    iren->GetRenderWindow()->Finalize();
    // Stop the interactor
    iren->TerminateApp();
}
