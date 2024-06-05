#ifndef ROAD_H
#define ROAD_H
#include <iostream>
#include <string>
#include <vector>
#include "spationode.h"
using namespace std;
class SpatioNode;
class Road
{
public:
    Road(string roadName);
    static double _timeSpan;
    bool operator < (const Road& r)const;
    bool operator ==(const Road& r)const;
    // void operator = ( Road& r)const;
    // void addInstance(SpatioNode* ins);
    string name()const;
    vector<SpatioNode*> congestionTimes;
    vector<string> _neighbor;
private:
    double _freeV; //自由流速
    string _roadName;  //道路名（特征)
    // vector< Road > _neiborRoad;
};

#endif // ROAD_H
