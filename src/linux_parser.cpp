#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Return Operating System name
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

// Return kernel name
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

// Return a container of process ids
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memtotal, memfree, output{0.0};
  string line, name;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> name >> memtotal;

    std::getline(stream, line);
    linestream.clear();
    linestream.str(line);
    linestream >> name >> memfree;

    output = (memtotal - memfree) / memtotal;
  }
  return output;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long val{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> val;
  }
  return val; 
}

// Read and return the total number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line, name;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0}, irq{0}, softirq{0}, steal{0};
  long total;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);  
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
    total = user + nice + system + irq + softirq + steal + iowait + idle;

  return total; 
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long utime{0}, stime{0}, cutime{0}, cstime{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; i++){
      linestream >> value;
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  return utime + stime + cutime + cstime;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, name;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0}, irq{0}, softirq{0}, steal{0};
  long total_active;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);  
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  total_active = user + nice + system + irq + softirq + steal;

  return total_active; 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, name;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  long total_idle;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    total_idle = idle + iowait;
  }
  return total_idle;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, name;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "processes")
        linestream >> value;
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, name;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "procs_running")
        linestream >> value;
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line, cmd{};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
  }
  return line; }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, name, value;
  long n{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> name >> value;
      if (name == "VmSize:"){
        n = std::stoi(value) / 1000;
      }
    }
  }
  return std::to_string(n); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, name, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" + kStatusFilename); 
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> name >> value;
      if (name == "Uid:"){
        return value;
      }
    }
  }
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {  
  string uid{LinuxParser::Uid(pid)};
  string line, name, x, value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> x >> value;
      if (value == uid)
        return name;
    }
  }
  return string();
}

// Read and return the uptime of a process
long LinuxParser::StartTime(int pid) {
  long output{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" + kStatFilename);
  string line, value;
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      for (int i = 0; i < 22; i++){
        linestream >> value;
      }
      output = std::stol(value)/sysconf(_SC_CLK_TCK);
    }
  }
  return output;
}