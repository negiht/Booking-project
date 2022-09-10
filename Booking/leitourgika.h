#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

//for bool types
#include <stdbool.h>

//to avoid implicit
//declaration warnings
#include <unistd.h>
#include <ctype.h>

//constants
#define Nseat 10
#define NzoneA 10
#define NzoneB 20
#define PzoneA 0.3
#define CzoneA 30
#define CzoneB 20
#define Nseatlow 1
#define Nseathigh 5
#define treslow 1
#define treshigh 5
#define tseatlow 5
#define tseathigh 13
#define tcashlow 4
#define tcashhigh 8
#define Pcardsucces 0.9

//initial number of
//telephones and cashiers
int Ntel=3;
int Ncash=2;

int telwait=0;
int cashwait = 0;
int bank=0;
int telfail_count = 0;
int cashfail_count = 0;
int succ_count = 0;
unsigned int seed = 0;

int A[NzoneA][Nseat];
int B[NzoneB][Nseat];


//variables initialization
//for gettime function
struct timespec start,finish;
long * F_times;



//routine declaration
void * booking(void * x);



//function declaration
//for checking if an argument
//is a positive number
bool isPositiveN(char number[]);



//function for checking
//pthread actions are ok
void rc_check(int rc);



//function for checking any possible memory leaks
void memory_check(void* pointer);



//mutexes declarations
pthread_mutex_t tel_lock;
pthread_mutex_t cash_lock;
pthread_mutex_t time_lock;
pthread_mutex_t screen_lock;
pthread_mutex_t plan_lock;



//conditions declarations
pthread_cond_t tel_cond;
pthread_cond_t cash_cond;



//struct declaration
//for the order arguments
typedef struct booking_args{
	int number_of_seats;
	int id;
	char zone;
	//time_t f_time;



} bookings_ids;
