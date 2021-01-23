#include "oven_monitor.hpp"
#include <chrono>
#include <cstdlib>

int OvenMonitor::my_rand(int start, int end){
  auto now = std::chrono::system_clock::now();
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
  return start + (std::rand()%(end-start));
}


void *OvenMonitor::monitor(){
  // put a correct flat to check if all person.threads are joined
  while(true){
    pthread_mutex_lock(mOven_lock);
    pthread_mutex_lock(mData_lock);

    //validate who will entry at the oven
    std::string name = "";
    pthread_mutex_unlock(mData_lock);
    pthread_cond_signal(mPersons[name].get_condition());
    pthread_mutex_unlock(mOven_lock);
  }
}

// thread function to prevent deadlocks
void *OvenMonitor::raj(){}

OvenMonitor::OvenMonitor(int oven_access, pthread_mutex_t *oven_lock, pthread_mutex_t *data_lock){
  std::string name;
  for(int i=0; i<SO_TP1_NAMES.size(); i++){
    name = SO_TP1_NAMES[i];
    mPersons[name] = Person(name, mRounds, this->my_rand(3,6), this);
  }

// dispara a funcao monitor() como uma thread
// dispara a funcao raj() como uma thread
}

Person OvenMonitor::get_person(std::string name){
  return mPersons[name];
}

void OvenMonitor::wait(std::string name, pthread_cond_t *cond){
  pthread_mutex_lock(mData_lock);
  // check if this person can access the oven
  if(!this->can_use(name)){
    pthread_cond_wait(cond, mOven_lock);
  }
}
bool OvenMonitor::can_use(std::string name){
  //implement a verification with all persons
  // if the person cannot to use the oven, add the name to the
  // waiting_list
}

void OvenMonitor::start(){
  // shuffle an order of calls
  //std::random_shuffle(names.begin(), names.end());
  // call threads for each person
}

void OvenMonitor::cook(std::string name){
  pthread_mutex_lock(mOven_lock);
    // printa "alguem quer usar o fogão"
  pthread_mutex_unlock(mOven_lock);
}

void OvenMonitor::free_oven(){
  //OvenMonitor.free_oven() - start
  //check who is the next and call cond_signal for this next
}

void OvenMonitor::end(){
  // wait for join of all persos threads

}