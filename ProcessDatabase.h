//
//  ProcessDatabase.h
//  DSSFloodModule
//
//  Created by a1091793 on 11/09/2015.
//  Copyright (c) 2015 University of Adelaide. All rights reserved.
//

#ifndef DSSFloodModule_ProcessDatabase_h
#define DSSFloodModule_ProcessDatabase_h

#include <set>
#include <tuple>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include "Types.h"
#include "DirectoryNameParser.h"
#include "MapTypes.h"
#include "ReadInMap.h"

std::string catchment_prefix("Catchment");
std::string climate_prefix("Climate Scenario");
std::string mitigation_prefix("Mitigation Portfolio");
std::string return_prefix("Return Interval");
std::string time_prefix("Time Slice");

bool isHidden(const boost::filesystem::path &p)
{
	std::string name = p.filename().string();
	if (name != ".." &&
		name != "."  &&
		name[0] == '.')
	{
		return true;
	}

	return false;
}

void
processDatabase(Database & my_data, file_sys::path const & base_dir)
{
    namespace file_sys = boost::filesystem;
    
    // Iterate through Catchments
    file_sys::directory_iterator catch_it(base_dir), catch_eod;
    BOOST_FOREACH(file_sys::path const & catch_dir, std::make_pair(catch_it, catch_eod))
    {
        
        if(file_sys::is_directory(catch_dir) && fs::exists(catch_dir/"index.txt") && fs::exists(catch_dir/"name.txt"))
        {
            
            Catchment_SPtr catchment(new Catchment);
            std::pair<int, std::string> id;
            std::string dir_stem = catch_dir.filename().string();
            //bool parse_success = parse_dir_name(dir_stem.begin(), dir_stem.end(), catchment_prefix, id);
			bool parse_success = parse_dir_name2(catch_dir, id);
            if (!parse_success)
            {
                std::cerr << "error parsing catchment directory name: " << catch_dir << " Got: " << id.first << ", " << id.second << std::endl;
            }
            
            my_data.catchments_ik.insert(std::make_pair(id.first, catchment));
            my_data.catchments_sk.insert(std::make_pair(id.second, catchment));
            
            
            //Open the file that contains the coordinates.
            std::string dummystr;
            file_sys::path coord_path = catch_dir / "coordinates.txt";
            if (!file_sys::is_regular_file(coord_path))
            {
                std::cerr << "error: did not find coordinate file for catchment " << id.second << ". Looked in: " << coord_path << std::endl;
            }
            std::ifstream coords(coord_path.c_str());
            coords >> dummystr >> catchment->number_cols;
            coords >> dummystr >> catchment->number_rows;
            coords >> dummystr >> catchment->begin_col;
            coords >> dummystr >> catchment->begin_row;
            
            //Iterate through Climate Scenarios
            file_sys::directory_iterator clim_it(catch_dir), clim_eod;
            BOOST_FOREACH(file_sys::path const & clim_dir, std::make_pair(clim_it, clim_eod))
            {
				if (file_sys::is_directory(clim_dir) && fs::exists(clim_dir / "index.txt") && fs::exists(clim_dir / "name.txt"))
                {
                    
                    ClimateScenario_SPtr scenario(new ClimateScenario);
                    std::pair<int, std::string> id;
                    std::string dir_stem = clim_dir.filename().string();
                    //bool parse_success = parse_dir_name(dir_stem.begin(), dir_stem.end(), climate_prefix, id);
					bool parse_success = parse_dir_name2(clim_dir, id);
                    if (!parse_success)
                    {
                        std::cerr << "error parsing climate scenario directory name: " << clim_dir << " Got: " << id.first << ", " << id.second << std::endl;
                    }
                    catchment->clim_scnrios_ik.insert(std::make_pair(id.first, scenario));
                    catchment->clim_scnrios_sk.insert(std::make_pair(id.second, scenario));
                    
                    
                    //Iterate through Mitigation portfolios
                    file_sys::directory_iterator mit_it(clim_dir), mit_eod;
                    BOOST_FOREACH(file_sys::path const & mit_dir, std::make_pair(mit_it, mit_eod))
                    {
						if (file_sys::is_directory(mit_dir) && fs::exists(mit_dir / "index.txt") && fs::exists(mit_dir / "name.txt"))
                        {
                            
                            MitigationPortfolio_SPtr portfolio(new MitigationPortfolio);
                            std::pair<int, std::string> id;
                            std::string dir_stem = mit_dir.filename().string();
                            //bool parse_success = parse_dir_name(dir_stem.begin(), dir_stem.end(), mitigation_prefix, id);
							bool parse_success = parse_dir_name2(mit_dir, id);
                            if (!parse_success)
                            {
                                std::cerr << "error parsing mitigation portfolio directory name: " << mit_dir << " Got: " << id.first << ", " << id.second << std::endl;
                            }
                            scenario->mit_prtflios_ik.insert(std::make_pair(id.first, portfolio));
                            scenario->mit_prtflios_sk.insert(std::make_pair(id.second, portfolio));
                            
                            //Iterate through return intervals
                            file_sys::directory_iterator return_int_it(mit_dir), return_int_eod;
                            BOOST_FOREACH(file_sys::path const & return_int_dir, std::make_pair(return_int_it, return_int_eod))
                            {
                                
								if (file_sys::is_directory(return_int_dir) && fs::exists(return_int_dir / "index.txt") && fs::exists(return_int_dir / "name.txt"))
                                {
                                    ReturnInterval_SPtr interval(new ReturnInterval);
                                    std::pair<int, std::string> id;
                                    std::string dir_stem = return_int_dir.filename().string();
                                    //bool parse_success = parse_dir_name(dir_stem.begin(), dir_stem.end(), return_prefix, id);
									bool parse_success = parse_dir_name2(return_int_dir, id);
                                    if (!parse_success)
                                    {
                                        std::cerr << "error parsing return interval directory name: " << return_int_dir << " Got: " << id.first << ", " << id.second << std::endl;
                                    }
                                    portfolio->rtrn_intrvls_ik.insert(std::make_pair(id.first, interval));
                                    portfolio->rtrn_intrvls_sk.insert(std::make_pair(id.second, interval));
                                    
                                    // Iterate through time slices
                                    file_sys::directory_iterator time_it(return_int_dir), time_eod;
                                    BOOST_FOREACH(file_sys::path const & time_dir, std::make_pair(time_it, time_eod))
                                    {
										if (file_sys::is_directory(time_dir) && fs::exists(time_dir / "index.txt") && fs::exists(time_dir / "name.txt"))
                                        {
                                            //Iterate through files, finding the last modified file.
                                            std::time_t most_recent_write_time(0);
                                            file_sys::path most_recent_map(time_dir);
                                            file_sys::directory_iterator map_it(time_dir), map_eod;
                                            BOOST_FOREACH(file_sys::path const & map_file, std::make_pair(map_it, map_eod))
                                            {
                                                
												if (file_sys::is_regular_file(map_file) && (map_file != (time_dir / "index.txt")) && (map_file != (time_dir / "name.txt")) && !isHidden(map_file))
                                                {
                                                    std::time_t last_write = file_sys::last_write_time(map_file);
                                                    if (most_recent_write_time < last_write)
                                                    {
                                                        most_recent_write_time = last_write;
                                                        most_recent_map = map_file;
                                                    }
                                                }
                                            }
                                            if (most_recent_map == time_dir)
                                            {
                                                std::cerr << "Error in finding most recent flood map in " << time_dir << std::endl;
                                            }
                                            Path_SPtr path(new file_sys::path(most_recent_map));
                                            // Find the name and ID of the time slice.
                                            std::pair<int, std::string> id;
                                            std::string dir_stem = time_dir.filename().string();
                                            //bool parse_success = parse_dir_name(dir_stem.begin(), dir_stem.end(), time_prefix, id);
											bool parse_success = parse_dir_name2(time_dir, id);
                                            if (!parse_success)
                                            {
                                                std::cerr << "error parsing time slice directory name: " << time_dir << " Got: " << id.first << ", " << id.second << std::endl;
                                            }
                                            //Insert into the data structure
                                            interval->time_slices_ik.insert(std::make_pair(id.first, path));
                                            interval->time_slices_sk.insert(std::make_pair(id.second, path));
                                        }
                                    }
                                    //Insert timeslices int return intervals structure
                                }
                            } // end for each return Intervals
                        }
                    } // end for each mitigation portfolios
                }
            } // end for each climate scenarios
        }
    } // end for each catchments
    
}


