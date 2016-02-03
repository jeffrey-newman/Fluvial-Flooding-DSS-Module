//
//  Types.h
//  DSSFloodModule
//
//  Created by a1091793 on 10/09/2015.
//  Copyright (c) 2015 University of Adelaide. All rights reserved.
//

#ifndef DSSFloodModule_Types_h
#define DSSFloodModule_Types_h

#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include "MapTypes.h"

namespace file_sys = boost::filesystem;


typedef boost::shared_ptr<file_sys::path> Path_SPtr;
typedef std::map<int, Path_SPtr> TimeSliceMap_IntKey;
typedef std::map<std::string, Path_SPtr> TimeSliceMap_StringKey;

struct ReturnInterval
{
public:
    TimeSliceMap_IntKey time_slices_ik;
    TimeSliceMap_StringKey time_slices_sk;
    file_sys::path const & time_slice(int _id)
    {
        return *(time_slices_ik[_id]);
    }
    file_sys::path const & time_slice(std::string _id)
    {
        return *(time_slices_sk[_id]);
    }
};

typedef boost::shared_ptr<ReturnInterval> ReturnInterval_SPtr;


typedef std::map<int, ReturnInterval_SPtr> ReturnIntrvlMap_IntKey;
typedef std::map<std::string, ReturnInterval_SPtr> ReturnIntrvlMap_StringKey;

struct MitigationPortfolio
{
public:
    ReturnIntrvlMap_IntKey rtrn_intrvls_ik;
    ReturnIntrvlMap_StringKey rtrn_intrvls_sk;
    ReturnInterval_SPtr get_interval(int const _id)
    {
        return (rtrn_intrvls_ik[_id]);
    }
    ReturnInterval_SPtr get_interval(std::string const _id)
    {
        return (rtrn_intrvls_sk[_id]);
    }
    
};

typedef boost::shared_ptr<MitigationPortfolio> MitigationPortfolio_SPtr;



typedef std::map<int, MitigationPortfolio_SPtr> MitPrtfliolMap_IntKey;
typedef std::map<std::string, MitigationPortfolio_SPtr> MitPrtflioMap_StringKey;

struct ClimateScenario
{
public:
    MitPrtfliolMap_IntKey mit_prtflios_ik;
    MitPrtflioMap_StringKey mit_prtflios_sk;
    MitigationPortfolio_SPtr get_portfolio(int const _id)
    {
        return (mit_prtflios_ik[_id]);
    }
    MitigationPortfolio_SPtr get_portfolio(std::string const & _id)
    {
        return (mit_prtflios_sk[_id]);
    }
};

typedef boost::shared_ptr<ClimateScenario> ClimateScenario_SPtr;



typedef std::map<int, ClimateScenario_SPtr> ClimScnrioMap_IntKey;
typedef std::map<std::string, ClimateScenario_SPtr> ClimScnrioMap_StringKey;


struct Catchment
{
public:
    ClimScnrioMap_IntKey clim_scnrios_ik;
    ClimScnrioMap_StringKey clim_scnrios_sk;
    int begin_col;
    int begin_row;
    int number_rows;
    int number_cols;
    ClimateScenario_SPtr get_scenario(int const _id)
    {
        return (clim_scnrios_ik[_id]);
    }
    ClimateScenario_SPtr get_scenario(std::string const & _id)
    {
        return (clim_scnrios_sk[_id]);
    }
};



typedef boost::shared_ptr<Catchment> Catchment_SPtr;
typedef std::map<int, Catchment_SPtr> CatchmentMap_IntKey;
typedef std::map<std::string, Catchment_SPtr> CatchmentMap_StringKey;


struct Database
{
    CatchmentMap_IntKey catchments_ik;
    CatchmentMap_StringKey catchments_sk;
    Catchment_SPtr get_catchment(int const _id)
    {
        return (catchments_ik[_id]);
    }
    Catchment_SPtr get_catchment(std::string const & _id)
    {
        return (catchments_sk[_id]);
    }
};

ReturnInterval_SPtr getMaps(int _catchment, int _scenario, int _portfolio, int _interval, Database & _data)
{
    return _data.get_catchment(_catchment)->get_scenario(_scenario)->get_portfolio(_portfolio)->get_interval(_interval);
}



typedef std::map<std::pair<int, int>, Map_Float_SPtr> StitchedMapSet;

#endif
