#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public: 
  float Utilization();

 private:
  long p_total_{0};
  long p_total_active_{0};
  float utilization_{0.0};
};

#endif