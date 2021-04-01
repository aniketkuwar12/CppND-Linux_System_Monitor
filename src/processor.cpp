#include <unistd.h>
#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() { 
    
    return (float)LinuxParser::ActiveJiffies() / sysconf(_SC_CLK_TCK) / LinuxParser::UpTime();
}