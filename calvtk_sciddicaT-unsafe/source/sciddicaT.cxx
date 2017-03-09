#include "sciddicaT.h"

struct CALModel2D* sciddicaT;
struct CALRun2D* sciddicaT_simulation;
struct sciddicaTSubstates Q;
struct sciddicaTParameters P;

// The sciddicaT transition function
void sciddicaTFlowsComputation(struct CALModel2D* sciddicaT, int i, int j)
{
    CALbyte eliminated_cells[5]={CAL_FALSE,CAL_FALSE,CAL_FALSE,CAL_FALSE,CAL_FALSE};
    CALbyte again;
    CALint cells_count;
    CALreal average;
    CALreal m;
    CALreal u[5];
    CALint n;
    CALreal z, h;
    CALreal f;


    m = calGet2Dr(sciddicaT, Q.h, i, j) - P.epsilon;
    u[0] = calGet2Dr(sciddicaT, Q.z, i, j) + P.epsilon;
    for (n=1; n<sciddicaT->sizeof_X; n++)
    {
        z = calGetX2Dr(sciddicaT, Q.z, i, j, n);
        h = calGetX2Dr(sciddicaT, Q.h, i, j, n);
        u[n] = z + h;
    }

    //computes outflows and updates debris thickness
    do{
        again = CAL_FALSE;
        average = m;
        cells_count = 0;

        for (n=0; n<sciddicaT->sizeof_X; n++)
            if (!eliminated_cells[n]){
                average += u[n];
                cells_count++;
            }

            if (cells_count != 0)
                average /= cells_count;

            for (n=0; n<sciddicaT->sizeof_X; n++)
                if( (average<=u[n]) && (!eliminated_cells[n]) ){
                    eliminated_cells[n]=CAL_TRUE;
                    again=CAL_TRUE;
                }

    }while (again);

    for (n=1; n<sciddicaT->sizeof_X; n++)
        if (!eliminated_cells[n])
        {
            f = (average-u[n])*P.r;
            calSet2Dr (sciddicaT,Q.h,i,j,   calGetNext2Dr (sciddicaT,Q.h,i,j)   - f );
            calSetX2Dr(sciddicaT,Q.h,i,j,n, calGetNextX2Dr(sciddicaT,Q.h,i,j,n) + f );

            //adds the cell (i, j, n) to the set of active ones
            calAddActiveCellX2D(sciddicaT, i, j, n);
        }
}


void sciddicaTRemoveInactiveCells(struct CALModel2D* sciddicaT, int i, int j)
{
    if (calGet2Dr(sciddicaT, Q.h, i, j) <= P.epsilon)
        calRemoveActiveCell2D(sciddicaT,i,j);
}


void sciddicaTSimulationInit(struct CALModel2D* sciddicaT)
{
    CALreal z, h;
    CALint i, j;

    //sciddicaT parameters setting
    P.r = P_R;
    P.epsilon = P_EPSILON;

    //sciddicaT source initialization
    for (i=0; i<sciddicaT->rows; i++)
        for (j=0; j<sciddicaT->columns; j++)
        {
            h = calGet2Dr(sciddicaT, Q.h, i, j);

            if ( h > 0.0 ) {
                z = calGet2Dr(sciddicaT, Q.z, i, j);
                calSetCurrent2Dr(sciddicaT, Q.z, i, j, z-h);

                //adds the cell (i, j) to the set of active ones
        calAddActiveCell2D(sciddicaT, i, j);
            }
        }

    calUpdate2D(sciddicaT);
}


void sciddicaTransitionFunction(struct CALModel2D* sciddicaT)
{
    // active cells must be updated first becouse outflows
    // have already been sent to (pheraps inactive) the neighbours
      calApplyElementaryProcess2D(sciddicaT, sciddicaTFlowsComputation);
      calUpdateActiveCells2D(sciddicaT);
      calUpdateSubstate2Dr(sciddicaT, Q.h);

    // here you don't need to update Q.h
      calApplyElementaryProcess2D(sciddicaT, sciddicaTRemoveInactiveCells);
      calUpdateActiveCells2D(sciddicaT);
}


CALbyte sciddicaTSimulationStopCondition(struct CALModel2D* sciddicaT)
{
    if (sciddicaT_simulation->step >= STEPS)
        return CAL_TRUE;
    return CAL_FALSE;
}

void sciddicaTCADefinition(){
    // define of the sciddicaT CA and sciddicaT_simulation simulation objects
    sciddicaT = calCADef2D (ROWS, COLS, CAL_VON_NEUMANN_NEIGHBORHOOD_2D, 
                            CAL_SPACE_TOROIDAL, CAL_OPT_ACTIVE_CELLS);
    sciddicaT_simulation = calRunDef2D(sciddicaT, 1, CAL_RUN_LOOP, CAL_UPDATE_EXPLICIT);

    // add transition function's sigma_1 and sigma_2 elementary processes
    calAddElementaryProcess2D(sciddicaT, sciddicaTFlowsComputation);
    calAddElementaryProcess2D(sciddicaT, sciddicaTRemoveInactiveCells);

    // add substates
    Q.z = calAddSingleLayerSubstate2Dr(sciddicaT);
    Q.h = calAddSubstate2Dr(sciddicaT);
}
void sciddicaTConfigurationLoad(){
    // load configuration
    calLoadSubstate2Dr(sciddicaT, Q.z, DEM_PATH);
    calLoadSubstate2Dr(sciddicaT, Q.h, SOURCE_PATH);
}

void sciddicaTConfigurationSave(){
    // saving configuration
    calSaveSubstate2Dr(sciddicaT, Q.h, OUTPUT_PATH);
}

void sciddicaTFinalization(){
    // finalizations
    calRunFinalize2D(sciddicaT_simulation);
    calFinalize2D(sciddicaT);
}
