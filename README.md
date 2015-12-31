# location-finder
Converts location data encoded in CSV to binary and reads from memory

```
CSV has the following format:
ID,Continent,Country,City
1234134,Asia,Korea,Busan
342432,Europe,Paris,France
431435,North America,United States,Saint Louis
```

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

Currently there's a minor bug where specified inputs ("country city continent" etc) have a comma at the end. May or may not ever be fixed.
