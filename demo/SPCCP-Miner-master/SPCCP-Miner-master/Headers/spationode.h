#ifndef SPATIONODE_H
#define SPATIONODE_H
#include <iostream>
#include <string>
#include <vector>
#include "road.h"
using namespace std;
class Road;
class SpatioNode
{
public:
    SpatioNode(string name);
    string roadName()const;
    string getInsName()const;
    Road* feature;
    int getTimeSpan()const;
    bool operator == (const SpatioNode& node)const;
    bool operator < (const SpatioNode& node)const;
private:
    string _roadName;
    int _timeSpan;  //属于的时间片(实例id)
    double _travelTime;  //通过时间
};

#endif // SPATIONODE_H
