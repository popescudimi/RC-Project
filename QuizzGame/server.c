#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "thread.h"
#include "protocol.h"
#include "database.h"


/* declated in errno.h, and this variable is in one of system libraries */
extern int errno;
void threadCreate(int i, Thread *thread);

int sd; //socket descritor
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dlock = PTHREAD_MUTEX_INITIALIZER;
char** userNames;
int* scores;
int nthreads;
int activeClients;
int waitingClients;

/* port */
#define PORT 2909
int main (int argc, char *argv[])
{
    struct sockaddr_in server;

    Thread *threadsPool; 

    int on = 1;
    int i;

    activeClients = 0;
    waitingClients = 0;
   
    if(argc < 2)
    {
        fprintf(stderr,"Error: Missing argument for number of threads ...\n");
        exit(1);
    }
    nthreads = atoi(argv[1]);
   
    if(nthreads <=0)
    {
        fprintf(stderr,"Error: Invalid number of threads...\n");
	exit(1);
    }

    threadsPool = calloc(sizeof(Thread), nthreads);
    if (!threadsPool)
    {
        perror("Cannot create thread pool ");
        return errno;
    }

    userNames = calloc(sizeof(char*), nthreads);
    scores = calloc(sizeof(int), nthreads);
    for(i = 0; i < nthreads; i++)
    {
        userNames[i] = 0;
        scores[i] = 0;
    }

    sd = socket (AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror ("Cannot create socket ");
        free(threadsPool);
        free(userNames);
        free(scores);
        return errno;
    }

    bzero (&server, sizeof (server));

    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr =  htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("cannot bind to socket ");
        free(threadsPool);
        free(userNames);
        free(scores);
        return errno;
    }

    if (listen (sd, 2) == -1)
    {
        perror ("Cannot listen socket ");
        free(threadsPool);
        free(userNames);
        free(scores);
        return errno;
    }


    printf("Number of threads: %d\n", nthreads); 
    fflush(stdout);

    for(i =0 ; i < nthreads; i++) 
        threadCreate(i, &threadsPool[i]);

    for ( ; ; )
    {
        pause();
    }

    free(threadsPool);
    free(userNames);
    free(scores);
}

int registerUser(int index, const char* name) // un client se inregistreaza
{

    int i;

    if (!name)
        return 0;

    if (strlen(name)==0)
        return 0;

    pthread_mutex_lock(&dlock);

    if (!userNames[index])
    {
        for(i=0; i<nthreads; i++)
        {
            if (userNames[i])
            {
                if (strcmp(userNames[i], name)==0)
                {
                    pthread_mutex_unlock(&dlock);
                    return 0;
                }
            }
        }
        userNames[index] = strdup(name);
        scores[index] = 0; // scorul initial va fi 0
        activeClients++; // numarul de clienti activi creste
        pthread_mutex_unlock(&dlock);
        return 1;
    }
    pthread_mutex_unlock(&dlock);
    return 0;
}

void unregisterUser(int index) // daca clientul paraseste jocul, atunci el va primi index-ul 0
{
    pthread_mutex_lock(&dlock);

    if (userNames[index])
    {
        free(userNames[index]);
        activeClients--;
    }
    userNames[index] = 0;
    scores[index] = 0;
    pthread_mutex_unlock(&dlock);
}

