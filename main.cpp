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

using namespace std;

int main (int argc, const char *argv[]){
  if(argc != 2)
    std::cout <<  "The only execution argument is the number "
      << "of rounds" << std::endl;

  int rounds = atoi(argv[1]);
  OvenMonitor monitor = OvenMonitor(rounds);

  monitor.start();

  monitor.end();
  return 0;
}
