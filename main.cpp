#include <iostream>
#include <pthread.h>
#include <string>
#include <vector> 
#include <map>
#include <list>
#include <cstdlib>
#include <unistd.h>
#include <chrono>
#include <algorithm>

#include "oven_monitor.hpp"

pthread_mutex_t oven_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;

int main (int argc, const char *argv[]){
  if(argc != 2)
    std::cout <<  "The only execution argument is the number "
      << "of rounds" << std::endl;

  int counter = atoi(argv[1]);
  pthread_mutex_t oven_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;
  OvenMonitor monitor = OvenMonitor(counter, &oven_lock, &data_lock);

  monitor.start();

  monitor.end();
  return 0;
}
