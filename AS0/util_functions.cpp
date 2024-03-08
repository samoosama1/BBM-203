#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

std::vector<std::string> letters = {"A1", "A2", "A3", "B1", "B2", "B3", "C1", "C2", "C3", "D", "F1", "F2", "F3"};


/**
 * Reads the contents of a file and stores each line as a string in a vector.
 *
 * @param fileName The name of the file to read.
 * @return A vector of strings containing the lines from the file.
 * @note If the file cannot be opened, the program exits with a status code of -1.
 */
std::vector<std::string> readLines(const std::string& fileName) {
    std::ifstream fs;
    std::string line;
    std::vector<std::string> v;
    fs.open(fileName);

    if(!fs.is_open()) {
        std::cerr << "Error opening the file: " << fileName << std::endl;
    }
    while(std::getline(fs, line)) {
        v.push_back(line);
    }
    return v;
}


/**
 * Appends a string to a file as a new line.
 *
 * @param fileName The name of the file to append to.
 * @param content The string to be appended as a new line.
 */
void appendLine(const std::string& fileName, const std::string& content) {

    //Fill this method.

}

/**
 * Converts a space-separated string of integers into a vector of integers.
 *
 * @param input The input string containing space-separated integers.
 * @return A vector of integers.
 */
std::vector<int> stringToIntegerVector(const std::string& input) {
    std::vector<int> intVector;
    std::stringstream ss {input};
    int number = 0;
    while(ss << number) {
        intVector.push_back(number);
    }
    return intVector;
}

/**
 * This function calculates the letter grade based on the given grade point.
 *
 * @param point: Grade point (float)
 * @return: Letter grade (std::string)
 */
std::string calculateGrade(float point) {
    if (point >= 95)
        return "A1";
    else if (point >= 90)
        return "A2";
    else if (point >= 85)
        return "A3";
    else if (point >= 80)
        return "B1";
    else if (point >= 75)
        return "B2";
    else if (point >= 70)
        return "B3";
    else if (point >= 65)
        return "C1";
    else if (point >= 60)
        return "C2";
    else if (point >= 55)
        return "C3";
    else if (point >= 50)
        return "D";
    else
        return "F3";
}

/**
 * Returns the frequencies of letters grades in a vector<string> as a map.
 * Takes parameter as a vector such as {"A1", "A2", "F1", "B3"}
 *
 * @param elements: The array containing elements (std::vector<std::string>)
 * @return: A map containing the counts of elements (std::map<std::string, int>)
 */
std::map<std::string, int> countLetters(const std::vector<std::string>& elements) {
    std::map<std::string, int> mp;
    for (std::string grade : elements) {
        if(mp.find(grade) == mp.end()) {
            mp[grade] = 1;
        } else {
            mp[grade] += 1;
        }
    }
    return mp;
}

/**
 * Split a string into a vector of substrings based on a delimiter.
 *
 * @param mainStr The main string to be split.
 * @param delimiter The delimiter string used for splitting (default is " ").
 * @return A vector of strings containing the substrings.
 */
std::vector<std::string> splitString(const std::string& mainStr, const std::string& delimiter = " ") {
    std::string str;
    std::vector<std::string> strVect;
    size_t beginIndex {0};
    size_t pos = mainStr.find(delimiter, 0);
    while(pos != std::string::npos) {
        str = mainStr.substr(beginIndex, pos - beginIndex);
        strVect.push_back(str);
        beginIndex = pos + delimiter.length();
        pos = mainStr.find(delimiter, beginIndex);
    }
    strVect.push_back(mainStr.substr(beginIndex));
    return strVect;
}
