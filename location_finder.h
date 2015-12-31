#ifndef LOCATION_FINDER
#define LOCATION_FINDER

#include <string>

struct LocationHeader
{
    int id;
    int dataPtr;
    int contLoad;
    int countryLoad;
    int cityLoad;
};

class LocationFinder
{
public:
    LocationFinder();
    int encode(char* csvFile);
    int decode();

    LocationHeader* binarySearch(char* map, size_t begin, size_t end, int target);
};

#endif
