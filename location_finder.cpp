#include "location_finder.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <iterator>

#include <unistd.h>
#include <fcntl.h>

using namespace std;

const char* DATAFILE = "temp/locations.dat";

LocationFinder::LocationFinder()
{

}

int LocationFinder::encode(char* csvFile)
{
    stringstream ss;
    stringstream dataload;
    string line;

    vector<LocationHeader> headerList;
    size_t numHeaders = 0;
    string id, continent, country, city;

    ifstream myfile(csvFile);
    while(getline(myfile, line))
    {
        ss << line;

        getline(ss, id, ',');
        getline(ss, continent, ',');
        getline(ss, country, ',');
        getline(ss, city, ',');

        LocationHeader header;
        header.id = atoi(id.c_str());
        header.dataPtr = dataload.tellp();
        header.contLoad = continent.size();
        header.countryLoad = country.size();
        header.cityLoad = city.size();

        ++numHeaders;
        headerList.push_back(header);
        dataload << continent << country << city;

        ss.clear();
    }
    myfile.close();

    sort(headerList.begin(), headerList.end(), 
        [](const LocationHeader &l1, const LocationHeader &l2) -> bool
        { return l1.id < l2.id; });

    ofstream outfile(DATAFILE);
    if(!outfile.is_open())
    {
        cout << "Failed to open file" << endl;
        return 1;
    }

    outfile.write(reinterpret_cast<char*>(&numHeaders), sizeof(size_t));
    for(size_t i = 0; i < headerList.size(); ++i)
    {
        outfile.write(reinterpret_cast<char*>(&headerList[i]), sizeof(LocationHeader));
    }
    const string &tmp = dataload.str();
    const char* c_dataload = tmp.c_str();
    outfile.write(c_dataload, strlen(c_dataload));
    outfile.close();

    cout << "Success" << endl;
    return 0;
}

int LocationFinder::decode()
{
    struct stat sb;

    int fd;
    size_t numHeaders;
    char* map;
    char* data;
    char* locationPtr;

    string input;
    vector<string> inputTokens;
    size_t contCount = 0;
    size_t countryCount = 0;
    size_t cityCount = 0;

    fd = open(DATAFILE, O_RDONLY);
    if (fd == -1)
    {
        cout << "Failed to open file" << endl;
        return 1;
    }
    fstat(fd, &sb);

    map = (char*) mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        cout << "Error mmapping the file" << endl;
        return 1;
    }

    numHeaders = (size_t)map[0];
    data = map + sizeof(size_t) + (numHeaders * sizeof(LocationHeader));

    cout << "Success, waiting for further input" << endl;
    getline(cin, input);
    while(input != "exit")
    {
        istringstream iss(input);
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(inputTokens));

        try
        {
            LocationHeader* target = binarySearch(map+sizeof(size_t), 0, numHeaders-1, stoi(inputTokens[0]));

            if(!target)
            {
                cout << "ID not found" << endl;
                inputTokens.clear();
                getline(cin, input);
                continue;
            }

            char* locationPtr = data + target->dataPtr;

            if(inputTokens.size() == 1)
            {
                printf("%.*s, %.*s, %.*s",
                    target->contLoad, locationPtr,
                    target->countryLoad, locationPtr+target->contLoad,
                    target->cityLoad, locationPtr+target->contLoad+target->countryLoad);
            }
            else
            {
                for(size_t i = 1; i < inputTokens.size(); ++i)
                {
                    if(inputTokens[i] == "continent")
                    {
                        ++contCount;
                    }
                    else if(inputTokens[i] == "country")
                    {
                        ++countryCount;
                    }
                    else if(inputTokens[i] == "city")
                    {
                        ++cityCount;
                    }
                    else
                    {
                        throw 1;
                    }
                    if(contCount > 1 || countryCount > 1 || cityCount > 1)
                    {
                        throw 1;
                    }
                }
                for(size_t i = 1; i < inputTokens.size(); ++i)
                {
                    if(inputTokens[i] == "continent")
                    {
                        printf("%.*s, ", target->contLoad, locationPtr);
                    }
                    else if(inputTokens[i] == "country")
                    {
                        printf("%.*s, ", target->countryLoad, locationPtr+target->contLoad);
                    }
                    else if(inputTokens[i] == "city")
                    {
                        printf("%.*s, ", target->cityLoad, locationPtr+target->contLoad+target->countryLoad);
                    }
                }
            }
            cout << endl;

            inputTokens.clear();
            contCount = 0;
            countryCount = 0;
            cityCount = 0;
            getline(cin, input);
        }
        catch (...)
        {
            cout << "Invalid Input" << endl;
            inputTokens.clear();
            contCount = 0;
            countryCount = 0;
            cityCount = 0;
            getline(cin, input);
        }
    }

    return 0;
}

LocationHeader* LocationFinder::binarySearch(char* map, size_t begin, size_t end, int target)
{
    LocationHeader* center = reinterpret_cast<LocationHeader *>(map+(((end+begin)/2)*sizeof(LocationHeader)));

    if(target == center->id)
    {
        return center;
    }
    if(begin == end)
    {
        return nullptr;
    }
    if(target < center->id)
    {
        return binarySearch(map, begin, ((end+begin)/2)-1, target);
    }
    if(target > center->id)
    {
        return binarySearch(map, ((end+begin)/2)+1, end, target);
    }
}

int main(int argc, char* argv[])
{
    LocationFinder lf;
    if(argc == 2 && strcmp(argv[1], "decode") == 0)
    {
        lf.decode();
    }
    else if(argc == 3 && strcmp(argv[1], "encode") == 0)
    {
        lf.encode(argv[2]);
    }
    else
    {
        return 1;
    }
    return 0;
}
