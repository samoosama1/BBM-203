#include <iostream>
#include <vector>
#include "util_functions.h"
#include "curve_operations.h"

void part1(); 

int main() {
    part1();
}

void part1() {
    std::vector<std::string> lines = readLines("part1_input.txt");
    std::vector<double> avgGrades;
    std::vector<std::string> studentGrades;
    int midtermScore{};
    int finalScore{};
    double grade{};
    for(std::string element : lines) {
        studentGrades = splitString(element, "\t");
        midtermScore = std::stoi(studentGrades[0]);
        finalScore = std::stoi(studentGrades[1]);
        grade = 0.6 * finalScore + 0.4 * midtermScore;
        avgGrades.push_back(grade);
    }
    analyzeAveragesToFile(avgGrades, "b", "a");
}
