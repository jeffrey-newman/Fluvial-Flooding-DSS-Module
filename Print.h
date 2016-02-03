
#ifndef PRINT
#define PRINT

#include <iostream>
#include "MapTypes.h"

template <typename DataFormat>
void
print(boost::shared_ptr<Map_Matrix<DataFormat> > map, std::ostream & str);


#endif //PRINT