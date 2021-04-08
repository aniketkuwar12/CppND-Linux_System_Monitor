#include <string>
#include <chrono>
#include<iomanip>


#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long numSeconds) {
    std::stringstream sStream;
    
    std::chrono::seconds seconds{numSeconds};

    std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(seconds);
    seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

    std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);
    seconds -= std::chrono::duration_cast<std::chrono::minutes>(minutes);   
    
    sStream << std::setw(2) << std::setfill('0') << hours.count();
    sStream << std::setw(1) << ":";
    sStream << std::setw(2) << std::setfill('0') << minutes.count();
    sStream << std::setw(1) << ":";
    sStream << std::setw(2) << std::setfill('0') << seconds.count();
    
    return sStream.str();
}