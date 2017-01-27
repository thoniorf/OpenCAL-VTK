#ifndef CALVTKRUN_H
#define CALVTKRUN_H

#include "calvtkrender2d.h"

#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMultiThreader.h>

typedef bool (*simulationFunction)();
class calvtkRenderTimer : public vtkCommand
{
public :
    vtkTypeMacro(calvtkRenderTimer,vtkCommand)

    static calvtkRenderTimer *New()
    {
        return new calvtkRenderTimer;
    }

    void Execute(vtkObject *caller, unsigned long eventId,
                 void *vtkNotUsed(callData))
    {
        if(caller->IsA("vtkRenderWindowInteractor"))
        {
            vtkRenderWindowInteractor *renderWindow = static_cast<vtkRenderWindowInteractor*>(caller);

            if(vtkCommand::TimerEvent == eventId)
            {
                renderWindow->Render();
            }
        }
    }
};

class calvtkRun
{
public:
    static calvtkRun* New();
    void Delete();

    void SetRender(calvtkRender2D* const render);

    void CreateRefreshRenderTimer(unsigned int milliseconds);
    void SetSimulationFunction(bool (*argFunction)());
    void Initialize();
    void Start();

protected:
    calvtkRun();
    ~calvtkRun();

private:
    static VTK_THREAD_RETURN_TYPE workerFunction(void * args);
    static simulationFunction simulation;

    int timerId;
    int threadId;
    calvtkRenderTimer* renderTimer;
    vtkRenderWindowInteractor*  renderInteractor;
    vtkMultiThreader* threader;
};
#endif // CALVTKRUN_H