void *threadProc(void * arg)
{
    int k, client;  // client socket
    int index = (int)arg;
    int waiting = 0;
    struct sockaddr_in from; 

    bzero(&from, sizeof (from));
    //printf ("Thread %d - listening...\n", (int)index);
    

    while(1)
    {
        socklen_t length = (socklen_t)sizeof(from);
        time_t startTime;
        pthread_mutex_lock(&mlock);
   
        printf ("Thread %d - listening...\n", (int)index);
        fflush(stdout);

        client = accept(sd, (struct sockaddr *) &from, &length);
        if (client < 0)
        {
            perror ("Cannot accept socket ");
        }
        pthread_mutex_unlock(&mlock);

        if (client >= 0)
        {
            char* line;
            int disconnected = 0;
            int* ids;
            int q = 0;
            int timeout;
            printf ("Thread %d - connected...\n", index);
            fflush(stdout);

            writeLine(client, "Hello from server\n");
            ids = malloc(sizeof(int) * getQuestionsCount());

            for(k=0; k<getQuestionsCount(); k++)
                ids[k] = k + 1;

            for(k=0; k<getQuestionsCount(); k++)
            {
                int p1 = rand() % getQuestionsCount();
                int p2 = rand() % getQuestionsCount();
                int t = ids[p1];
                ids[p1] = ids[p2];
                ids[p2] = t;
            }

            while(!disconnected)
            {
                line = readLine(client);
                if (line)
                {
                    if (startsWith(line, REGISTER_USER_COMMAND))
                    {        

                        if (registerUser(index, &line[strlen(REGISTER_USER_COMMAND)]))
                        {
                            writeLine(client, SUCCESS_USER_REGISTER); // Inregistrare cu succes
                            
                            writeLine(client, "\n");
                            printf("Thread %d - user has been registered\n", index);
                        }
                        else
                        {
                            writeLine(client, FAILED_USER_REGISTER);
                            writeLine(client, "\n");
                            printf("Thread %d - user has not been registered\n", index);
                        }                        
                        fflush(stdout);
                    }

                    if (startsWith(line, GET_QUESTION_COMMAND)) // trimiterea intrebarilor
                    {
                        char buffer[512];
                        if (q<16)
                        {
                            int id = ids[q];
 
                            getQuestionText(id, buffer);
                            timeout = getQuestionTimeout(id);

                            printf("Thread %d - sending question %d (time to answer = %d)...\n", index, id, timeout);
                            fflush(stdout);
     
                            writeLine(client, QUESTION_TEXT_COMMAND);
                            writeLine(client, buffer);
                            writeLine(client, "\n");
                            for(k=0; k<getQuestionAnswers(id); k++)
                            {
                                getQuestionAnswerText(id, k, buffer);
                                writeLine(client, ANSWER_OPTION_COMMAND);
                                writeLine(client, buffer);
                                writeLine(client, "\n");
                                printf("Thread %d - sending answer...\n", index);
                                fflush(stdout);
                           }
                           writeLine(client, DONE_QUESTION_COMMAND);
                           writeLine(client, "\n");
                           startTime = time(NULL);
                           printf("Thread %d - sent (%d answers)...\n", index, k);
                           fflush(stdout);
                       }
                       else
                       {
                           printf("Thread %d - sending notification about finishing with questions...\n", index);
                           fflush(stdout);
                           writeLine(client, NO_QUESTIONS_COMMAND);
                           writeLine(client, "\n");
                           pthread_mutex_lock(&dlock);
                           waiting = 1;
                           waitingClients++;
                           pthread_mutex_unlock(&dlock);
                       }
                    }
                    if (startsWith(line, ANSWER_COMMAND)) // primirea raspunsurilor
                    {
                        int answerId = atoi(&line[strlen(ANSWER_COMMAND)]);
                        int id = ids[q];
                        pthread_mutex_lock(&dlock);

                        if ( difftime(time(NULL), startTime) <= timeout)     // TIMEOUT, nu ai raspuns la timp              
                            scores[index] += getScoreAnswer(id, answerId, NULL);
                        else {
                            printf("Thread %d - Timeout on answer\n", index);
                            fflush(stdout);
                        }

                        printf("Thread %d - score = %d\n", index, scores[index]);
                        fflush(stdout);
                        pthread_mutex_unlock(&dlock);
                        q++;
                    }
                    if (startsWith(line, WHOIS_WINNER_COMMAND))
                    {
                        int finished = 0;
                        do
                        { 
                            pthread_mutex_lock(&dlock);
                            finished = (waitingClients==activeClients);
                            if (finished)
                            {
                                 int i, j, maxScore = 0;

                                 j = -1;
                                 for(i=0; i<nthreads; i++)
                                 {
                                     if (scores[i]>maxScore)
                                     {
                                         maxScore = scores[i];
                                         j = i;
                                     }
                                 }

                                 if (j>=0)
                                     writeLine(client, userNames[j]);
                                 writeLine(client, "\n");
                            }
                            pthread_mutex_unlock(&dlock);
                            sleep(1);
                        }
                        while(!finished);
                    }
                    free(line);
                }
                else
                {
                    unregisterUser(index);
                    pthread_mutex_lock(&dlock);
                    if (waiting)
                        waitingClients--;
                    pthread_mutex_unlock(&dlock);

                    printf("Thread %d - disconnected...\n", index);
                    fflush(stdout);
                    disconnected = 1;
                }
            }

            /* close client socket at finally*/
            close(client);
            free(ids);
        }
    }
    printf("Fatal error: should not be here\n\n");
}

void threadCreate(int i, Thread *thread)
{
    pthread_create(&(thread->id),NULL,&threadProc,(void*)i);
}
