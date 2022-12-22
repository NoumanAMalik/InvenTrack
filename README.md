# InvenTrack - C++ SQLite Tool

This C++ SQLite Tool will offer a convenient way for users to keep track of items in stock.

This tool will serve 2 main functions:

1. Scan-In Mode: Allows the user to enter a product UPC number which will be added to the database
2. Scan-Out Mode: Allows the user to enter a product UPC number which will be removed from the database

This tool is going to be a command line too where the user would have 2 options for interacting with the tool.
In the first way, the user would enter all the information in one line.

``` bash
InvenTrack -m scanin -upc ########
```

In the second way, the user would enter information when requested by the program

```bash
>> Please select a mode (enter number or name):
>> 1) Scan-In
>> 2) Scan-Out
2
>> You have selected Scan-Out Mode
>> Now please enter the UPC number of the product you would like to Scan-Out:
#######
>> The product has been scanned out
```
