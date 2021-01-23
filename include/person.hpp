#ifndef TP1_SO_PERSON
#define TP1_SO_PERSON

#include <string>
#include <pthread.h>
#include "oven_monitor.hpp"

class Person {
private:
  std::string name;
  int round;
  int time_to_sleep;
  pthread_cond_t condition;
  OvenMonitor *monitor;

public:
  Person(std::string name, int round, int sleep_tim, OvenMonitor *monitor);
  void* run(void *);
  const std::string get_name();
  pthread_cond_t *get_condition();
};
#endif