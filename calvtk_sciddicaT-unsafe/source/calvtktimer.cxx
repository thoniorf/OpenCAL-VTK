#include "calvtktimer.h"
calvtkTimer* calvtkTimer::New()
{
    return new calvtkTimer;
}

void calvtkTimer::Execute(vtkObject *caller, unsigned long eventId,void * callData)
{
    if(caller->IsA("vtkRenderWindowInteractor"))
    {
        vtkRenderWindowInteractor* renderWindow = static_cast<vtkRenderWindowInteractor*>(caller);

        if(vtkCommand::TimerEvent == eventId)
        {
            run->GetMutexLock()->Lock();
            render->Render();
            render->Update();
            run->GetMutexLock()->Unlock();
        }
    }
}

void calvtkTimer::SetRender(calvtkRender2D * const render)
{
    this->render = render;
}
void calvtkTimer::SetRun(calvtkRun * const run)
{
    this->run = run;
}