struct CatchmentMaps
{
    std::map<int, std::map<int, Path_SPtr> > map_paths;   // First index is return interval, second is timestamp.
    int begin_col;
    int begin_row;
    int number_rows;
    int number_cols;
    
};


const float MODELLING_DOMAIN_MARKER = 9999.9;

void stitchmaps(int _scenario, int year, int return_int, std::vector<int> & _portfolio, Database & _data, file_sys::path template_path, file_sys::path file_path, GDALDataType data_type)
{
	//GDALAllRegister(); //This registers all availble raster file formats for use with this utility. How neat is that. We can input any GDAL supported rater file format.
	GDALDriver * poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	if (poDriver == NULL)
	{
		throw std::runtime_error("No driver for file style found");
	}

	char **papszOptions = NULL;
	papszOptions = CSLSetNameValue(papszOptions, "COMPRESS", "LZW");
	//poDriver->CopyFiles(file_path.string().c_str(), template_path.string().c_str());
	//GDALDataset * poDstDS = (GDALDataset *)GDALOpen(file_path.string().c_str(), GA_Update);

	GDALDataset *poSrcDS =
		(GDALDataset *)GDALOpen(template_path.string().c_str(), GA_ReadOnly);
	GDALDataset *poDstDS;
	poDstDS = poDriver->CreateCopy(file_path.string().c_str(), poSrcDS, FALSE,
		papszOptions, NULL, NULL);
	GDALClose((GDALDatasetH)poSrcDS);
	if (poDstDS == NULL) throw std::runtime_error("Unable to createcopy template map");
	GDALRasterBand * poBand = poDstDS->GetRasterBand(1);

	// Get Maps
	std::map<int, CatchmentMaps> map_set;
	typedef std::pair<const int, Catchment_SPtr> CatchmentPair;
	int i = 0;
	BOOST_FOREACH(CatchmentPair & ctchmnt, _data.catchments_ik)
	{
		Path_SPtr map_path = ctchmnt.second->get_scenario(_scenario)->get_portfolio(_portfolio[i])->get_interval(return_int)->time_slices_ik[year];
		std::string wKTprojection;
		GeoTransform transform;
		Map_Float_SPtr map;
		std::tie(map, wKTprojection, transform) = read_in_map<float>(*map_path, GDT_Float32, NO_CATEGORISATION);

		int begin_col = ctchmnt.second->begin_col;
		int begin_row = ctchmnt.second->begin_row;

		// Note: Map Matrixes indexes are in opposite order to C arrays. e.g. map matrix is indexed by (row, Col) which is (y, x) and c matrices are done by (x, y) which is (Col, Row)
		boost::shared_ptr<Map_Matrix<float> > compare_map(new Map_Matrix<float>((int)map->NRows(), (int)map->NCols()));
		//get a c array of this size and read into this.
		float * pafScanline1 = compare_map->get_data_ptr();
		//pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
		poBand->RasterIO(GF_Read, begin_col, begin_row, (int)map->NCols(), (int)map->NRows(),
			pafScanline1, (int)map->NCols(), (int)map->NRows(), data_type,
			0, 0);
		int pbsuccess; // can be used with get no data value
		compare_map->SetNoDataValue(poBand->GetNoDataValue(&pbsuccess));

		for (unsigned int i = 0; i < map->NRows(); ++i)
		{
			for (unsigned int j = 0; j < map->NCols(); ++j)
			{
				if (compare_map->Get(i, j) != compare_map->NoDataValue() && compare_map->Get(i, j) != MODELLING_DOMAIN_MARKER)
				{
					if (map->Get(i, j) != map->NoDataValue())
					{
						if (compare_map->Get(i, j) > map->Get(i, j)) map->Get(i, j) = compare_map->Get(i, j);
					}
					if (map->Get(i, j) == map->NoDataValue())
					{
						map->Get(i, j) = compare_map->Get(i, j);
					}
				}
				if (map->Get(i, j) == map->NoDataValue())
				{
					map->Get(i, j) = compare_map->NoDataValue();
				}
			}
		}
		
		float * pafScanline2 = map->get_data_ptr();
		poBand->RasterIO(GF_Write, begin_col, begin_row, (int)map->NCols(), (int)map->NRows(), pafScanline2, (int)map->NCols(), (int)map->NRows(), data_type, 0, 0);
	}

	/* Once we're done, close properly the dataset */
	if (poDstDS != NULL)
		GDALClose((GDALDatasetH)poDstDS);
}


