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
    vtkRender->calvtkRenderInizialization(0);

    // simulation run
    calRunAddInitFunc2D(sciddicaT_simulation, sciddicaTSimulationInit);
    calRunAddGlobalTransitionFunc2D(sciddicaT_simulation, sciddicaTransitionFunction);
    calRunAddStopConditionFunc2D(sciddicaT_simulation, sciddicaTSimulationStopCondition);

    std::thread simulationRunThread(simulationThreadWorker,simulationUpdateAndRender);

    vtkRender->Start();

    simulationRunThread.join();

    sciddicaTConfigurationSave();
    sciddicaTFinalization();

    delete vtkRender;

    return EXIT_SUCCESS;
}
