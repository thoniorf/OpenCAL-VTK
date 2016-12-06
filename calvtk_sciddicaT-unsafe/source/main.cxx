#include "sciddicaT.h"
#include "sciddicatvtkrender.h"
SciddicaTVTKRender *vtkRender;

void simulationRunStep ( vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData) )
{
    // add exit condition
    calRunCAStep2D(sciddicaT_simulation);
    sciddicaT_simulation->step++;
    if(sciddicaT_simulation->step % 10 == 0) {
        vtkRender->Update();
        vtkRender->Render();
    }

}

int main()
{
    CALbyte again;
    time_t start_time, end_time;

    sciddicaTCADefinition();
    sciddicaTConfigurationLoad();
    vtkRender = new SciddicaTVTKRender();
    vtkRender->RenderInizialization();

    // simulation run
    calRunAddInitFunc2D(sciddicaT_simulation, sciddicaTSimulationInit);
    calRunAddGlobalTransitionFunc2D(sciddicaT_simulation, sciddicaTransitionFunction);
    calRunAddStopConditionFunc2D(sciddicaT_simulation, sciddicaTSimulationStopCondition);

    printf ("Starting simulation...\n");
    start_time = time(NULL);
    // applies the callback init func registered by calRunAddInitFunc2D()
    calRunInitSimulation2D(sciddicaT_simulation);

    vtkRender->Inizialize();
    vtkRender->CreateTimerEvent(1,simulationRunStep);
    vtkRender->Start();

    calRunFinalizeSimulation2D(sciddicaT_simulation);
    end_time = time(NULL);
    printf ("Simulation terminated.\nElapsed time: %lds\n", end_time-start_time);
    sciddicaTConfigurationSave();
    sciddicaTFinalization();

    delete vtkRender;

    return EXIT_SUCCESS;
}
