#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

void System::setProcessData() { 
    //Get Pids from LinuxParser
    vector<int> allPids = LinuxParser::Pids();
    for(auto eachP : allPids)
    {
        Process p( eachP,
                   LinuxParser::User(eachP),
                   LinuxParser::Command(eachP),
                   LinuxParser::processCpu(eachP),
                   LinuxParser::Ram(eachP),
                   LinuxParser::UpTime(eachP) );
        
        processes_.push_back(p);
    }

    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());
}

vector<Process>& System::Processes() { 
    this->setProcessData();
    return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }