#include <pthread.h>

typedef struct {
    pthread_t id; // id-ul thread-ului
    int thCount;  // nr de conexiuni servite
} Thread;
