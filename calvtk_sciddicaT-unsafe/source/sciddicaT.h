// The SciddicaT further optimized CCA debris flows model
#ifndef SciddicaT
#define SciddicaT

extern "C" {
#include <OpenCAL/cal2D.h>
#include <OpenCAL/cal2DIO.h>
#include <OpenCAL/cal2DRun.h>
#include <OpenCAL/cal2DUnsafe.h>
#include <stdlib.h>
#include <time.h>
}


// Some definitions...
#define ROWS 610
#define COLS 496
#define P_R 0.5
#define CELLSIZE 10
#define P_EPSILON 0.001
#define STEPS 4000
#define DEM_PATH "./data/dem.txt"
#define SOURCE_PATH "./data/source.txt"
#define OUTPUT_PATH "./data/width_final.txt"
#define NUMBER_OF_OUTFLOWS 4

// declare CCA model (sciddicaT), substates (Q), parameters (P),
// and simulation object (sciddicaT_simulation)
struct sciddicaTSubstates {
    struct CALSubstate2Dr *z;
    struct CALSubstate2Dr *h;
};

struct sciddicaTParameters {
    CALParameterr epsilon;
    CALParameterr r;
};

extern  struct CALRun2D* sciddicaT_simulation;
extern struct sciddicaTSubstates Q;
extern struct sciddicaTParameters P;

extern struct CALModel2D* sciddicaT;

void sciddicaTFlowsComputation(struct CALModel2D* sciddicaT, int i, int j);
void sciddicaTRemoveInactiveCells(struct CALModel2D* sciddicaT, int i, int j);
void sciddicaTSimulationInit(struct CALModel2D* sciddicaT);
void sciddicaTransitionFunction(struct CALModel2D* sciddicaT);
CALbyte sciddicaTSimulationStopCondition(struct CALModel2D* sciddicaT);
void sciddicaTCADefinition();
void sciddicaTConfigurationLoad();
void sciddicaTConfigurationSave();
void sciddicaTFinalization();
#endif

