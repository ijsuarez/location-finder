# location-finder
Converts location data encoded in CSV to binary and reads from memory

```
CSV has the following format:
ID,Continent,Country,City
1234134,Asia,Korea,Busan
342432,Europe,Paris,France
431435,North America,United States,Saint Louis
```

The program encodes the CSV into a binary file that has the following structure:

* An int to store the number of location headers in the file
* The location headers (in sorted order based off the ID value)
* The dataload (in the order that the data is read aka unsorted)

A location header stores the location's ID value, a pointer to the data within the dataload, and the number of characters for each attribute (size of continent, country, and city strings). A binary search is performed on the headers, and once a matching header is found it accesses the data from the stored pointer.

* Encoding time complexity is O(nlgn) from the header sorting, where n is the number of lines in the file
* Decoding time complexity is O(lgn) from the lookup using binary search, where n is the number of lines in the file
* Space complexity for both encoding and decoding is O(n), where n is the number of lines in the file

```
Inputs/outputs:

Input: “./location_finder.bin encode path_to_file_to_encode.csv”
Output: “Success”

Input: “./location_finder.bin decode”
Output: “Success, waiting for further input”

Input: “342432”
Output: “Europe, Paris, France”

Input: “1234134 country city continent”
Output: “Korea,Busan,Asia”

Input: “431435 city”
Output: “Saint Louis”

Input: “431435 city country”
Output: “Saint Louis,United States”

Input: “431435 city city”
Output: “Invalid Input”

Input: “exit”
Output: None, but the program should terminate
```

### TO-DO
* Remove commas from specific output strings
* Refine main method to be more robust
