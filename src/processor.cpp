#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

using std::string;

// Update and return the CPU utilization
float Processor::Utilization() { 
  long total{LinuxParser::Jiffies()};
  long total_active{LinuxParser::ActiveJiffies()};
  float utilization{utilization_};
        
  if (total - p_total_  > 0){
    utilization = float(total_active - p_total_active_) / float(total - p_total_);
    utilization_ = utilization; 
    p_total_ = total;
    p_total_active_ = total_active;
  }
  return utilization;
}