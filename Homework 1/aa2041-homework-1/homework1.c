
// Ashwin Anand RUID: 192007894 Net Id: aa2041

// Packages needed for program to run
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>


// main method of program
int main(int argc, char **argv){

	// sum variables created and value of 0 assigned
	int s1 = 0;
	int sum2 = 0;
	int sum3 = 0;
	int totalSum = 0;
	int fd[2];
	pipe(fd);
	pid_t pid_process_1;
	pid_process_1 = fork();


	if(pid_process_1 == 0){
		for(int i = 1; i <= 33; i++){
			sum1 += i;
		}
		printf("Process pid %d calculated 1 to 33 = %d\n", (int) getpid(), sum1);
		write(fd[1], &sum1, sizeof(sum1));
	}else{
		pid_t pid2 = fork();
		if(pid2 == 0){
			for(int i = 34; i <= 67; i++){
				sum2 += i;
			}
			printf("Process pid %d calculated 34 to 67 = %d\n", (int) getpid(), sum2);
			write(fd[1], &sum2, sizeof(sum2));
		}else{
			read(fd[0], &sum1, sizeof(sum1));
			for(int i = 68; i <= 100; i++){
				sum3 += i;
			}
			printf("Process pid %d calculated 68 to 100 = %d\n", (int) getpid(), sum3);
			read(fd[0], &sum2, sizeof(sum2));
			totalSum = sum1 + sum2 + sum3;

			printf("Sum from 1 to 100 is %d\n", totalSum);
		}


	}









}
