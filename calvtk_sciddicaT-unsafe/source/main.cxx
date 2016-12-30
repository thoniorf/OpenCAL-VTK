#include "sciddicaT.h"
#include "calvtk.h"

#include <thread>
#include <mutex>
CALVTKRender *vtkRender;
bool simulationUpdateAndRender(){
    CALbyte again;

    again = calRunCAStep2D(sciddicaT_simulation);
    sciddicaT_simulation->step++;

    if(sciddicaT_simulation->step %10 == 0){
        vtkRender->Update();
    }

    return again;
}

void simulationThreadWorker(bool (*simulationRunFunction)()){
    bool again;
    do {
        again = (*simulationRunFunction)();
    }
    while(again);
}

void ModifiedCallback( vtkObject* vtkNotUsed(caller), long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData) )
{
    vtkRender->Render();
}

int main()
{
    time_t start_time, end_time;

    sciddicaTCADefinition();
    sciddicaTConfigurationLoad();

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
    vtkRender->calvtkRenderInizialization();


    // simulation run
    calRunAddInitFunc2D(sciddicaT_simulation, sciddicaTSimulationInit);
    calRunAddGlobalTransitionFunc2D(sciddicaT_simulation, sciddicaTransitionFunction);
    calRunAddStopConditionFunc2D(sciddicaT_simulation, sciddicaTSimulationStopCondition);

    printf ("Starting simulation...\n");
    start_time = time(NULL);
    // applies the callback init func registered by calRunAddInitFunc2D()
    calRunInitSimulation2D(sciddicaT_simulation);



    //std::thread simulationRunThread(simulationThreadWorker,simulationUpdateAndRender);

    vtkRender->Inizialize();
    vtkRender->Start();

    //simulationRunThread.join();

    calRunFinalizeSimulation2D(sciddicaT_simulation);
    end_time = time(NULL);
    printf ("Simulation terminated.\nElapsed time: %lds\n", end_time-start_time);

    sciddicaTConfigurationSave();
    sciddicaTFinalization();

    delete vtkRender;

    return EXIT_SUCCESS;
}
