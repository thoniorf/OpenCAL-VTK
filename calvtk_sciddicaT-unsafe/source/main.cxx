extern "C"{
#include "sciddicaT.h"
}

#include "vtk.h"
int main()
{
    time_t start_time, end_time;

    sciddicaTCADefinition();
    sciddicaTConfigurationLoad();
    // simulation run
    calRunAddInitFunc2D(sciddicaT_simulation, sciddicaTSimulationInit);
    printf ("Starting simulation...\n");
    start_time = time(NULL);
    calRun2D(sciddicaT_simulation);
    end_time = time(NULL);
    printf ("Simulation terminated.\nElapsed time: %lds\n", end_time-start_time);
    sciddicaTConfigurationSave();
    sciddicaTFinalization();



    return 0;
}
