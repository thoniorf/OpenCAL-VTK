#include "calvtkabstractlayer.h"

calvtkAbstractLayer::calvtkAbstractLayer()
{
    name = "";
    std::fill_n(extremes,2,0);
}

void calvtkAbstractLayer::SetName(std::string name)
{
    this->name = name;
}
std::string calvtkAbstractLayer::GetName()
{
    return name;
}

double* calvtkAbstractLayer::GetExtremes()
{
    return extremes;
}

calvtkAbstractLayer::~calvtkAbstractLayer()
{

}
