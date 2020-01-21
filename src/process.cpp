#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() const { return pid_; }

// Update and then return this process's CPU utilization
float Process::CpuUtilization() { 
  long total_active{LinuxParser::ActiveJiffies(Pid())};
  float utilization{utilization_};
  long time{UpTime()};
        
  if (time - p_time_  != 0){
    float change_active = float(total_active - p_total_active_) / sysconf(_SC_CLK_TCK);
    utilization = change_active / float(time - p_time_);
    utilization_ = utilization; 
    p_time_ = time;
    p_total_active_ = total_active;

  }
  return utilization;
}

// Return this process's CPU utilization without updating
float Process::GetCpuUtilization() const { 
  return utilization_;
}

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() const {return user_; }

// Return the age of this process (in seconds)
long Process::UpTime() const { 
    return system_->UpTime() - starttime_; }

// A function to compare pointers to Process for sorting
bool Process::Compare(const Process *a, const Process *b){
  return a->GetCpuUtilization()  < b->GetCpuUtilization(); 
}
