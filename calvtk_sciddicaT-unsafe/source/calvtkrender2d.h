#ifndef CALVTKRENDER_H
#define CALVTKRENDER_H

#include "calvtklayer2d.h"
#include "calvtkscalarbar.h"
#include "calvtkaxes.h"

#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vector>
#include <string>

class calvtkRender2D
{
friend class calvtkRun;
public:
    calvtkRender2D(std::string windowName, int windowWidth, int windowHeight);
    ~calvtkRender2D();

    void AddLayer(calvtkLayer2D* const);
    calvtkLayer2D* RemoveLayer(int);

    void AddScalarBar(calvtkScalarBar * const scalarBar);
    void SetOutline();
    void SetAxes(calvtkAxes* const axes);

    void Inizialization();

    void Update();
private:
    std::vector<calvtkLayer2D*> layers;
    std::vector<calvtkScalarBar*> scalarbars;

    calvtkAxes* axes;

    vtkRenderer* renderer;
    vtkRenderWindow* renderWindow;


};
#endif // CALVTKRENDER_H
