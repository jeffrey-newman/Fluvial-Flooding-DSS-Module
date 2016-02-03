//
//  DirectoryNameParser.h
//  DSSFloodModule
//
//  Created by a1091793 on 10/09/2015.
//  Copyright (c) 2015 University of Adelaide. All rights reserved.
//

#ifndef DSSFloodModule_DirectoryNameParser_h
#define DSSFloodModule_DirectoryNameParser_h

#include <string>
#include <boost/filesystem.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_no_case.hpp>

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

//using boost::spirit::qi::double_;
//using boost::spirit::qi::_1;
//using boost::spirit::qi::phrase_parse;
//using boost::spirit::ascii::space;
//using boost::phoenix::ref;

namespace fs = boost::filesystem;

bool parse_dir_name2(fs::path directory, std::pair<int, std::string> & return_val)
{
	fs::path name_file = directory / "name.txt";
	std::ifstream nfs(name_file.c_str());
	if (!nfs.is_open())
	{
		return (false);
	}
	std::getline(nfs, return_val.second);
	
	fs::path index_file = directory / "index.txt";
	std::ifstream ifs(index_file.c_str());
	if (!ifs.is_open())
	{
		return (false);
	}
	ifs >> return_val.first;
	return (true);
}

template <typename Iterator>
bool parse_dir_name(Iterator first, Iterator last, std::string const & dir_prefix, std::pair<int, std::string> & return_val)
{

    qi::rule<Iterator, std::string(), qi::space_type> name_parse = qi::lexeme[ +qi::char_];

    bool r = qi::phrase_parse(first, last,
                          
                          //  Begin grammar
                          (
                           qi::lit(dir_prefix) >> qi::int_[phoenix::ref(return_val.first) = qi::_1]
                           >> '-'
                           >> name_parse[phoenix::ref(return_val.second) = qi::_1]
                           ),
                          //  End grammar
                           qi::space);
    if (!r || first != last) // fail if we did not get a full match
        return (false);
    return (r);
}

template <typename Iterator>
bool parse_return_interval(Iterator first, Iterator last, int & return_val)
{
    
    qi::rule<Iterator, std::string(), qi::space_type> name_parse = qi::lexeme[ +qi::char_];
    
    bool r = qi::phrase_parse(first, last,
                              
                              //  Begin grammar
                              (
							  qi::no_case[qi::lit("1 In")]
                               >> qi::int_[phoenix::ref(return_val) = qi::_1]
							   >> qi::no_case[qi::lit("Years")]
                               ),
                              //  End grammar
                              qi::space);
    if (!r || first != last) // fail if we did not get a full match
        return (false);
    return (r);
}


#endif
