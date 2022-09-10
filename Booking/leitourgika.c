#include "leitourgika.h"


int main(int argc, char ** argv){


	//check for number of arguments

	if (argc!=3 ){

		printf("Incorrect number of arguments, must be 2! \n");

		exit (-1);

	}

	//check for positive number

	if(!isPositiveN(argv[1])  || !isPositiveN(argv[2])){

		printf("Both arguments must be positive numbers!\n");

		exit (-1);
	}

	//counts
	//int telfail_count = 0;
	//int cashfail_count = 0;
	//int succ_count = 0;
	//number of bookings
	int Ncust=atoi(argv[1]);

	//bank balance
	//int bank = 0;

	//waiting time sum
	//int telwait = 0;
	//int cashwait = 0;

	//booking zone tables
	//int A[NzoneA][Nseat];
	//int B[NzoneB][Nseat];

    int i, j;
   for(i=0; i<NzoneA; i++) {
      for(j=0;j<Nseat;j++) {
         A[i][j] = 0;
      }
   }

   for(i=0; i<NzoneB; i++) {
      for(j=0;j<Nseat;j++) {
         B[i][j] = 0;
      }
   }

	//table that will contain all times
	//for the bookings
	F_times = (long*)malloc((Ncust+1) * sizeof(long));

	//checking for memory leaks
	memory_check(F_times);



	F_times[0] = 0;

	//seed
	seed=atoi(argv[2]);


	//table that will contain all threads
	pthread_t* threads=(pthread_t*)malloc(Ncust*sizeof(pthread_t));

	//checking for memory leaks
	memory_check(threads);


	//table with the bookings ids
	int* id =(int*)malloc(Ncust*sizeof(int));

	//checking for memory leaks
	memory_check(id);


	//int variable for
	//checking pthread actions are ok

	int rc;



	//mutexes and conditions initializations
	//and response code checking for each

	rc=pthread_mutex_init(&tel_lock,NULL);

	rc_check(rc);


	rc=pthread_mutex_init(&cash_lock,NULL);

	rc_check(rc);




	rc=pthread_cond_init(&tel_cond,NULL);

	rc_check(rc);



	rc=pthread_cond_init(&cash_cond,NULL);

	rc_check(rc);




	rc=pthread_mutex_init(&time_lock,NULL);

	rc_check(rc);



	rc=pthread_mutex_init(&screen_lock,NULL);

	rc_check(rc);



	rc=pthread_mutex_init(&plan_lock,NULL);

	rc_check(rc);


	//creating the threads

	for (int i=0; i<Ncust; i++){

		//ids and number of bookings
		id[i]=i+1;
		bookings_ids x;
		x.number_of_seats=rand_r(&seed) % Nseathigh + Nseatlow;
		x.id=id[i];
		int k = rand_r(&seed) % 100;
		if(k<=(PzoneA*100)){
			x.zone='A';
		}else{
			x.zone='B';
		}

		//changing the seed for
		//each thread
		//so it's more random
		seed=seed+id[i]; 											//EXTRA

		rc=pthread_create(&threads[i],NULL,booking,(void*)&x);

		rc_check(rc);

		unsigned int y= rand_r(&seed) %(treshigh)+(treslow);

		//sleeping until next booking
		sleep(y);

	}


	//waiting for all threads
	//to end their routine

	for (int i=0; i<Ncust; i++){

		rc=pthread_join(threads[i], NULL);

		rc_check(rc);
	}





	//Destroying all mutexes and conditions

	rc=pthread_mutex_destroy(&tel_lock);

	rc_check(rc);




	rc=pthread_mutex_destroy(&cash_lock);

	rc_check(rc);




	rc=pthread_cond_destroy(&tel_cond);

	rc_check(rc);




	rc=pthread_cond_destroy(&cash_cond);

	rc_check(rc);




	rc=pthread_mutex_destroy(&time_lock);

	rc_check(rc);



	rc=pthread_mutex_destroy(&screen_lock);

	rc_check(rc);



	rc=pthread_mutex_destroy(&plan_lock);

	rc_check(rc);


	//seat booking plan
	printf("\n");
	
	for(int i=0; i<NzoneA; i++) {
		 for(int j=0;j<Nseat;j++) {
				if(A[i][j] != 0){
					printf("Zone A / Row %d / Seat %d / Customer %d \n", i+1, j+1, A[i][j]);
				}
		 }
	}

	for(int i=0; i<NzoneB; i++) {
		 for(int j=0;j<Nseat;j++) {
			 if(B[i][j] != 0){
				 printf("Zone B / Row %d / Seat %d / Customer %d \n", i+1, j+1, B[i][j]);
			 }
		 }
	}

	//sum to calculate
	//average time

	int sum = 0;

	for (int i = 0;i<Ncust;i++) {

		sum += F_times[i+1];


		}

	double avg=(double)sum/Ncust;
	double telfail_perc=(double)(telfail_count*100)/Ncust;
	double cashfail_perc=(double)(cashfail_count*100)/Ncust;
	double success_perc=(double)(succ_count*100)/Ncust;
	double avgwaitt =(double)telwait/Ncust;
	double avgwaitc =(double)cashwait/(Ncust-telfail_count);


	//print messages for max and average time

	printf("\nTotal income: %d euro \n", bank);
	printf("\nFailed on finding seats: %.1f %% \n", telfail_perc);
	printf("\nFailed on payment: %.1f %% \n", cashfail_perc);
	printf("\nSuccessful bookings: %.1f %% \n", success_perc);
	printf("\nAverage telephone seat booking time: %.1f minutes \n", avgwaitt);
	printf("\nAverage cashier payment time: %.1f minutes \n", avgwaitc);
	printf("\nAverage customer serve time: %.1f minutes \n", avg);




	//freeing up
	//memory

	free(F_times);
	free(threads);
	free(id);

}





