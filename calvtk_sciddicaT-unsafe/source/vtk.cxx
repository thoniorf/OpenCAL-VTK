#include "vtk.h"
vtkSmartPointer<vtkImageData> topology;
vtkSmartPointer<vtkDataSetMapper> topologyMapper;
vtkSmartPointer<vtkActor> topologyActor;
vtkSmartPointer<vtkRenderer> renderer;
vtkSmartPointer<vtkRenderWindow> renderWindow;
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
vtkSmartPointer<vtkInteractorStyleJoystickCamera> joystickStyleInteractor;
void vtkDataSetLoad(){
    topology = vtkSmartPointer<vtkImageData>::New();
    topology->SetDimensions(ROWS,COLS,1);
    topology->SetSpacing(1.0,1.0,1.0);
    topology->SetOrigin(.0,.0,.0);

}
void vtkRenderDefinition(){
    topologyMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    topologyMapper-> SetInputData(topology);

    topologyActor = vtkSmartPointer<vtkActor>::New();
    topologyActor->SetMapper(topologyMapper);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(topologyActor);
    renderer->SetViewport(.0,.0,1,1);

    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("calvtk_sciddicaT_unsafe");

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
