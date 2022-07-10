#ifndef JsonInterface_H_
#define JsonInterface_H_

#include "jsoncons/json.hpp"

using jsoncons::json;

void SaveObjectToJson(json object,std::string fileName);
json LoadJsonFile(std::string fileName);


#endif 