void
getMapSet(int _scenario, std::vector<int> _portfolio, Database & _data, StitchedMapSet & set, unsigned int n_cols, unsigned int n_rows, float null_value)
{

    std::set<int> time_slices_set;
    std::set<int> return_intervals_set;
    std::set<int> catchments_set;
    
    // Get Maps
    std::map<int, CatchmentMaps> map_set;
    typedef std::pair<const int, Catchment_SPtr> CatchmentPair;
	int i = 0;
    BOOST_FOREACH(CatchmentPair & ctchmnt, _data.catchments_ik)
    {
        CatchmentMaps maps;
        maps.begin_col = ctchmnt.second->begin_col;
        maps.begin_row = ctchmnt.second->begin_row;
        maps.number_rows = ctchmnt.second->number_rows;
        maps.number_cols = ctchmnt.second->number_cols;
        
        MitigationPortfolio_SPtr folio = ctchmnt.second->get_scenario(_scenario)->get_portfolio(_portfolio[i]);
		++i;


        
        
        typedef std::pair<const std::string, ReturnInterval_SPtr> IntervalPair;
        BOOST_FOREACH(IntervalPair & interval, folio->rtrn_intrvls_sk)
        {

            int return_int;
            std::map<int, Path_SPtr> timeslice_maps;
            parse_return_interval(interval.first.begin(), interval.first.end(), return_int);
            
            
            typedef std::pair<const std::string, Path_SPtr> PathPair;
            BOOST_FOREACH(PathPair & path, interval.second->time_slices_sk)
            {
                int year = std::stoi(path.first);
                timeslice_maps.insert(std::make_pair(year, path.second));
                time_slices_set.insert(year);
            }
            
            maps.map_paths.insert(std::make_pair(return_int, timeslice_maps));
            return_intervals_set.insert(return_int);
        }
        map_set.insert(std::make_pair(ctchmnt.first, maps));
        
        catchments_set.insert(ctchmnt.first);
    }
    
    // Stitch maps
    
    int col_coord = 0;
    int row_coord = 0;
    
    BOOST_FOREACH(int return_int, return_intervals_set)
    {
        BOOST_FOREACH(int time_slice, time_slices_set)
        {
            // Make the map from template
            Map_Float_SPtr output_map(new Map_Float(n_rows, n_cols, null_value));
            
            BOOST_FOREACH(int catchment, catchments_set)
            {
                //Stritch each submap into the map
                CatchmentMaps & ctchm = map_set[catchment];
                Path_SPtr map_path = ctchm.map_paths[return_int][time_slice];
                std::string wKTprojection;
                GeoTransform transform;
                Map_Float_SPtr map;
                std::tie(map, wKTprojection, transform) = read_in_map<float>(*map_path, GDT_Float32, NO_CATEGORISATION);
                
                for (unsigned int i = 0; i < map->NRows(); ++i)
                {
                    for (unsigned int j = 0; j < map->NCols(); ++j)
                    {
                        if (map->Get(i, j) != map->NoDataValue())
                        {
                            col_coord = j + ctchm.begin_col;
                            row_coord = i + ctchm.begin_row;
                            
                            if (col_coord < n_cols && row_coord < n_rows)
                            output_map->Get(row_coord, col_coord) = map->Get(i, j);
                        }
                    }
                }
                
                
            }
            
            set.insert(std::make_pair(std::make_pair(return_int, time_slice), output_map));
        }
    }
}

