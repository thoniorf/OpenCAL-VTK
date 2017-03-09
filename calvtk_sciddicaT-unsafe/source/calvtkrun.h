#ifndef CALVTKRUN_H
#define CALVTKRUN_H

extern "C" {
#include <OpenCAL/cal2D.h>
#include <OpenCAL/cal2DIO.h>
#include <OpenCAL/cal2DRun.h>
#include <OpenCAL/cal2DUnsafe.h>
}

#include "calvtkrender2d.h"
#include "calvtktimer.h"

#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMultiThreader.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkMutexLock.h>

typedef bool (*simulationFunction)();
class calvtkTimer;

class calvtkRun
{
    friend class calvtkAxes;
public:
    static calvtkRun* New();
    void Delete();

    void SetRender(calvtkRender2D* const render);
    calvtkRender2D* GetRender();
    void CreateRefreshRenderTimer(unsigned int milliseconds);
    void DestroyRefreshRenderTimer();
    void SetSimulationFunction(bool (*argFunction)());
    void SetCALRun(CALRun2D * calrun);
    CALRun2D* GetCALRun();
    void Initialize();
    void Start();

    vtkMutexLock* GetMutexLock();

protected:
    calvtkRun();
    ~calvtkRun();

    bool RunSimulationFunction();
private:
    static VTK_THREAD_RETURN_TYPE workerFunction(void * args);
    static void TerminateAppCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);
    simulationFunction simulation;

    int timerId;
    int threadId;

    calvtkRender2D* render;

    CALRun2D* calrun;

    calvtkTimer * timer;
    vtkCallbackCommand* terminateAppCallback;
    vtkRenderWindowInteractor*  renderInteractor;
    vtkInteractorStyleSwitch * style;
    vtkMultiThreader* threader;
    vtkMutexLock* mutex;
};
#endif // CALVTKRUN_H
