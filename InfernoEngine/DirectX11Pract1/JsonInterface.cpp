
#include"JsonInterface.h"
#include<fstream>
using jsoncons::json;
using jsoncons::pretty_print;

void SaveObjectToJson(json value,std::string fileName)
{
	std::ofstream file(fileName);	//json val = json::parse_string(input);

	file<< value << std::endl;
	file.close();
}

json LoadJsonFile(std::string fileName)
{
	json val = json::parse_file(fileName);

	return val;
	//std::cout << pretty_print(val) << std::endl;
}