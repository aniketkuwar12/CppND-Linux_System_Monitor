#include <string>
#include<iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    std::stringstream sStream;
    int hh = seconds / 3600;
    int mm = seconds % 3600;
    int ss = mm % 60;
    mm = mm / 60;
    sStream << std::setw(2) << std::setfill('0') << std::to_string(hh);
    sStream << ":";
    sStream << std::setw(2) << std::setfill('0') << std::to_string(mm);
    sStream << ":";
    sStream << std::setw(2) << std::setfill('0') << std::to_string(ss);

    return sStream.str();
}