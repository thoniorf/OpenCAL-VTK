#ifndef CALVTKRENDER_H
#define CALVTKRENDER_H

#include "calvtklayer2d.h"
#include "calvtkscalarbar.h"

#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vector>
#include <string>

class calvtkRender2D
{
friend class calvtkRun;
public:
    static calvtkRender2D* New();
    void Delete();

    void AddLayer(calvtkLayer2D* const);
    calvtkLayer2D* RemoveLayer(int);

    void AddScalarBar(calvtkScalarBar * const scalarBar);
    void SetOutline();

    void Inizialization();

    void Update();

    void Render();

    void SetBackgroundColor(double red, double green, double blue);

    void ResetCamera();
protected:
    calvtkRender2D(std::string windowName = "Window", int windowWidth = 600, int windowHeight = 600);
    ~calvtkRender2D();
private:
    std::vector<calvtkLayer2D*> layers;
    std::vector<calvtkScalarBar*> scalarbars;

    vtkRenderer* renderer;
    vtkRenderWindow* renderWindow;


};
#endif // CALVTKRENDER_H
