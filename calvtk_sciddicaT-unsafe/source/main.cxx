#include "sciddicaT.c"
int main()
{
    time_t start_time, end_time;

    // define of the sciddicaT CA and sciddicaT_simulation simulation objects
    struct CALModel2D* sciddicaT = calCADef2D (ROWS, COLS, CAL_VON_NEUMANN_NEIGHBORHOOD_2D, CAL_SPACE_TOROIDAL, CAL_OPT_ACTIVE_CELLS);
    struct CALRun2D* sciddicaT_simulation = calRunDef2D(sciddicaT, 1, STEPS, CAL_UPDATE_IMPLICIT);

    // add transition function's sigma_1 and sigma_2 elementary processes
    calAddElementaryProcess2D(sciddicaT, sciddicaTFlowsComputation);
    calAddElementaryProcess2D(sciddicaT, sciddicaTRemoveInactiveCells);

    // add substates
    Q.z = calAddSingleLayerSubstate2Dr(sciddicaT);
    Q.h = calAddSubstate2Dr(sciddicaT);

    // load configuration
    calLoadSubstate2Dr(sciddicaT, Q.z, DEM_PATH);
    calLoadSubstate2Dr(sciddicaT, Q.h, SOURCE_PATH);

    // simulation run
    calRunAddInitFunc2D(sciddicaT_simulation, sciddicaTSimulationInit);
    printf ("Starting simulation...\n");
    start_time = time(NULL);
    calRun2D(sciddicaT_simulation);
    end_time = time(NULL);
    printf ("Simulation terminated.\nElapsed time: %lds\n", end_time-start_time);

    // saving configuration
    calSaveSubstate2Dr(sciddicaT, Q.h, OUTPUT_PATH);

    // finalizations
    calRunFinalize2D(sciddicaT_simulation);
    calFinalize2D(sciddicaT);

    return 0;
}