//routine for the threads
//to follow

void* booking(void * x) {


	//using the struct we
	//have created to store
	//the id and the number of seats

	bookings_ids* thread_arg;
	thread_arg = (bookings_ids*)x;


	//initializing the variables

	int id = thread_arg->id;
	int seats = thread_arg->number_of_seats;
	char zone = thread_arg->zone;

	int rc;


	//locking the time_lock mutex

	rc = pthread_mutex_lock(&time_lock);

	rc_check(rc);

	//storing the starting time of the booking

	clock_gettime(CLOCK_REALTIME, &start);

	F_times[id] = start.tv_sec;


	rc = pthread_mutex_unlock(&time_lock);

	rc_check(rc);



	//locking the cook_lock mutex

	rc = pthread_mutex_lock(&tel_lock);

	rc_check(rc);

	//while there are no cooks available
	//the thread waits

	while (Ntel == 0) {

		rc = pthread_cond_wait(&tel_cond, &tel_lock);

		rc_check(rc);


	}

	//there is a cook to handle the specific booking now

	Ntel--;


	//cook unlock

	rc = pthread_mutex_unlock(&tel_lock);

	rc_check(rc);

	//pizzas preparation time

	unsigned int w = rand_r(&seed) % (tseathigh)+(tseatlow);

	telwait += w;

	sleep(w);

	rc = pthread_mutex_lock(&plan_lock);

	rc_check(rc);


    int found = 0;
    int row = 0;
    int col = 0;
	if (zone == 'A') {
		//int found = 0;
		for (int i = 0; i < NzoneA; i++) {
			if (found == 1) {
                row = i-1;
				break;
			}
			int count = 0;
			for (int j = 0; j < Nseat; j++) {

				if (A[i][j] == 0) {
					count += 1;
				}
				else {
					count = 0;
				}

				if (count == seats) {
					found = 1;
					col = j - count + 1;
					int seat = j - count + 1;
					for (int k = 0; k < count; k++) {
						A[i][seat] = id;
						seat += 1;
					}
				}

			}
		}
	}
	else {
		//int found = 0;
		for (int i = 0; i < NzoneB; i++) {
			if (found == 1) {
				row = i-1;
				break;
			}
			int count = 0;
			for (int j = 0; j < Nseat; j++) {

				if (B[i][j] == 0) {
					count += 1;
				}
				else {
					count = 0;
				}

				if (count == seats) {
					found = 1;
					col = j - count + 1;
					int seat = j - count + 1;
					for (int k = 0; k < count; k++) {
						B[i][seat] = id;
						seat += 1;
					}
				}

			}
		}
	}

	rc = pthread_mutex_unlock(&plan_lock);

	rc_check(rc);

	rc = pthread_mutex_lock(&tel_lock);

	rc_check(rc);

	//the oven that was busy with the specific booking
	//is now available

	Ntel++;


	//we signal the threads that are sleeping

	rc = pthread_cond_signal(&tel_cond);

	rc_check(rc);


	//oven unlock

	rc = pthread_mutex_unlock(&tel_lock);

	rc_check(rc);

	//check if booking was successfull

	if (found == 0) {

		rc = pthread_mutex_lock(&time_lock);

		rc_check(rc);

		//calculating time and storing it
		//in F_times table

		clock_gettime(CLOCK_REALTIME, &finish);

		time_t finish_time = finish.tv_sec;

		time_t f_time = finish_time - F_times[id];

		F_times[id] = f_time;

		//time unlock

		rc = pthread_mutex_unlock(&time_lock);

		rc_check(rc);

		//screen lock

		rc = pthread_mutex_lock(&screen_lock);

		rc_check(rc);
		//
		telfail_count += 1;

		//booking is failed so we print the appropriate message

		printf("id: %d Η κράτηση απέτυχε γιατί δεν υπάρχουν κατάλληλες θέσεις. \n", id);

		//screen unlock

		rc = pthread_mutex_unlock(&screen_lock);

		rc_check(rc);

		//thread exits

		pthread_exit(NULL);
	}


	// ELENXOS GIA DIATHESIMES THESEIS

	//now the booking is ready to enter
	//the oven, so we lock it with
	//the appropriate mutex and check if
	//there are any available ovens

	rc = pthread_mutex_lock(&cash_lock);

	rc_check(rc);


	//while there are no ovens available
	//the thread waits

	while (Ncash == 0) {


		rc = pthread_cond_wait(&cash_cond, &cash_lock);

		rc_check(rc);

	}

	//there is an oven to enter now

	Ncash--;


	//oven unlock

	rc = pthread_mutex_unlock(&cash_lock);

	rc_check(rc);

	//bake time
	unsigned int m = rand_r(&seed) % (tcashhigh)+(tcashlow);

	cashwait += m;

	sleep(m);

	int success = 1;
	int k = rand_r(&seed) % 100;
	if (k > (Pcardsucces * 100)) {
		success = 0;
	}

	if (success == 0) {

		rc = pthread_mutex_lock(&time_lock);

		rc_check(rc);

		//calculating time and storing it
		//in F_times table

		clock_gettime(CLOCK_REALTIME, &finish);

		time_t finish_time = finish.tv_sec;

		time_t f_time = finish_time - F_times[id];

		F_times[id] = f_time;

		//time unlock

		rc = pthread_mutex_unlock(&time_lock);

		rc_check(rc);


		rc = pthread_mutex_lock(&plan_lock);

		rc_check(rc);

        if(zone == 'A'){
            for (int k = 0; k < seats; k++) {
                A[row][col] = 0;
                col += 1;
            }
        }
        else{
            for (int k = 0; k < seats; k++) {
                B[row][col] = 0;
                col += 1;
            }
        }


		rc = pthread_mutex_unlock(&plan_lock);

		rc_check(rc);

		//screen lock

		rc = pthread_mutex_lock(&screen_lock);

		rc_check(rc);

		//fails of payments

		cashfail_count += 1;

		//payment failed so we print the appropriate message


		printf("id: %d Η κράτηση απέτυχε γιατί η συναλλαγή με την πιστωτική κάρτα δεν έγινε αποδεκτή. \n", id);

		//screen unlock

		rc = pthread_mutex_unlock(&screen_lock);

		rc_check(rc);

	}
	else {

		int paid_amount;
		if (zone == 'A') {
			paid_amount = seats * CzoneA;
		}
		else {
			paid_amount = seats * CzoneB;
		}

		bank += paid_amount;

		rc = pthread_mutex_lock(&time_lock);

		rc_check(rc);

		//calculating time and storing it
		//in F_times table

		clock_gettime(CLOCK_REALTIME, &finish);

		time_t finish_time = finish.tv_sec;

		time_t f_time = finish_time - F_times[id];

		F_times[id] = f_time;

		//time unlock

		rc = pthread_mutex_unlock(&time_lock);

		rc_check(rc);

		//screen lock

		rc = pthread_mutex_lock(&screen_lock);

		rc_check(rc);

		//succ

		succ_count += 1;

		//payment succeeded so we print the appropriate message

		printf("id: %d Η κράτηση ολοκληρώθηκε επιτυχώς. Οι θέσεις σας είναι στη ζώνη <%c>, σειρά <%d>, αριθμός <", id, zone, row+1);
		int first_seat_no = col + 1;
		for (int i = 1; i <= seats; i++) {
			if (i < seats) {
				printf("%d,", first_seat_no);
				first_seat_no += 1;
			}
			else {
				printf("%d", first_seat_no);
			}
		}
		printf("> και το κόστος της συναλλαγής είναι <%d> ευρώ. \n", paid_amount);

		//screen unlock

		rc = pthread_mutex_unlock(&screen_lock);

		rc_check(rc);
	}

	rc = pthread_mutex_lock(&cash_lock);

	rc_check(rc);

	//the oven that was busy with the specific booking
	//is now available

	Ncash++;


	//we signal the threads that are sleeping

	rc = pthread_cond_signal(&cash_cond);

	rc_check(rc);


	//oven unlock

	rc = pthread_mutex_unlock(&cash_lock);

	rc_check(rc);

	//thread exits

	pthread_exit(NULL);

}

//function for checking
//memory leaks

void memory_check(void* pointer){

	if (pointer==NULL){

		printf("No memory available!\n");

		exit (-1);


	}
}

//function for checking
//if all pthread actions
//are ok

void rc_check(int rc){

	if (rc != 0) {

		printf("ERROR: return code from the specific pthread action is %d\n", rc);

		pthread_exit(&rc);
	}
}



//function for checking
//if the argument is a
//positive number

bool isPositiveN(char number[])
{

    //checking for negative numbers
    if(number[0]=='-'){

		return false;
    }

	for (int i=0; number[i] != 0; i++)
    {

        if (!isdigit(number[i])){

            return false;

	}
    }

    return true;
}
