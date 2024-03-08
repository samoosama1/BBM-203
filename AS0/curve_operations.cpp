#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "util_functions.h"

/*
Calculates simple statistics of a class, using vector of average scores, writing information
to the output file with a title.
(part 1 and part 2 outputs.)
*/
void analyzeAveragesToFile(std::vector<double>& averages, const std::string& fileName, const std::string& title){
    sort(averages.begin(), averages.end());
    double avgGrade{};
    double minGrade = averages[0];
    double maxGrade = averages[averages.size() - 1];
    double sum{};
    std::ofstream ofs {"part1_output_test.txt"};
    std::vector<std::string> letterGrades;
    for (double grade : averages) {
        sum += grade;
        letterGrades.push_back(calculateGrade(grade));
    }
    avgGrade = sum / averages.size();
    std::map<std::string, int> mp = countLetters(letterGrades);
    auto it = mp.begin();
    ofs << "- - - - - - - - - - -" << "\n";
    ofs << "BBM Statistics (Part 1)" << "\n";
    ofs << "- - - - - - - - - - -" << "\n";
    while (it != mp.end()) {
        ofs << it->first << ":\t" << it->second << "\n";
        ++it;
    }
    ofs << "- - - - - - - - - - -" << "\n";
    ofs << "Total Number of Students: " << letterGrades.size() << "\n";;
    ofs << "Mean value of average grades: " << avgGrade << "\n";;
    ofs << "Max Score: " << maxGrade << "\n";;
    ofs << "Min Score " << minGrade << "\n";;
}

///*
//creates simple statistics output for the part 3.
//*/
//void analize_part_3(std::vector<student>& students){
//
//}
//
//
///*
//applies curve type 1 (increasing average score to 50) to the grades and
//returns a new vector of curved scores. does not change the original grades.
//*/
//std::vector<double> curve_type_1(std::vector<double> averages){
//
//}
//
//
///*
//Applies curve type 2 (increasing maximum score to 100) to the grades and 
//returns a new vector of curved scores. Does not change the original grades.
//*/
//std::vector<double> curve_type_2(std::vector<double> averages){
//
//}
//
//
///*
//Applies curve type 3 (fail exactly n student) to the grades and 
//returns a new vector of curved scores. Does not change the original grades.
//*/
//std::vector<double> curve_type_3(std::vector<double> averages, int will_fail){
//
//}
