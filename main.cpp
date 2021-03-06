//
//  main.cpp
//  DSSFloodModule
//
//  Created by a1091793 on 9/09/2015.
//  Copyright (c) 2015 University of Adelaide. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "Types.h"
#include "ProcessDatabase.h"
#include "ReadInMap.h"


int main(int argc, const char * argv[]) {

    namespace prog_opt = boost::program_options;
    namespace file_sys = boost::filesystem;
    
    ///////////////////////////////////////////////////////////////////////////
    ///                                                                     ///
    ///    PROGRAM OPTIONS                                                  ///
    ///                                                                     ///
    ///////////////////////////////////////////////////////////////////////////
    
    std::string base_dir_str;
    std::string output_path_str;
	bool use_int_signals;
	int climate_signal;
	std::vector<int> portfolio_signal;
	int ari_signal;
	int year;
    
    
    prog_opt::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("base-directory,d", prog_opt::value<std::string>(&base_dir_str), "The base directory for the map database")
	("climate-signal,c", prog_opt::value<int>(&climate_signal)->default_value(1), "The climate change projection that should be used")
	("portfolio-signal,p", prog_opt::value<std::vector<int> >(&portfolio_signal), "The mitigation portfolios that should be used. Need to specify exactly the number of portfolios as there are catchments")
	("ARI,r", prog_opt::value<int>(&ari_signal)->default_value(1), "The 1:ari value indicating expected frequency of flood hazard")
	("year,y", prog_opt::value<int>(&year)->default_value(1), "specify year in which to calculate flood hazard for")
    ("output,o", prog_opt::value<std::string>(&output_path_str)->default_value("output.tif"), "The path to print output interpolated map to")
	("int-signal,i", prog_opt::value<bool>(&use_int_signals)->default_value(true), "Whether to use string (false) or int (true) representation of mitigation and climate scanario signals");

	prog_opt::positional_options_description p;
	p.add("portfolio-signal", -1);

    prog_opt::variables_map vm;
	prog_opt::store(prog_opt::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    prog_opt::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }
	if (vm.count("portfolio-signal"))
	{
		std::cout << "Mitigation portfolios are: ";
		BOOST_FOREACH(int portfolio, portfolio_signal)
			{
			std::cout << portfolio << "  ";
			}
		std::cout << "\n";
	}
    
    file_sys::path base_dir(base_dir_str);
    //Check if base directory exists
    if (!file_sys::exists(base_dir))
    {
        std::cerr << "Base directory of database does not exist" << std::endl;
        //Could throw an error here instead
    }

	
    
	GDALAllRegister(); //This registers all availble raster file formats for use with this utility. How neat is that. We can input any GDAL supported rater file format.
    Database my_data;
    processDatabase(my_data, base_dir);
	fs::path template_map = base_dir / "template.tif";
	file_sys::path output_path(output_path_str);
	stitchmaps(climate_signal, year, ari_signal, portfolio_signal, my_data, template_map, output_path, GDT_Float32);



	////Read in template map.
	//
	//std::tuple<Map_Double_SPtr, std::string, GeoTransform> gdal_map = read_in_map<double>(template_map, GDT_Float64, NO_CATEGORISATION);
	//Map_Double_SPtr map(std::get<0>(gdal_map));
	//std::string & map_WKT_projection = (std::get<1>(gdal_map));
	//GeoTransform & map_transform = (std::get<2>(gdal_map));
	//int nrows = map->NRows();
	//int ncols = map->NCols();
	//double no_data_val = map->NoDataValue();

	////Set number of mitigaiton portfolios and scenarios
	//int num_of_scenarios = my_data.get_catchment(1)->clim_scnrios_ik.size();
	//int num_of_portfolios = my_data.get_catchment(1)->get_scenario(1)->mit_prtflios_ik.size();
	//int num_of_catchments = my_data.catchments_ik.size();

 //   StitchedMapSet maps;

	//if (use_int_signals == true)
	//{
	//	std::vector<int> portfolios(num_of_catchments);
	//	for (int i = 0; i < num_of_catchments; ++i)
	//	{
	//		portfolios[i] = std::stoi(portfolio_signal[i]);
	//	}
	//	int scenario = std::stoi(climate_signal);
	//	getMapSet(scenario, portfolios, my_data, maps, ncols, nrows, no_data_val);
	//}
	//else
	//{
	//	getMapSet(climate_signal, portfolio_signal, my_data, maps, ncols, nrows, no_data_val);
	//}

 //   
 //   
 //   Map_Float_SPtr interped_map = interpolatedmap(ari_signal, year, maps);
 //   
 //   /********************************************/
 //   /* Print resultent map                      */
 //   /********************************************/
 //   
 //   
 //   
 //   std::cout << "\n\n*************************************\n";
 //   std::cout << "*         Saving output file        *\n";
 //   std::cout << "*************************************" << std::endl;
 //   std::string driverName = "GTiff";
	//write_map(output_path, GDT_Float32, interped_map, map_WKT_projection, map_transform, driverName);
 //   
 //   
 //   std::cout << "finished processing database" << std::endl;
 //   std::cout << "Another breakpoint" << std::endl;
}


