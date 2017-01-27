#include "calvtkrender2d.h"
calvtkRender2D::calvtkRender2D(std::string windowName, int windowWidth, int windowHeight)
{
    renderer = vtkRenderer::New();

    renderWindow = vtkRenderWindow::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName(windowName.c_str());
    renderWindow->SetSize(windowWidth,windowHeight);
    renderWindow->SetMultiSamples(0);

}
calvtkRender2D::~calvtkRender2D()
{
    axes->Delete();
    renderWindow->Delete();
    renderer->Delete();
}

void calvtkRender2D::AddLayer(calvtkLayer2D* const layer)
{
    layers.push_back(layer);
}

calvtkLayer2D* calvtkRender2D::RemoveLayer(int layer_id)
{
    return *(layers.erase(layers.begin()+layer_id));
}

void calvtkRender2D::AddScalarBar(calvtkScalarBar* const scalarBar)
{
    scalarbars.push_back(scalarBar);
    renderer->AddActor2D(scalarBar);
}

void calvtkRender2D::SetAxes(calvtkAxes * const axes)
{
    this->axes = axes;
    renderer->AddActor(axes);
}

void calvtkRender2D::Inizialization()
{
    for(int i = 0; i < layers.size(); i++)
    {
        calvtkLayer2D * layer = layers[i];
        renderer->AddActor(layer->GetActor());
    }
}

void calvtkRender2D::Update()
{
    for(int i = 0; i < layers.size(); i++)
    {
        layers[i]->Update();
    }
}
