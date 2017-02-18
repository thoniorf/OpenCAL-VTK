#include "calvtkrun.h"

VTK_THREAD_RETURN_TYPE calvtkRun::workerFunction(void *arg)
{
    calvtkRun* run = static_cast<calvtkRun*>(static_cast<vtkMultiThreader::ThreadInfo*>(arg)->UserData);
    time_t start_time, end_time;
    bool again = false;
    cout<<"Starting simulation...\n";
    start_time = time(NULL);
    // applies the callback init func registered by calRunAddInitFunc2D()
    calRunInitSimulation2D(run->GetCALRun());

    clock_t start = clock();
    clock_t end = start;
    do{
        //run->GetMutexLock()->Lock();
        again = run->RunSimulationFunction();
        //run->GetMutexLock()->Unlock();
        double time = (double) (end - start) / CLOCKS_PER_SEC * 1000.0;
        if(time >= 500){
            run->GetRender()->Update();
            start = clock();
        } else {
            end = clock();
        }
    }while(again);

    calRunFinalizeSimulation2D(run->GetCALRun());
    end_time = time(NULL);
    cout<<"Simulation terminated."<<endl<<"Elapsed time: "<<end_time-start_time<<endl;

    return VTK_THREAD_RETURN_VALUE;
}
calvtkRun* calvtkRun::New()
{
    return new calvtkRun();
}
void calvtkRun::Delete()
{
    mutex->Delete();
    threader->TerminateThread(threadId);
    threader->Delete();
    renderInteractor->Delete();
}

void calvtkRun::SetRender(calvtkRender2D * const render)
{
    this->render = render;
    renderInteractor->SetRenderWindow(render->renderWindow);
}
calvtkRender2D* calvtkRun::GetRender()
{
    return render;
}

void calvtkRun::CreateRefreshRenderTimer(unsigned int milliseconds)
{
    timer = calvtkTimer::New();
    timer->SetRender(render);
    timer->SetRun(this);
    renderInteractor->AddObserver(vtkCommand::TimerEvent,timer);
    timerId = renderInteractor->CreateRepeatingTimer(milliseconds);
}

void calvtkRun::SetSimulationFunction(simulationFunction function)
{
    calvtkRun::simulation = function;
}
void calvtkRun::SetCALRun(CALRun2D* calrun)
{
    this->calrun = calrun;
}
CALRun2D* calvtkRun::GetCALRun()
{
    return calrun;
}

void calvtkRun::Initialize()
{
    renderInteractor->Initialize();
}

void calvtkRun::Start()
{
    threadId = threader->SpawnThread(workerFunction,this);
    renderInteractor->Start();
}
vtkMutexLock* calvtkRun::GetMutexLock()
{
    return mutex;
}

calvtkRun::calvtkRun()
{
    mutex = vtkMutexLock::New();
    style = vtkInteractorStyleSwitch::New();
    style->SetCurrentStyleToTrackballCamera();
    renderInteractor = vtkRenderWindowInteractor::New();
    renderInteractor->SetInteractorStyle(style);
    threader = vtkMultiThreader::New();
}

calvtkRun::~calvtkRun()
{

}

bool calvtkRun::RunSimulationFunction()
{
    return simulation();
}
