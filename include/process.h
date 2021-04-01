#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pId, 
          std::string uId, 
          std::string cmd, 
          float cpu, 
          std::string memUsage,
          long int uptime);

  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;

 private:
  int pid;
  std::string userid;
  std::string command;
  float cpuUsage;
  std::string ram;
  long int upTime;
};

#endif