void
getMapSet(std::string _scenario, std::vector<std::string> _portfolio, Database & _data, StitchedMapSet & set, unsigned int n_cols, unsigned int n_rows, float null_value)
{

	std::set<int> time_slices_set;
	std::set<int> return_intervals_set;
	std::set<int> catchments_set;

	// Get Maps
	std::map<int, CatchmentMaps> map_set;
	typedef std::pair<const int, Catchment_SPtr> CatchmentPair;
	int i = 0;
	BOOST_FOREACH(CatchmentPair & ctchmnt, _data.catchments_ik)
	{
		CatchmentMaps maps;
		maps.begin_col = ctchmnt.second->begin_col;
		maps.begin_row = ctchmnt.second->begin_row;
		maps.number_rows = ctchmnt.second->number_rows;
		maps.number_cols = ctchmnt.second->number_cols;

		MitigationPortfolio_SPtr folio = ctchmnt.second->get_scenario(_scenario)->get_portfolio(_portfolio[i]);
		++i;



		typedef std::pair<const std::string, ReturnInterval_SPtr> IntervalPair;
		BOOST_FOREACH(IntervalPair & interval, folio->rtrn_intrvls_sk)
		{

			int return_int;
			std::map<int, Path_SPtr> timeslice_maps;
			parse_return_interval(interval.first.begin(), interval.first.end(), return_int);


			typedef std::pair<const std::string, Path_SPtr> PathPair;
			BOOST_FOREACH(PathPair & path, interval.second->time_slices_sk)
			{
				int year = std::stoi(path.first);
				timeslice_maps.insert(std::make_pair(year, path.second));
				time_slices_set.insert(year);
			}

			maps.map_paths.insert(std::make_pair(return_int, timeslice_maps));
			return_intervals_set.insert(return_int);
		}
		map_set.insert(std::make_pair(ctchmnt.first, maps));

		catchments_set.insert(ctchmnt.first);
	}

	// Stitch maps

	int col_coord = 0;
	int row_coord = 0;

	BOOST_FOREACH(int return_int, return_intervals_set)
	{
		BOOST_FOREACH(int time_slice, time_slices_set)
		{
			// Make the map from template
			Map_Float_SPtr output_map(new Map_Float(n_rows, n_cols, null_value));

			BOOST_FOREACH(int catchment, catchments_set)
			{
				//Stritch each submap into the map
				CatchmentMaps & ctchm = map_set[catchment];
				Path_SPtr map_path = ctchm.map_paths[return_int][time_slice];
				std::string wKTprojection;
				GeoTransform transform;
				Map_Float_SPtr map;
				std::tie(map, wKTprojection, transform) = read_in_map<float>(*map_path, GDT_Float32, NO_CATEGORISATION);

				for (unsigned int i = 0; i < map->NRows(); ++i)
				{
					for (unsigned int j = 0; j < map->NCols(); ++j)
					{
						if (map->Get(i, j) != map->NoDataValue())
						{
							col_coord = j + ctchm.begin_col;
							row_coord = i + ctchm.begin_row;

							if (col_coord < n_cols && row_coord < n_rows)
								output_map->Get(row_coord, col_coord) = map->Get(i, j);
						}
					}
				}


			}

			set.insert(std::make_pair(std::make_pair(return_int, time_slice), output_map));
		}
	}
}


