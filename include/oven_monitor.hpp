#ifndef TP1_SO_OVEN_MONITOR
#define TP1_SO_OVEN_MONITOR

#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <pthread.h>

using namespace std;
class OvenMonitor;

// Instances
static const map<string, string> SO_TP1_COUPLES = {
  {"Sheldon",     "Amy"}, 
  {"Howard",      "Bernadette"},
  {"Leonard",     "Penny"},
  {"Amy",         "Sheldon"},
  {"Bernadette",  "Howard"},
  {"Penny",       "Leonard"},
  {"Kripke",      ""},
  {"Stuart",      ""}};

static const vector<string> SO_TP1_NAMES = {"Sheldon", "Howard", "Leonard",
  "Stuart", "Kripke", "Amy", "Penny", "Bernadette"};

struct Person {
  std::string name;
  int round;
  pthread_cond_t condition;
  OvenMonitor *monitor;
};

class OvenMonitor {
private:
  pthread_mutex_t *lock;
  list<Person*> waiting_list;
  map<string, Person*> people;
  vector<pthread_t> thread_ids;
  pthread_t raj_thread;

  // thread function to prevent deadlocks
  static void *raj(void *args); 
  bool in_deadlock();
  int number_of_couples();
  bool is_in_list(string name);
  void sort_waiting_list();
  int index_in_wl(string name);
  void add_in_order(string p_name1, int p_index1, string p_name2, int p_index2);
  static void *run(void *args);
  bool valid_deadlock();

public:
  OvenMonitor(int rounds);
  ~OvenMonitor();

  static int my_rand(int start, int end);
  static long my_random_seed();
  int waiting_list_size();
  void raise_random_person();
  void wait(Person *p);
  void free_oven();
  void start();
  void end();
  void push_sorted(Person *p);
  void mutex_lock();
  void mutex_unlock();
};
#endif