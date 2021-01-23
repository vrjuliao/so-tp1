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

struct Person {
  std::string name;
  int round;
  int time_to_sleep;
  pthread_cond_t *partner_condition;
  pthread_cond_t condition;
  pthread_mutex_t *lock;
};


void wait(const Person &person);
void eat(const Person &person);
bool can_access(const Person &person);
void* execute(void *args);
void verificar();
void raj();


int my_rand(int start, int end){
  auto now = std::chrono::system_clock::now();
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
  return start + (std::rand()%(end-start));
}

void instanciate(std::map<std::string, Person> &persons,
  const std::vector<std::string> names, int round){

  for(int i=0; i<names.size(); i++){
    Person p;
    p.name = names[i];
    p.round = round;
    p.partner_condition = nullptr;
    p.condition = PTHREAD_COND_INITIALIZER;
    p.lock = nullptr;
    p.time_to_sleep = my_rand(3, 6);

    persons[names[i]] = p;
  }

}
std::map<std::string, std::string> parther_of = 
  {{"Sheldon", "Amy"}, {"Howard", "Bernadette"}, {"Leonard", "Penny"},
  {"Amy", "Sheldon"}, {"Bernadette", "Howard"}, {"Penny", "Leonard"},
  {"Kripke", ""},{"Stuart", ""}};

std::vector<std::string> names = {"Sheldon", "Howard", "Leonard",
  "Stuart", "Kripke", "Amy", "Penny", "Bernadette"};

std::map<std::string, Person> persons = std::map<std::string, Person>();

int main (int argc, const char *argv[]){
  if(argc != 2)
    std::cout <<  "The only execution argument is the number "
      << "of rounds" << std::endl;

  int counter = atoi(argv[1]);

  //create persons
  instanciate(persons, names, counter);

  // create an order to call each thread
  std::random_shuffle(names.begin(), names.end());
  
  // call
  for(int i=0; i<names.size(); i++)
    execute(&(persons[names[i]]));

  raj();

  return 0;
}

void* execute(void *args){
  Person *person = (Person*)args;

  for(int i=0; i<person->round; i++){
    sleep(person->time_to_sleep);

    pthread_mutex_lock(&data_lock);
    // check if this person can access the oven
    if(!can_access(*person)){
      pthread_cond_wait(&person->condition, &oven_lock);
    }

    pthread_mutex_lock(&oven_lock);
     // usar o fogão

    pthread_mutex_unlock(&oven_lock);
    
    //eat
    sleep(person->time_to_sleep);

    //go to work
    sleep(person->time_to_sleep);
  
  }

}

// monitor declaring mutex 
pthread_mutex_t oven_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;

std::vector<Person> waitign_list;

void *monitor(){
  
  while(true){
    pthread_mutex_lock(&oven_lock);
    pthread_mutex_lock(&data_lock);

    //validate who will entry at the oven
    std::string name = "";
    pthread_mutex_unlock(&data_lock);
    pthread_cond_signal(&(persons[name].condition));
    pthread_mutex_unlock(&oven_lock);
  }
}



bool can_access(const Person &person){
  return true;
  // if the partner is in the list, then it should free the signal
}

// monitor do forno
void wait(const Person &person){
  std::cout << person.name << " quer usar o forno" << std::endl;
}

void eat(const Person &person){
  std::cout << person.name << " vai comer" << std::endl;

}

//Raj
void verificar(){
  // validacao de deadlock
  return ;
}