#include <vector>
#include <cfloat>
#include <math.h>

//vector< float > interp1( vector< float > &x, vector< float > &y, vector< float > &x_new )
//{
//    vector< float > y_new;
//    y_new.reserve( x_new.size() );
//    
//    std::vector< float > dx, dy, slope, intercept;
//    dx.reserve( x.size() );
//    dy.reserve( x.size() );
//    slope.reserve( x.size() );
//    intercept.reserve( x.size() );
//    for( int i = 0; i < x.size(); ++i ){
//        if( i < x.size()-1 )
//        {
//            dx.push_back( x[i+1] - x[i] );
//            dy.push_back( y[i+1] - y[i] );
//            slope.push_back( dy[i] / dx[i] );
//            intercept.push_back( y[i] - x[i] * slope[i] );
//        }
//        else
//        {
//            dx.push_back( dx[i-1] );
//            dy.push_back( dy[i-1] );
//            slope.push_back( slope[i-1] );
//            intercept.push_back( intercept[i-1] );
//        }
//    }
//    
//    for ( int i = 0; i < x_new.size(); ++i )
//    {
//        int idx = findNearestNeighbourIndex( x_new[i], x );
//        y_new.push_back( slope[idx] * x_new[i] + intercept[idx] );
//        
//    }
//    
//}

