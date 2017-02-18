#include "sciddicaT.h"
#include "calvtk.h"

#include "calvtkaxes.h"
#include "calvtklayer2d.h"
#include "calvtkrender2d.h"
#include "calvtkrun.h"

#include <thread>
#include <mutex>
CALVTKRender *vtkRender;
bool simulationUpdateAndRender(){
    CALbyte again;

    again = calRunCAStep2D(sciddicaT_simulation);
    sciddicaT_simulation->step++;
    return again;
}

void simulationThreadWorker(bool (*simulationRunFunction)()){
    time_t start_time, end_time;
    bool again;

    printf ("Starting simulation...\n");
    start_time = time(NULL);
    // applies the callback init func registered by calRunAddInitFunc2D()
    calRunInitSimulation2D(sciddicaT_simulation);


    do {
        again = (*simulationRunFunction)();
    }
    while(again);

    calRunFinalizeSimulation2D(sciddicaT_simulation);
    end_time = time(NULL);
    printf ("Simulation terminated.\nElapsed time: %lds\n", end_time-start_time);
}

int main()
{
    sciddicaTCADefinition();
    sciddicaTConfigurationLoad();
    // simulation run
    calRunAddInitFunc2D(sciddicaT_simulation, sciddicaTSimulationInit);
    calRunAddGlobalTransitionFunc2D(sciddicaT_simulation, sciddicaTransitionFunction);
    calRunAddStopConditionFunc2D(sciddicaT_simulation, sciddicaTSimulationStopCondition);

    calvtkLayer2D* elevation = calvtkLayer2D::New();
    elevation->SetName("Elevation");
    elevation->SetCALModel2D(sciddicaT);
    elevation->SetCALSubstate2D(Q.z);
    elevation->SetCellSize(10);
    elevation->SetLayerType(CALVTK_SINGLE_LAYER);
    elevation->GenerateDataSet();
    elevation->GenerateScalarValues();

    calvtkLookupTable* elevationLUT = calvtkLookupTable::New();
    elevationLUT->SetHueRange(.0,.0);
    elevationLUT->SetSaturationRange(.0,.0);
    elevationLUT->SetValueRange(0.,1.);
    elevationLUT->SetAboveRangeColor(.0,.0,.0,.0);
    elevationLUT->SetBelowRangeColor(.0,.0,.0,.0);
    elevationLUT->SetScalarRange(elevation->GetExtremes());
    elevationLUT->Build();
    elevation->SetLookupTable(elevationLUT);

    calvtkLayer2D* debris = calvtkLayer2D::New();
    debris->SetName("Debris");
    debris->SetCALModel2D(sciddicaT);
    debris->SetCALSubstate2D(Q.h);
    debris->SetCellSize(10);
    debris->SetLayerType(CALVTK_MULTI);
    debris->SetPvalue(P_EPSILON);
    debris->GenerateDataSet();
    debris->GenerateScalarValues();

    calvtkLookupTable* debrisLUT = calvtkLookupTable::New();
    debrisLUT->SetHueRange(.0,.16);
    debrisLUT->SetAboveRangeColor(.0,.0,.0,.0);
    debrisLUT->SetBelowRangeColor(.0,.0,.0,.0);
    debrisLUT->SetScalarRange(debris->GetExtremes());
    debrisLUT->Build();
    debris->SetLookupTable(debrisLUT);


    calvtkRender2D* render = calvtkRender2D::New();
    render->AddLayer(elevation);
    render->AddLayer(debris);
    render->SetBackgroundColor(.1,.2,.3);
    render->Inizialization();
    render->Update();

    calvtkRun * run = calvtkRun::New();
    run->SetRender(render);
    run->SetSimulationFunction(simulationUpdateAndRender);
    run->SetCALRun(sciddicaT_simulation);
    run->Initialize();
    run->CreateRefreshRenderTimer(0);
    calvtkAxes* axes = calvtkAxes::New();
    axes->SetInteractor(run);
    axes->SetViewport(0,0,0.4,0.4);
    axes->SetEnabled(1);
    axes->InteractiveOff();

    render->ResetCamera();

    run->Start();

    run->Delete();
    render->Delete();
    debris->Delete();
    elevation->Delete();
    axes->Delete();

    return EXIT_SUCCESS;

    vtkRender = new CALVTKRender(sciddicaT,10);
    vtkRender->calvtkSetBaseRendering(CALVTK_RENDER3D);
    vtkRender->calvtkAddLayer(Q.z,"Elevation");
    vtkRender->calvtkAddLayer(Q.h,"Debris");
    vtkRender->calvtkGenerateAllLayersPolyData();
    vtkRender->calvtkGenerateAllLayersScalarsValues();
    vtkRender->calvtkGenerateAllLayerLookupTable();

    int z_id = vtkRender->calvtkGetLayerId(Q.z);
    vtkRender->calvtkSetLayerValueRange(z_id,0.0,1.0);
    vtkRender->calvtkSetLayerHueRange(z_id,.0,.0);
    vtkRender->calvtkSetLayerSaturationRange(z_id,.0,.0);
    vtkRender->calvtkSetLayerAboveColor(z_id,.0,.0,.0,.0);
    vtkRender->calvtkSetLayerBelowColor(z_id,.0,.0,.0,.0);

    vtkRender->calvtkBuildAllLayerLookupTable();
    vtkRender->calvtkWarpScalar();
    vtkRender->calvtkAddAxes();
    vtkRender->calvtkRenderInizialization(1);



    std::thread simulationRunThread(simulationThreadWorker,simulationUpdateAndRender);

    vtkRender->Start();

    simulationRunThread.join();

    sciddicaTConfigurationSave();
    sciddicaTFinalization();

    delete vtkRender;

    return EXIT_SUCCESS;
}
