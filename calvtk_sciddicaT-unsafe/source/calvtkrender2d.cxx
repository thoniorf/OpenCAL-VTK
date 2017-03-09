#include "calvtkrender2d.h"

calvtkRender2D* calvtkRender2D::New()
{
    return new calvtkRender2D();
}
void calvtkRender2D::Delete()
{
    renderWindow->Delete();
    renderer->Delete();
    layers.clear();
    scalarbars.clear();
}

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
    renderWindow->Delete();
    renderer->Delete();
    layers.clear();
    scalarbars.clear();

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
void calvtkRender2D::SetOutline(calvtkOutline * const outline)
{
    this->outline = outline;
    renderer->AddActor(outline->actor);
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
void calvtkRender2D::Render()
{
    renderWindow->Render();
}

void calvtkRender2D::SetWindowName(std::string name)
{
    renderWindow->SetWindowName(name.c_str());
}

void calvtkRender2D::SetWindowSize(int width, int height)
{
    renderWindow->SetSize(width,height);
}

void calvtkRender2D::SetBackgroundColor(double red, double green, double blue)
{
    renderer->SetBackground(red,green,blue);
}

void calvtkRender2D::ResetCamera()
{
    renderer->ResetCamera();
}
