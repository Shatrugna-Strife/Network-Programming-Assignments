#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<signal.h>
#include<time.h>
#include<string.h>
#include<sys/wait.h>

int n;
int M;
int count_dead;
pid_t *ep;
pid_t *all;
int eh;

void even_user(int signo, siginfo_t* sig, void*context){
	count_dead++;
	if(count_dead > M){
		printf("Process %d received SIGUSR1 signal from process %d\n", getpid(), sig->si_pid);
		printf("Process %d sent SIGTERM signal to process %d\n", getpid(), sig->si_pid);
		printf("Process %d sent SIGKILL signal to process %d\n", getpid(), sig->si_pid);
		kill(sig->si_pid, SIGTERM);
		//kill(sig->si_pid, SIGKILL);
		printf("Process %d Terminated Self after receiveing %d SIGUSR1 signals\n", getpid(), count_dead);
		exit(0);
	}
	printf("Process %d received SIGUSR1 signal from process %d and had received %d signals\n", getpid(), sig->si_pid, count_dead);
}

void even_kill(int signo){
	printf("Process %d received %d signals from other processes and one signal from bash for terminating\n", getpid(), count_dead);
	exit(0);
}


void even(){
	struct sigaction evenusr;
	evenusr.sa_sigaction = &even_user;
	evenusr.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &evenusr, NULL);
	signal(SIGTERM, even_kill);
	pause();
}

void odd_term(int signo, siginfo_t *sig, void*context){
	bool c = false;
	for(int i = 0; i<eh;i++){
		if(ep[i] == sig->si_pid){
			c = true;
			break;
		}
	}
	if(!c){
		printf("Process %d terminated by receiving SIGTERM signal from bash\n", getpid());
		exit(0);
	}
	printf("Process %d received SIGTERM from process %d\n", getpid(), sig->si_pid);	
	printf("Process %d received SIGKILL from process %d\n", getpid(), sig->si_pid);	
	printf("Process %d Terminated by process %d\n", getpid(), sig->si_pid);
	exit(0);
}

void odd_kill(int signo){
	printf("Process %d received SIGKILL signal", getpid());
}

void odd(){
	struct sigaction oddterm;
	oddterm.sa_sigaction = &odd_term;
	oddterm.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &oddterm, NULL);
	//signal(SIGKILL, odd_kill);
	int tot[eh];
	int done = 0;
	int prev = 0;
	memset(tot, 0, eh*sizeof(int));
	if(eh != 0){
		while(done != eh){
			int r = rand()%eh;
			if(tot[r] == M+1){
				done++;
				continue;
			}
			tot[r] += 1;
			if(prev == r)sleep(1);
			prev = r;
			printf("Process %d sent SIGUSR1 signal to process %d\n", getpid(), ep[r]);
			kill(ep[r],SIGUSR1);
			//sleep(1);
			//usleep(30000);
		}
	}
	printf("Process %d has no even process to send signal to\n",getpid());
	exit(0);
	pause();
}
	

int main(int argc, char *argv[]){
	if(argc<3){
		printf("Enter 2 arguments while executing\n");
		exit(0);
	}
	n = atoi(argv[1]);
	M = atoi(argv[2]);
	int status;
	eh = 0;
	count_dead = 0;
	ep = (pid_t*)malloc(n*sizeof(pid_t));
	all = (pid_t*)malloc(n*sizeof(pid_t));
	pid_t child;
	printf("Number of processes %d and M is %d\n", n, M);
	for(int i =0;i<n;i++){
		sleep(1);
		child = fork();
		if(child == 0){
			sleep(2);
			if(getpid()%2 == 0)even();
			else odd();
			break;
		}
		else if(child>0){
			printf("Process %d created\n", child);
			all[i] = child;
			if(child%2 == 0){
				ep[eh] = child;
				eh++;
			}
		}
	}
	for(int i = 0;i<n;i++){
		if(all[i]%2 ==1)waitpid(all[i],&status,0);
	}
	//for(int i =0;i<eh;i++)kill(ep[i], SIGTERM);
	//sleep(30);
}
