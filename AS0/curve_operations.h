#ifndef CLASS_STATISTICS_H
#define CLASS_STATISTICS_H

#include <vector>
#include <string>
//#include "Student.h" // Include the header file for the Student class

// Function to calculate simple statistics of a class and write information to the output file
void analyzeAveragesToFile(std::vector<double>& averages, const std::string& fileName, const std::string& title);

// Function to create simple statistics output for part 3
//void analyzePart3(std::vector<Student>& students);

// Function to apply curve type 1 (increasing average score to 50) to the grades
std::vector<double> curveType1(std::vector<double> averages);

// Function to apply curve type 2 (increasing maximum score to 100) to the grades
std::vector<double> curveType2(std::vector<double> averages);

// Function to apply curve type 3 (fail exactly n students) to the grades
std::vector<double> curveType3(std::vector<double> averages, int willFail);

#endif // CLASS_STATISTICS_H
