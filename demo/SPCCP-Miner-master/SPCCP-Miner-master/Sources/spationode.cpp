#include "spationode.h"

using namespace std;

SpatioNode::SpatioNode(string name){
    int index = 0;
    while(name[index] >= 'A' && name[index] <='Z'){
        index ++;
    }
    _roadName = name.substr(0, index);
    _timeSpan = stoi(name.substr(index, name.size()-index));
}
string SpatioNode::roadName()const{
    return _roadName;
}
string SpatioNode::getInsName()const{
    string str = _roadName + to_string(_timeSpan);
    return str;
}
int SpatioNode::getTimeSpan()const{
    return _timeSpan;
}
bool SpatioNode::operator == (const SpatioNode& node)const{
    return this->getInsName() == node.getInsName();
}

bool SpatioNode::operator < (const SpatioNode& node)const{
    return this->getInsName() < node.getInsName();
}

