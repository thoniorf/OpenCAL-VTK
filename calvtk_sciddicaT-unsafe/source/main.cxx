extern "C"{
#include "sciddicaT.h"
}

#include "vtk.h"
int main()
{
    CALbyte again;
    time_t start_time, end_time;

    sciddicaTCADefinition();
    sciddicaTConfigurationLoad();
    vtkDataSetLoad();
    vtkDataSetScalarsSet();
    vtkRenderDefinition();
    // simulation run
    calRunAddInitFunc2D(sciddicaT_simulation, sciddicaTSimulationInit);
    calRunAddGlobalTransitionFunc2D(sciddicaT_simulation, sciddicaTransitionFunction);
    calRunAddStopConditionFunc2D(sciddicaT_simulation, sciddicaTSimulationStopCondition);

    printf ("Starting simulation...\n");
    start_time = time(NULL);
    // applies the callback init func registered by calRunAddInitFunc2D()
    calRunInitSimulation2D(sciddicaT_simulation);
    // the do-while explicitates the calRun2D() implicit looop
    do{
            again = calRunCAStep2D(sciddicaT_simulation);
            sciddicaT_simulation->step++;
            // update visualization
               vtkDataSetScalarsSet();
               renderWindow->Render();
               printf("Step++");
    } while (again);
    calRunFinalizeSimulation2D(sciddicaT_simulation);
    end_time = time(NULL);
    printf ("Simulation terminated.\nElapsed time: %lds\n", end_time-start_time);
    sciddicaTConfigurationSave();
    sciddicaTFinalization();
    vtkFinalization();
    return EXIT_SUCCESS;
}
