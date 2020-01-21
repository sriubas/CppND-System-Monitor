#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "system.h"
#include "linux_parser.h"

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(System* system, int pid) : system_(system),
                                     pid_(pid),
                                     user_(LinuxParser::User(pid)),
                                     starttime_(LinuxParser::StartTime(pid)),
                                     command_(LinuxParser::Command(pid)),
                                     p_time_{system->UpTime() - starttime_},
                                     p_total_active_{LinuxParser::ActiveJiffies(pid)} {};
  int Pid() const;
  float GetCpuUtilization() const;
  std::string Ram() const;
  std::string User() const;
  std::string Command() const;
  long int UpTime() const;
  static bool Compare(const Process *a, const Process *b);
  
  float CpuUtilization();

 private:
  System* const system_;
  int const pid_;
  string const user_;
  long const starttime_;
  string const  command_;
  
  long p_time_{0};
  long p_total_active_{0};
  float utilization_{0.0};
};

#endif