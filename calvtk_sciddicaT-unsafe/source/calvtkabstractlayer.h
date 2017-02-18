#ifndef CALVTKABSTRACTLAYER_H
#define CALVTKABSTRACTLAYER_H

extern "C" {
#include <OpenCAL/cal2D.h>
#include <OpenCAL/cal2DIO.h>
}

#include <vtkAbstractMapper.h>
#include <vtkActor.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>

#include <algorithm>
#include <string>

class calvtkAbstractLayer
{
public:
    calvtkAbstractLayer();
    ~calvtkAbstractLayer();

    std::string GetLayerName();
    void SetName(std::string name);

    virtual void GenerateDataSet() = 0;

    virtual void GenerateScalarValues() = 0;

    virtual void UpdateScalarValues() = 0;

    virtual void Update() = 0;

    double* GetExtremes();

protected:
    virtual void ComputeExtremes() = 0;

    double extremes[2];

    std::string name;

};

#endif // CALVTKABSTRACTLAYER_H