std::tuple<int, float, int, float> findNearestNeighbourIndex( int value, std::map<int, Map_Float_SPtr> &x )
{
    float dist1 = std::numeric_limits<float>::max() - 1;
    float dist2 = std::numeric_limits<float>::max();
    int idx1 = -1;
    int idx2 = -1;
    
    typedef std::pair<const int, Map_Float_SPtr> MapPair;
    BOOST_FOREACH(MapPair map, x)
    {
        int newDist = std::abs(value - map.first);
        if ( newDist < dist1 )
        {
            dist2 = dist1;
            idx2 = idx1;
            dist1 = newDist;
            idx1 = map.first;
        }
        else if (newDist < dist2)
        {
            dist2 = newDist;
            idx2 = map.first;
        }
    }
    
    int idx_range = std::abs(idx1 - idx2);
    float weight1 = (idx_range - std::abs(value - idx1)) / (float)idx_range;
    float weight2 = (idx_range - std::abs(value - idx2)) / (float)idx_range;
    return (std::make_tuple(idx1, weight1, idx2, weight2));
}


Map_Float_SPtr interpolatedmap(int return_interval, int year, StitchedMapSet & set)
{

    std::map<int, Map_Float_SPtr> map_set;
    typedef std::pair<const std::pair<int, int>, Map_Float_SPtr> SetPair;
    BOOST_FOREACH(SetPair & map, set)
    {
        if (map.first.first == return_interval) map_set.insert(std::make_pair(map.first.second, map.second));
    }
    std::tuple<int, float, int, float> interp_params = findNearestNeighbourIndex(year, map_set);
    
    float & weight_map1 = std::get<1>(interp_params);
    Map_Float_SPtr map1 = map_set[std::get<0>(interp_params)];
    float & weight_map2 = std::get<3>(interp_params);
    Map_Float_SPtr map2 = map_set[std::get<2>(interp_params)];
    
	if (map1->NoDataValue() != 0.0)
	{
		for (unsigned int i = 0; i < map1->NRows(); ++i)
		{
			for (unsigned int j = 0; j < map1->NCols(); ++j)
			{
				if (map1->Get(i, j) == map1->NoDataValue()) map1->Get(i, j) = 0;
			}
		}
		map1->SetNoDataValue(0.0);
	}
	if (map2->NoDataValue() != 0.0)
	{
		for (unsigned int i = 0; i < map2->NRows(); ++i)
		{
			for (unsigned int j = 0; j < map2->NCols(); ++j)
			{
				if (map2->Get(i, j) == map2->NoDataValue()) map2->Get(i, j) = 0.0;
			}
		}
		map2->SetNoDataValue(0.0);
	}

    //Sanity check on Map dimensions needed here. (or should be checked somewhere / ensured)
    
    Map_Float_SPtr output_map(new Map_Float((*map1 * weight_map1) + (*map2 * weight_map2), map1->NoDataValue()));
	output_map->SetNoDataValue(map1->NoDataValue());
    return (output_map);
}

    




#endif
