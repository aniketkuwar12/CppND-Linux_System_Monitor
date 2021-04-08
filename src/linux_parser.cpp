#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Another way to do : Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float memTotal{0.0};
  float memFree{0.0};
  float buffers{0.0};

  std::ifstream ifs(kProcDirectory  + kMeminfoFilename);
  std::string line;
  std::string key;
  float value{0.0};
  if(ifs.is_open())
  {
    while(getline(ifs, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::stringstream linestream(line);
      linestream >> key >> value;

      if (key == "MemTotal")
        memTotal = value;
      else if (key == "MemFree")
        memFree = value;
      else if (key == "Buffers")
        buffers = value;
    }
  }

  return 1.0 - ( memFree / (memTotal - buffers));
}

long int LinuxParser::UpTime() { 
  std::ifstream ifs(kProcDirectory + kUptimeFilename);
  long int upTime;
  if (ifs.is_open())
  {
    std::string line;
    while(getline(ifs, line))
    {
      std::stringstream lineStream(line);
      lineStream >> upTime;
      break;
    }
  }
  return upTime;
 }

long LinuxParser::Jiffies() { 
  return LinuxParser::UpTime()*sysconf(_SC_CLK_TCK);
 }

long LinuxParser::ActiveJiffies() { 
  vector<string> utilization = LinuxParser::CpuUtilization();
  long activeJiffies = 0;
  activeJiffies = stol(utilization[kUser_]) + stol(utilization[kNice_]) + stol(utilization[kSystem_]) +
                  + stol(utilization[kIRQ_]) + stol(utilization[kSoftIRQ_]) + stol(utilization[kSteal_]) + 
                  stol(utilization[kGuest_]) + stol(utilization[kGuestNice_]);
                  
  return activeJiffies;
}

long LinuxParser::IdleJiffies() { 
  vector<string> utilization = LinuxParser::CpuUtilization();
  long IdleJiffies = 0;
  IdleJiffies = stol(utilization[kIdle_]) + stol(utilization[kIOwait_]);
  return IdleJiffies;
 }

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> values;
  std::ifstream ifs(kProcDirectory + kStatFilename);

  if(ifs.is_open())
  {
    std::string line;
    getline(ifs, line);
    std::stringstream lineStream(line);
    std::string key, value;
    while(lineStream >> key)
    {
      if(key == "cpu")
      {
        while(lineStream >> value)
        {
          values.emplace_back(value);
        }
        break;
      }
    }
  }
  return values;
}

int LinuxParser::GenericProcesses(const char * fetchKey) { 
  std::ifstream ifs(kProcDirectory + kStatFilename);
  int numProcess;
  std::string line;
  std::string key;
  int value = 0;
  
  if(ifs.is_open())
  {
    std::string line;
    while(getline(ifs, line))
    {
      std::stringstream lineStream(line);
      lineStream >> key >> value;
      if ( key.compare(fetchKey) == 0)
      {
        numProcess = value;
        break;
      }
    }
  }
  return numProcess;
}

int LinuxParser::TotalProcesses()
{
  return GenericProcesses("processes");
}

int LinuxParser::RunningProcesses() 
{ 
  return GenericProcesses("procs_running");
}

string LinuxParser::Command(int pid) 
{ 
  string cmd;
  std::ifstream ifs(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if(ifs.is_open())
  {
    string lineStream;
    while(getline(ifs, lineStream))
    {
      cmd += lineStream;
    }
  }
  return cmd;
  
}

float LinuxParser::processCpu(int pid)
{
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatFilename);
  float procCpu{0.0};

  if(ifs.is_open())
  {
    string line;
    while(getline(ifs, line))
    {
      std::stringstream lineStream(line);
      string value;
      int count = 0;
      float utime, stime, cutime, cstime, starttime;
      while(count < 22)
      {
        lineStream >> value;
        if(count == 13)
          utime = stol(value);
        if(count == 14)
          stime = stol(value);
        if(count == 15)
          cutime = stol(value);
        if(count == 16)
          cstime = stol(value);
        count++;
      }

      starttime = stol(value);
      float totaltime = utime + stime + cutime + cstime;
      float seconds = UpTime() - starttime/sysconf(_SC_CLK_TCK);
      procCpu = (totaltime / sysconf(_SC_CLK_TCK)) / seconds;

    }
  }
  return procCpu;
}


string LinuxParser::Ram(int pid) { 
  string mem{"0.0"};
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatusFilename);
  if(ifs.is_open())
  {
    string line;
    while(getline(ifs, line))
    {
      std::stringstream lineStream(line);
      string key;
      lineStream >> key;
      
      if(key == "VmSize:")
      {
        long value;
        lineStream >> value;
        mem = to_string(value/1000);
        return mem;
      }
    }
  }
  return mem;
}

string LinuxParser::User(int pid) { 
  string userid = LinuxParser::Uid(pid);
  std::ifstream ifs(kPasswordPath);
  string username; 
  if(ifs.is_open())
  {
    string line;
    while(getline(ifs, line))
    {
      std::stringstream lineStream(line);
      string token;
      int count = 0;
      while(getline(lineStream, token, ':'))
      {
        if(count == 0)
          username = token;
        
        if(count == 2 && token == userid)
          return username;

        count++;
      }
      username.clear();
    }
  }
  return username;
}

string LinuxParser::Uid(int pid) {
  string userid;
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatusFilename);
  if(ifs.is_open())
  {
    string line;
    while(getline(ifs, line))
    {
      std::stringstream lineStream(line);
      string key;
      lineStream >> key;
      
      if(key == "Uid:")
      {
        lineStream >> userid;
        return userid;
      }
    }
  }
  return userid;
}

long int LinuxParser::UpTime(int pid) { 
  
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatFilename);
  long int procUptime = 0;

  if(ifs.is_open())
  {
    string line;
    while(getline(ifs, line))
    {
      std::stringstream lineStream(line);
      string value;
      int count = 0;
      long int pUpTime;
      while(count < 22)
      {
        lineStream >> value;
        count++;
      }

      pUpTime = stol(value);
      procUptime = UpTime() - pUpTime/sysconf(_SC_CLK_TCK);  

    }
  }
  return procUptime; 
}