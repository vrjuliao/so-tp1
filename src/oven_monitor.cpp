#include "oven_monitor.hpp"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <unistd.h>

using namespace std;

OvenMonitor::OvenMonitor(int rounds){
  this->lock = new pthread_mutex_t;
  pthread_mutex_init(this->lock, NULL);
  this->thread_ids = vector<pthread_t>(SO_TP1_NAMES.size());
  this->waiting_list = list<Person *>();
  string name;
  
  for(int i=0; i<SO_TP1_NAMES.size(); i++){
    name = SO_TP1_NAMES[i];
    Person *p = new Person();
    p->name = name;
    p->round = rounds;
    pthread_cond_init(&p->condition, NULL);
    p->monitor = this;
    this->people[name] = p;
  }
}

long OvenMonitor::my_random_seed(){
  auto now = chrono::system_clock::now();
  auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);
  auto value = now_ms.time_since_epoch();
  return value.count();
}

int OvenMonitor::my_rand(int start, int end){
  srand(OvenMonitor::my_random_seed());
  return start + (rand()%(end-start+1));
}

// thread function for preventing deadlocks
void *OvenMonitor::raj(void *args){
  OvenMonitor *monitor = (OvenMonitor*)args;
  while(true){
    sleep(5);

    monitor->mutex_lock();
    if(monitor->valid_deadlock()){
      monitor->raise_random_person();
    }
    monitor->mutex_unlock();
  }

}

void OvenMonitor::raise_random_person(){
  this->waiting_list.pop_front();
  int pos = OvenMonitor::my_rand(0, this->waiting_list.size()-1);
  auto it = this->waiting_list.begin();
  advance(it, pos);
  Person *p = (*it);
  this->waiting_list.erase(it);
  this->waiting_list.push_front(p);
  this->sort_waiting_list();
  cout << "Raj detectou um deadlock, liberando " << p->name << endl;
  pthread_cond_signal(&p->condition);
}

int OvenMonitor::waiting_list_size(){
  return this->waiting_list.size();
}

bool OvenMonitor::valid_deadlock(){
  return (this->waiting_list.begin() != this->waiting_list.end()
         &&  *(this->waiting_list.begin()) == nullptr);
}

void OvenMonitor::start(){
  vector<string> instance_order = vector<string>(SO_TP1_NAMES);
  
  srand(OvenMonitor::my_random_seed());
  random_shuffle(instance_order.begin(), instance_order.end());
  int size = SO_TP1_NAMES.size();
  for(int i=0; i<size; i++){
    pthread_create(&this->thread_ids[i], NULL, &OvenMonitor::run, (void *) this->people[instance_order[i]]);
  }
  pthread_create(&this->raj_thread, NULL, &OvenMonitor::raj, (void*) this);
}

void OvenMonitor::end(){
  int size = SO_TP1_NAMES.size();
  for(int i=0; i<size;i++){
    pthread_join(this->thread_ids[i], NULL);
  }
  pthread_cancel(this->raj_thread);
}

OvenMonitor::~OvenMonitor(){
  pthread_mutex_destroy(this->lock);
  delete this->lock;
  for(auto &it: this->people){
    delete it.second;
  }
  this->people.clear();
}

void OvenMonitor::push_sorted(Person *p){
  // case 1: DEADLOCK
  auto it = ++(this->waiting_list.begin());
  if(it != this->waiting_list.end() && (*it)==nullptr)
    it = this->waiting_list.erase(it);
  this->waiting_list.push_back(p);
  if(this->in_deadlock()){
    this->waiting_list.insert(it, nullptr);
  } else {
    // case 2: a partner of someone in the queue arrives\
    // case 3: someone else arrives
    this->sort_waiting_list();
  }
}

