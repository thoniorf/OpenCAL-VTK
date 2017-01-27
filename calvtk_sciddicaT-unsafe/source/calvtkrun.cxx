#include "calvtkrun.h"
simulationFunction calvtkRun::simulation;

VTK_THREAD_RETURN_TYPE calvtkRun::workerFunction(void *args)
{
    bool again = false;
    do{
        again = calvtkRun::simulation();
    }while(again);

    return VTK_THREAD_RETURN_VALUE;
}
calvtkRun* calvtkRun::New()
{
    return new calvtkRun();
}
void calvtkRun::Delete()
{
    delete this;
}

void calvtkRun::SetRender(calvtkRender2D * const render)
{
    renderInteractor->SetRenderWindow(render->renderWindow);
}

void calvtkRun::CreateRefreshRenderTimer(unsigned int milliseconds)
{
    renderTimer = calvtkRenderTimer::New();
    renderInteractor->AddObserver(vtkCommand::TimerEvent,renderTimer);
    timerId = renderInteractor->CreateRepeatingTimer(milliseconds);
}

void calvtkRun::SetSimulationFunction(simulationFunction function)
{
    calvtkRun::simulation = function;
}

void calvtkRun::Initialize()
{
    renderInteractor->Initialize();
}

void calvtkRun::Start()
{
    threadId = threader->SpawnThread(workerFunction,NULL);
    renderInteractor->Start();
}

calvtkRun::calvtkRun()
{
    renderInteractor = vtkRenderWindowInteractor::New();
    threader = vtkMultiThreader::New();
}

calvtkRun::~calvtkRun()
{
    threader->TerminateThread(threadId);
    threader->Delete();
    renderInteractor->Delete();
}
