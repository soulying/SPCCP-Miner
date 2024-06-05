#include "road.h"

Road::Road(string roadName):_roadName(roadName)
{

}

string Road:: name()const{
    return _roadName;
}
bool  Road::operator < (const Road& r)const{
    return this->_roadName < r._roadName;
}
bool  Road::operator == (const Road& r)const{
    return this->_roadName == r._roadName;
}
