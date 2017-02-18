#ifndef CALVTKTIMER_H
#define CALVTKTIMER_H

#include "calvtkrender2d.h"
#include "calvtkrun.h"

#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
class calvtkRun;

class calvtkTimer : public vtkCommand
{
public :
    vtkTypeMacro(calvtkTimer,vtkCommand)

    static calvtkTimer *New();

    void Execute(vtkObject *caller, unsigned long eventId,void *callData);

    void SetRender(calvtkRender2D * const render);
    void SetRun(calvtkRun* const run);

protected :
    calvtkRender2D * render;
    calvtkRun* run;

};


#endif // CALVTKTIMER_H
