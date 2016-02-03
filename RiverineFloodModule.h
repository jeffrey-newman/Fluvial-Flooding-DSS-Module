// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RIVERINEFLOODMODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RIVERINEFLOODMODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RIVERINEFLOODMODULE_EXPORTS
#define RIVERINEFLOODMODULE_API __declspec(dllexport)
#else
#define RIVERINEFLOODMODULE_API __declspec(dllimport)
#endif



// Used to initialise the library. Parses the database directory to make an internal representation of its structure.
RIVERINEFLOODMODULE_API int initialise(char* _database_directory, int * num_scenarios, int * num_portfolios);

////Used to retrieve the catchments in the database - to obntain their names and indexes used to identify them
//RIVERINEFLOODMODULE_API size_t catchments(int num_catchments, char** catchment_names, int** catchment_indices);

////USed to retrieve the scenarios in the database - to obtain their names and indices used to identify them
//RIVERINEFLOODMODULE_API size_t scenarios(int num_scenarios, char** scenario_names, int** scenario_indices);

//// Used to retrive the mitigation options in the database for a catchment.
//RIVERINEFLOODMODULE_API size_t mitigation_options(int catchment_indice, int num_options, char** options_names, int** options_indices);

// USed to pull the correct maps out of the database that will be needed during the model run. THese are stitched together
// at this stage. Takes c string representations of climate scenario and mitigation options.
RIVERINEFLOODMODULE_API int startOfRunC(char* climate_scenario, char* mitigation_option[]);

// USed to pull the correct maps out of the database that will be needed during the model run. THese are stitched together
// at this stage. Takes int representations of climate scenario and mitigation options.
RIVERINEFLOODMODULE_API int startOfRunI(int climate_scenario, int mitigation_option[]);

// USed to get an inundation map at a certain year for a 1:ari event.
RIVERINEFLOODMODULE_API int get_mapC(int ari, int year, char* map_file_path);

//Frees memory, closes down the library.
RIVERINEFLOODMODULE_API int close();
