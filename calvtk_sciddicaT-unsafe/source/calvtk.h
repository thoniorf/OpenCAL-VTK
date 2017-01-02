#ifndef CALVTK_H
#define CALVTK_H

extern "C" {
#include <OpenCAL/cal2D.h>
#include <OpenCAL/cal2DIO.h>
}

#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCellData.h>
#include <vtkCellDataToPointData.h>
#include <vtkClipPolyData.h>
#include <vtkCommand.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkGenericDataArray.h>
#include <vtkLookupTable.h>
#include <vtkPlane.h>
#include <vtkPlaneSource.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyle.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkWarpScalar.h>
#include <vtkMutexLock.h>

#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
#include <mutex>

#define VTK_DEF(type,name) vtkSmartPointer<type> name
#define VTK_INI(type,name) name = vtkSmartPointer<type>::New()

#define Z_OFFSET_STEP 100

enum calvtkBaseRendering { CALVTK_RENDER2D,CALVTK_RENDER3D,CALVTK_VOLUMERENDER,CALVTK_SURFACERENDER};
class CALVTKRenderingTimer : public vtkCommand
{
public:
    vtkTypeMacro(CALVTKRenderingTimer,vtkCommand)

    static CALVTKRenderingTimer *New()
    {
        return new CALVTKRenderingTimer;
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
private:
    vtkMutexLock* renderLock;

};

class CALVTKRender
{

public:
    CALVTKRender(CALModel2D *model, int cellsize);
    ~CALVTKRender();

    void calvtkSetCALModel(CALModel2D* model);
    void calvtkAddLayer(CALSubstate2Dr* substate);
    void calvtkAddLayer(CALSubstate2Dr* substate,std::string);

    int calvtkGetLayerId(CALSubstate2Dr* substate);
    void calvtkGenerateNextLayerPolyData();
    void calvtkGenerateAllLayersPolyData();

    void calvtkGenerateLayerScalarValues(int layer_id);
    void calvtkGenerateAllLayersScalarsValues();
    void calvtkUpdateLayerScalarValues(int layer_id);

    void calvtkGenerateLayerLookupTable(int layer_id);
    void calvtkGenerateAllLayerLookupTable();
    void calvtkBuildAllLayerLookupTable();
    void calvtkUpdateLayerLookupTable(int layer_id);

    void calvtkSetLayerHueRange(int layer_id,double minHue,double maxHue);
    void calvtkSetLayerSaturationRange(int layer_id,double minSaturation,double maxSaturation);
    void calvtkSetLayerValueRange(int layer_id,double minValue,double maxValue);
    void calvtkSetLayerBelowColor(int layer_id,double red,double green,double blue,double alpha);
    void calvtkSetLayerAboveColor(int layer_id,double red,double green,double blue,double alpha);

    void calvtkSetBaseRendering(calvtkBaseRendering baseRender);

    void calvtkAddScalarBar(CALSubstate2Dr* substate);
    void calvtkAddOutLine();
    void calvtkAddAxes();
    void calvtkClipNoDataScalarsValues();
    void calvtkWarpScalar();

    void calvtkRenderInizialization();
    void calvtkRenderInizialization(unsigned long renderingTimerDuration);

    void Update();
    void Render();
    void Start();

    void Inizialize();

private:

    void ComputeExtremes(CALModel2D* ca2D,CALSubstate2Dr *substate,double *m,double *M);
    void ClipPolyData();

    double h_min,h_Max,z_min,z_Max;
    double scaleFactor;

    double z_offset;

    int cellsize;
    int cols;
    int rows;

    CALModel2D *model;

    calvtkBaseRendering baseRendering;

    std::vector<double*> extremes;
    std::vector<std::string> layersNames;
    std::vector<vtkLookupTable*> lookupTables;
    std::vector<CALSubstate2Dr*> layers;
    std::vector<vtkPolyData*> datasets;
    std::vector<vtkPolyDataMapper*> mappers;
    std::vector<vtkActor*> actors;

    vtkAxesActor* axes;

    vtkRenderer* renderer;
    vtkRenderWindow* renderWindow;
    vtkRenderWindowInteractor* renderWindowInteractor;

    CALVTKRenderingTimer* renderingTimer;

};

#endif // CALVTK_H
