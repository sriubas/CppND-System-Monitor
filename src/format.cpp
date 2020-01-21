#include <string>

#include "format.h"

using std::string;

// Return time the format HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string time; 
  if (seconds/3600 >= 10) { 
    time += std::to_string(seconds/3600) + ":";
  } else {
    time += "0" +  std::to_string(seconds/3600) + ":";
  }
  seconds %= 3600;
  
  if (seconds/60 >= 10) {
    time += std::to_string(seconds/60) + ":";
  } else {
    time += "0" + std::to_string(seconds/60) + ":";
  }
  seconds %= 60;
  
  if (seconds >= 10) {
    time += std::to_string(seconds);
  } else {
    time += "0" + std::to_string(seconds);
  }
  return time; 
}