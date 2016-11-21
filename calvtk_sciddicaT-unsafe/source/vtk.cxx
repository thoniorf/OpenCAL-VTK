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
void computeExtremes(struct CALModel2D* ca2D, struct CALSubstate2Dr* Q,double * m,double*M){
    // computer extremes
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(calGet2Dr(ca2D,Q,i,j) > 0){
                *M = calGet2Dr(ca2D,Q,i,j);
                *m = calGet2Dr(ca2D,Q,i,j);
            }

        }
    }
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(calGet2Dr(sciddicaT,Q,i,j)>0){
                if(*M < calGet2Dr(ca2D,Q,i,j))
                    *M = calGet2Dr(ca2D,Q,i,j);
                if(*m > calGet2Dr(ca2D,Q,i,j))
                    *m = calGet2Dr(ca2D,Q,i,j);
            }
        }
    }

}

void vtkDataSetLoad(){
    topology = vtkSmartPointer<vtkPlaneSource>::New();
    topology->SetResolution(COLS,ROWS);
    topology->GetOutput()->GlobalReleaseDataFlagOn();
    topology->Update();

    cellData = vtkSmartPointer<vtkUnsignedCharArray>::New();
    cellData->SetName("CellData");
    cellData->SetNumberOfComponents(3);
    cellData->SetNumberOfTuples(topology->GetOutput()->GetNumberOfCells());
    computeExtremes(sciddicaT,Q.z,&z_min,&z_Max);
}
void vtkDataSetScalarsUpdate(){
    double z,h,t=0;
    float color[3];
    computeExtremes(sciddicaT,Q.h,&h_min,&h_Max);
    for(int i = 0; i< ROWS; i++){
        for(int j = 0; j <COLS  ;j++){
            h = calGet2Dr(sciddicaT,Q.h,i,j);
            z = calGet2Dr(sciddicaT,Q.z,i,j);
            if(h>0){
                color[0] = 255;
                color[1] = ((h-h_min)/(h_Max-h_min))*255;
                color[2] = 0;
            }else if(z>=0){
                color[0] = ((z-z_min)/(z_Max-z_min))*255;
                color[1] = ((z-z_min)/(z_Max-z_min))*255;
                color[2] = ((z-z_min)/(z_Max-z_min))*255;
            }
            cellData->SetTuple(t,color);
            t++;
        }
    }
    cellData->Modified();
}

void vtkDataSetScalarsInit(){
    vtkDataSetScalarsUpdate();
    topology->GetOutput()->GetCellData()->SetScalars(cellData);
}

void vtkRenderDefinition(){

    topologyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    topologyMapper->SetInputConnection(topology->GetOutputPort());
    topologyMapper->SetScalarModeToUseCellData();
    topologyMapper->GlobalImmediateModeRenderingOn();

    topologyActor = vtkSmartPointer<vtkActor>::New();
    topologyActor->SetMapper(topologyMapper);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(topologyActor);
    renderer->SetBackground(.1,.2,.3);
    renderer->SetViewport(0,0,1,1);
    renderer->GetActiveCamera()->SetPosition(0,0,2);
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
