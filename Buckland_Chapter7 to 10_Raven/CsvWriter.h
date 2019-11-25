#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <iterator>
#include <string>
#include <algorithm>

/*
* A class to create and write data in a csv file.
*/
class CsvWriter
{
	std::string fileName;
	std::string delimeter;
	int linesCount;

public:
	CsvWriter(std::string filename, std::string delm = ",") :
		fileName(filename), delimeter(delm), linesCount(1)
	{}
	/*
	* Member function to store a range as comma seperated value
	*/
	template<typename T>
	void addDatainRow(T first, T last);
};

template<typename T>
inline void CsvWriter::addDatainRow(T first, T last)
{
	std::fstream file;
	// Open the file in truncate mode if first line else in Append Mode
	file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));

	// Iterate over the range and add each element to file seperated by delimeter.
	for (; first != last; )
	{
		file << *first;
		if (++first != last)
			file << delimeter;
	}
	file << "\n";
	linesCount++;

	// Close the file
	file.close();
}

#endif


