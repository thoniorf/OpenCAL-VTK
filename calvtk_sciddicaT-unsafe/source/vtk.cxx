#include "vtk.h"

vtkSmartPointer<vtkDoubleArray> elevation;
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

    elevation = vtkSmartPointer<vtkDoubleArray>::New();
    elevation->SetName("Elevation");
    for(int i = 0; i< ROWS; i++){
        for(int j = 0; j <COLS  ;j++){
            elevation->InsertNextValue(calGet2Dr(sciddicaT,Q.z,i,j));
        }
    }
    topology->GetOutput()->GetCellData()->SetScalars(elevation);

}
void vtkRenderDefinition(){

    VTK_SP(vtkLookupTable,colorTable);
    colorTable->SetTableRange(0,1500);
    colorTable->SetValueRange(0,1);
    colorTable->SetHueRange(0,0);
    colorTable->SetSaturationRange(0,0);
    colorTable->Build();

    topologyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    topologyMapper->SetInputConnection(topology->GetOutputPort());
    topologyMapper->SetScalarModeToUseCellData();
    topologyMapper->SetColorModeToMapScalars();
    topologyMapper->SetScalarRange(0,1500);
    topologyMapper->SetLookupTable(colorTable);


    topologyActor = vtkSmartPointer<vtkActor>::New();
    topologyActor->SetMapper(topologyMapper);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(topologyActor);
    renderer->SetBackground(.1,.2,.3);
    renderer->SetViewport(0,0,1,1);

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
    renderWindowInteractor->Start();
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
