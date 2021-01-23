#ifndef
#define


#endif


// declaring mutex 
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

void wait(const Person &person);
void eat(const Person &person);
bool can_access(const Person &person);
void* execute(void *args);
void verificar();
void raj();