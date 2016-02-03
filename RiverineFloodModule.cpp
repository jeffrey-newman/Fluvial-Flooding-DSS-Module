// RiverineFloodModule.cpp : Defines the exported functions for the DLL application.
//

#include <boost/filesystem.hpp>
#include "RiverineFloodModuleExport.h"
#include "Types.h"
#include "ProcessDatabase.h"

namespace fs = boost::filesystem;

Database my_data;
StitchedMapSet maps;
fs::path base_dir;
std::string map_WKT_projection;
GeoTransform map_transform;
int nrows;
int ncols;
double no_data_val;
std::string driverName = "GTiff";
int num_of_scenarios; 
int num_of_portfolios;
int num_of_catchments;

// Used to initialise the library. Parses the database directory to make an internal representation of its structure.
// Input - the directory at the top level of the database.
// Return codes: 0 - success
//               1 - directory does not exist
int initialise(char* _database_directory, int * num_scenarios, int * num_portfolios)
{
	base_dir = _database_directory;
	if (!fs::exists(base_dir))
	{
		return (1);
	}
	processDatabase(my_data, base_dir);

	//Read in template map.
	fs::path template_map = base_dir / "template.tif";
	std::tuple<Map_Double_SPtr, std::string, GeoTransform> gdal_map = read_in_map<double>(template_map, GDT_Float64, NO_CATEGORISATION);
	Map_Double_SPtr map(std::get<0>(gdal_map));
	map_WKT_projection = (std::get<1>(gdal_map));
	map_transform = (std::get<2>(gdal_map));
	nrows = map->NRows();
	ncols = map->NCols();
	no_data_val = map->NoDataValue();

	//Set number of mitigaiton portfolios and scenarios
	num_of_scenarios = my_data.get_catchment(1)->clim_scnrios_ik.size();
	num_of_portfolios = my_data.get_catchment(1)->get_scenario(1)->mit_prtflios_ik.size();
	num_of_catchments = my_data.catchments_ik.size();
	return (0);
}

// USed to pull the correct maps out of the database that will be needed during the model run. THese are stitched together
// at this stage. Takes c string representations of climate scenario and mitigation options.
// Return value - 0 - success.
int startOfRunC(char* climate_scenario, char* mitigation_option[])
{
	std::vector<std::string> mitigation_options_v(num_of_catchments);
	for (int i = 0; i < num_of_catchments; ++i)
	{
		mitigation_options_v[i] = std::string(mitigation_option[i]);
	}
	std::string climate_scenario_s(climate_scenario);
	getMapSet(climate_scenario_s, mitigation_options_v, my_data, maps, ncols, nrows, no_data_val);
	return (0);
}


// USed to pull the correct maps out of the database that will be needed during the model run. THese are stitched together
// at this stage. Takes int representations of climate scenario and mitigation options.
int startOfRunI(int climate_scenario, int mitigation_option[])
{
	std::vector<int> mitigation_options_v(num_of_catchments);
	for (int i = 0; i < num_of_catchments; ++i)
	{
		mitigation_options_v[i] = mitigation_option[i];
	}
	getMapSet(climate_scenario, mitigation_options_v, my_data, maps, ncols, nrows, no_data_val);
	return (0);
}

// USed to get an inundation map at a certain year for a 1:ari event.
int get_mapC(int ari, int year, char* map_file_path)
{
	Map_Float_SPtr interped_map = interpolatedmap(ari, year, maps);
	file_sys::path output_path(map_file_path);
	write_map(output_path, GDT_Float32, interped_map, map_WKT_projection, map_transform, driverName);
	return (0);
}

//Frees memory, closes down the library.
int close()
{
	return (0);
}