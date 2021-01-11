#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int main(){
	struct sembuf ad[2];
	struct sembuf sd[1];

	union semun setval;
	unsigned short val[1];

	int n;

	printf("Enter the number of philosophers:");
	scanf("%d%*c",&n);

	int id[n];
	key_t key[n];
	for(int i =1;i<=n;i++)key[i-1] = i;
	for(int i =0;i<n;i++){
		id[i] = semget(key[i],1,IPC_CREAT | 0666);
		if(id[i]<0){
			printf("id error");
			exit(0);
		}
	}

	ad[0].sem_num = 0;
	ad[0].sem_op = 0;
	ad[0].sem_flg = 0;
	ad[1].sem_num = 0;
	ad[1].sem_op = 1;
	ad[1].sem_flg = 0;
	
	sd[0].sem_num = 0;
	sd[0].sem_op = -1;
	sd[0].sem_flg = 0;

	pid_t child;
	int phid = 0;
	for(int i = 0;i<n;i++){
		child = fork();
		//sleep(1);
		if(child == 0){
			phid = i+1;
			break;
		}
	}

	if(child ==0){
		for(;;){
			semop(id[phid-1],sd,1);
			printf("Philosopher %d pid %ld ate\n", phid,(long)getpid());
		}
	}else{
		//for(int i =0;i<n;i++){
			//sleep(1);
			//semop(id[i],ad,2);
			//printf("Sent to %d\n", i+1);
		//}
		int max = n/2;
		sleep(1);
		for(;;){
			for(int i = 0;i<n;i++){
				for(int j =0;j<max;j++){
					semop(id[(i+j*2)%n],ad,2);
					sleep(0.01);
				}
				printf("\n");
				sleep(1);
			}
			//sleep(1);
		}
		int status;
		while(wait(&status)>0);
	}
}

