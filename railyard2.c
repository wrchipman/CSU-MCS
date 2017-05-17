/******************************************************************************
*								              
*			William Chipman					      *
*
*			     CS451					      *
*
*			     LAB 2					      *
*
*									      *
*
******************************************************************************/


#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

//Declare global variables

	#define MAX_TRAINS 5
	volatile sig_atomic_t fork_time = 1;	
	volatile sig_atomic_t sleepy_time = 1;
	int train_count, i, empty, curr;
	int track[MAX_TRAINS];
        int train_waiting_queue[MAX_TRAINS];
	struct itimerval tout_val;
	
void catch_child (int sig)
{

//Decrease the train count by 1    
	train_count--;
   	track[curr++]=0;
   	if (curr == MAX_TRAINS)
		curr = 0;

  	fork_time = 1;
  	signal (sig, catch_child);
	
	
}

void alarm_wakeup (int k) 
{
	sleepy_time = 0;
	signal (SIGALRM, alarm_wakeup);
	

}
void set_light_brakes (int track) {

//If no trains in queue, set lights to green, brakes to off
//else set lights to red and brakes to on.

	if (track<1) {
		printf("No tracks are active\nLights are GREEN\nBrakes are OFF\n\n");
		fflush(stdout);
	} else {

		printf("Lights are RED\nBrakes are ON\n\n");

	}

}//end set_light_brake


//Child Function

void fork_do(int track_num)
{

		printf("Train on track %d is on Exit track\n", track_num+1);
		fflush(stdout);
//train requires approx. 10 seconds to clear intersection 		

//Start Timer

		
		setitimer(ITIMER_REAL, &tout_val, 0);
		while (sleepy_time){}
		
//Train has finished
                printf("Train on track %d is gone\n", track_num+1);
		fflush(stdout);
		
}//end thread_do


void generate_new_train () {

	int rand_num = (rand()%100);
		
//If available, random % chance of new train
	if (rand_num < 33) {
		
		for (i = 0; i< MAX_TRAINS; i++){
			if (track[i] == 0) {
				track[i] = 1;
				printf("Track %d is active\n", i+1);
				fflush(stdout);
				
//Place new train on Create queue

				train_waiting_queue[empty++] = i;
				if (empty == MAX_TRAINS)
					empty = 0;

//If train is generated, increase train count.  
					train_count++;
					
   			break;
			}
		}		
	} 

}//end generate_new_train


int main (int argc, char *argv[]) {

//Controller function

srand(time(0));

signal (SIGCHLD, catch_child);
signal (SIGALRM, alarm_wakeup);


//Declare local variables / initialize global variables
	train_count = 0;
	int j;
	curr = 0;
	empty = 0;
	pid_t pid;

	

	tout_val.it_interval.tv_sec = 0;
	tout_val.it_interval.tv_usec = 0;
	tout_val.it_value.tv_sec = 10;
	tout_val.it_value.tv_usec = 0;



//Initialize track array
	for (j=0; j<MAX_TRAINS; j++){
		track[j] = 0;
		//track_waiting_queue[j] = 0;	
	}
	
	set_light_brakes(0);	

	while (1) {
		if (train_count < MAX_TRAINS) {
			generate_new_train();
		}
		
		sleep (2);
		
		if (fork_time) 
			set_light_brakes(train_count);

		if (train_count > 0 &&  fork_time) {
			
			pid = fork();
			fork_time = 0;
			if (pid < 0)  {  /* error occurred  */
       				 fprintf(stderr, "Fork Failed");
        			break;
    			}
    			else if (pid == 0)  {  /* child process  */
        			fork_do(train_waiting_queue[curr]);
				break;
    			}
			
    				
   		 }// end if

	}//end while	
} // end main