void OvenMonitor::sort_waiting_list(){
  if(this->waiting_list.size() <= 1) return;
    
  int hw = this->index_in_wl("Howard"), bn = this->index_in_wl("Bernadette"),
      le = this->index_in_wl("Leonard"), pn = this->index_in_wl("Penny"), 
      sh = this->index_in_wl("Sheldon"), am = this->index_in_wl("Amy"),
      kp = this->index_in_wl("Kripke"), st = this->index_in_wl("Stuart");
 
  auto it = ++(this->waiting_list.begin());
  this->waiting_list.erase(it, this->waiting_list.end());

  if(hw && bn){
    if (le && pn){
      this->add_in_order("Howard", hw, "Bernadette", bn);
      this->add_in_order("Leonard", le, "Penny", pn);
      if(sh) this->waiting_list.push_back(this->people["Sheldon"]);
      if(am) this->waiting_list.push_back(this->people["Amy"]);
    }
    else if(sh && am){
      this->add_in_order("Sheldon", sh, "Amy", am);
      this->add_in_order("Howard", hw, "Bernadette", bn);
      if(le) this->waiting_list.push_back(this->people["Leonard"]);
      if(pn) this->waiting_list.push_back(this->people["Penny"]);
    }
    else {
      this->add_in_order("Howard", hw, "Bernadette", bn);
      if(le) this->waiting_list.push_back(this->people["Leonard"]);
      if(pn) this->waiting_list.push_back(this->people["Penny"]);
      if(sh) this->waiting_list.push_back(this->people["Sheldon"]);
      if(am) this->waiting_list.push_back(this->people["Amy"]);
    }
  } else if(le && pn){
    if(sh && am){
      this->add_in_order("Leonard", le, "Penny", pn);
      this->add_in_order("Sheldon", sh, "Amy", am);
      if(hw) this->waiting_list.push_back(this->people["Howard"]);
      if(bn) this->waiting_list.push_back(this->people["Bernadette"]);
    }
    else {
      this->add_in_order("Leonard", le, "Penny", pn);
      if(sh) this->waiting_list.push_back(this->people["Sheldon"]);
      if(am) this->waiting_list.push_back(this->people["Amy"]);
      if(hw) this->waiting_list.push_back(this->people["Howard"]);
      if(bn) this->waiting_list.push_back(this->people["Bernadette"]);
    }
  } else if (sh && am){
    this->add_in_order("Sheldon", sh, "Amy", am);
    if(hw) this->waiting_list.push_back(this->people["Howard"]);
    if(bn) this->waiting_list.push_back(this->people["Bernadette"]);
    if(le) this->waiting_list.push_back(this->people["Leonard"]);
    if(pn) this->waiting_list.push_back(this->people["Penny"]);
  }
  else {
    if(!hw){
      if(le) this->waiting_list.push_back(this->people["Leonard"]);
      if(pn) this->waiting_list.push_back(this->people["Penny"]);
      if(sh) this->waiting_list.push_back(this->people["Sheldon"]);
      if(am) this->waiting_list.push_back(this->people["Amy"]);
    } else if (!le){
      if(sh) this->waiting_list.push_back(this->people["Sheldon"]);
      if(am) this->waiting_list.push_back(this->people["Amy"]);
      if(hw) this->waiting_list.push_back(this->people["Howard"]);
      if(bn) this->waiting_list.push_back(this->people["Bernadette"]);
    } else {
      if(hw) this->waiting_list.push_back(this->people["Howard"]);
      if(le) this->waiting_list.push_back(this->people["Leonard"]);
    }
  }
  if(st) this->waiting_list.push_back(this->people["Stuart"]);
  if(kp) this->waiting_list.push_back(this->people["Kripke"]);

}

void OvenMonitor::add_in_order(string p_name1, int p_index1, string p_name2, int p_index2){
  if (p_index1>p_index2){
    this->waiting_list.push_back(this->people[p_name2]);
    this->waiting_list.push_back(this->people[p_name1]);
  } else {
    this->waiting_list.push_back(this->people[p_name1]);
    this->waiting_list.push_back(this->people[p_name2]);
  }
}

int OvenMonitor::index_in_wl(string name){
  int pos = 1;
  auto it = ++(this->waiting_list.begin());
  for(;it != this->waiting_list.end(); ++it, ++pos)
    if ((*it)->name == name) return pos;
  return 0;
}

bool OvenMonitor::is_in_list(string name){
  return this->index_in_wl(name) != 0;
}

int OvenMonitor::number_of_couples(){
  return ((is_in_list("Howard")  && is_in_list("Bernadette"))
         + (is_in_list("Leonard") && is_in_list("Penny"))
         + (is_in_list("Sheldon") && is_in_list("Amy")));
}

bool OvenMonitor::in_deadlock(){
  // (Howard |x| Bernadette ), (Leonard |x| Penny), (Sheldon |x| Amy)
  int couples_qtt = this->number_of_couples();
  if(couples_qtt == 3) return true;
  else if (couples_qtt != 0) return false;
  else {
    return ((is_in_list("Howard")  || is_in_list("Bernadette"))
         && (is_in_list("Leonard") || is_in_list("Penny"))
         && (is_in_list("Sheldon") || is_in_list("Amy")));
  }
}

void OvenMonitor::wait(Person *p){
  cout << p->name << " quer usar o forno"<< endl;
  this->push_sorted(p);
  if(this->waiting_list.size()>1){
    pthread_cond_wait(&p->condition, this->lock);
  }
}

void OvenMonitor::mutex_lock(){
  pthread_mutex_lock(this->lock);
}

void OvenMonitor::mutex_unlock(){
  pthread_mutex_unlock(this->lock);
}

void OvenMonitor::free_oven(){
  auto it = this->waiting_list.begin();
  if(it != this->waiting_list.end()) {
    std::advance(it, 1);
    if(it != this->waiting_list.end() && (*it) != nullptr){
      pthread_cond_signal(&(*it)->condition);
    }
  }
  this->waiting_list.pop_front();
}

void *OvenMonitor::run(void *args){
  Person *it = (Person*)args;
  for(int i=0; i<(it->round); i++){
    it->monitor->mutex_lock();
    it->monitor->wait(it);
    it->monitor->mutex_unlock();
    
    //cook
    cout << it->name << " começa a usar o forno"<<endl;
    sleep(1);

    it->monitor->mutex_lock();
    it->monitor->free_oven();
    it->monitor->mutex_unlock();

    cout << it->name << " vai comer"<<endl;
    sleep(OvenMonitor::my_rand(3,6));

    cout << it->name << " voltou para o trabalho" << endl;
    sleep(OvenMonitor::my_rand(3,6));
  }
}