#include "calvtkabstractlayer.h"

calvtkAbstractLayer::calvtkAbstractLayer()
{
    name = "";
    std::fill_n(extremes,2,0);
}

calvtkAbstractLayer::~calvtkAbstractLayer()
{

}

