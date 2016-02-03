#include "RiverineFloodModuleExport.h"
#include <iostream>

int main(int argv, char* argc[])
{
	int num_sc;
	int num_port;
	//char* base_dir = "C:\\Users\\a1091793\\Dropbox (WaterSystems)\\Inundation Database for Oct 20 DSS\\Database";
	char* base_dir = "C:\\Users\\a1091793\\Dropbox (WaterSystems)\\Inundation Database for Oct 20 DSS\\Database";
	char* out_map = "C:\\Users\\a1091793\\Desktop\\out.tif";
	initialise(base_dir, &num_sc, &num_port);
	int mitigation_opts[12] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	startOfRunI(1, mitigation_opts);
	get_mapC(20, 2016, out_map);
	int input;
	std::cin >> input;
}