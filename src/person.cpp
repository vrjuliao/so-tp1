#include "person.hpp"
#include <cstdlib>
#include <unistd.h>

Person::Person(std::string name, int round, int sleep_time, OvenMonitor *monitor){
  this->name = name;
  this->round = round;
  this->time_to_sleep = sleep_time;
  this->monitor = monitor;
  this->condition = PTHREAD_COND_INITIALIZER;
};

void *Person::run(void *args){
  while(--this->round){
    sleep(this->time_to_sleep);

    monitor->wait(this->name, &this->condition);
    monitor->cook(this->name);
        
    monitor->free_oven();
    
    // cout << "<Personagem> foi comer" << endl
    sleep(OvenMonitor::my_rand(3,6));

    //go to work
    // cout << "<Personagem> foi trabalhar" << endl
  }
}

const std::string Person::get_name(){
  return this->name;
}

pthread_cond_t *Person::get_condition(){
  return &(this->condition);
}