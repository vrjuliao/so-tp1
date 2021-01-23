#ifndef TP1_SO_OVEN_MONITOR
#define TP1_SO_OVEN_MONITOR

#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <pthread.h>

#include "person.hpp"

static const std::map<std::string, std::string> SO_TP1_COUPLES = 
  {{"Sheldon", "Amy"}, {"Howard", "Bernadette"}, {"Leonard", "Penny"},
  {"Amy", "Sheldon"}, {"Bernadette", "Howard"}, {"Penny", "Leonard"},
  {"Kripke", ""},{"Stuart", ""}};

static const std::vector<std::string> SO_TP1_NAMES = {"Sheldon", "Howard", "Leonard",
  "Stuart", "Kripke", "Amy", "Penny", "Bernadette"};

class OvenMonitor {
private:
  pthread_mutex_t *mOven_lock, *mData_lock;
  std::list<Person> mWaitign_list;
  std::map<std::string, Person> mPersons;
  int mRounds;

  std::vector<std::string> get_instance();

  // thread function
  // check who allow the semaphore of next oven access
  void *monitor();

  // thread function to prevent deadlocks
  void *raj(); 

public:
  // dispara a funcao monitor() como uma thread
  // dispara a funcao raj() como uma thread
  OvenMonitor(int oven_access, pthread_mutex_t *oven_lock, pthread_mutex_t *data_lock);  

  static int my_rand(int start, int end);
  Person get_person(std::string name);
  void wait(std::string name, pthread_cond_t *cond);
  void cook(std::string name);
  void free_oven();
  bool can_use(std::string name);
  void start();
  void end();

};
#endif