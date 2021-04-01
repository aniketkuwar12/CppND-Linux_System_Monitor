#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pId, 
          string uId,
          string cmd, 
          float cpu, 
          string memUsage,
          long int uptime) : 
          pid(pId),
          userid(uId),
          command(cmd),
          cpuUsage(cpu),
          ram(memUsage),
          upTime(uptime)
{

}

int Process::Pid() { return pid; }

float Process::CpuUtilization() { return cpuUsage; }

string Process::Command() { return command; }

string Process::Ram() { return ram; }

string Process::User() { return userid; }

long int Process::UpTime() { return upTime; }

bool Process::operator<(Process const& that) const
{
    return this->cpuUsage < that.cpuUsage;
}