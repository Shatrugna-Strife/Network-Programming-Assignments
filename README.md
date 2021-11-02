# Network-Programming-Assignments

##Task1##
Create N processes. N to be taken as an argument.
*Each even process waits for a signal. Even by pid.
*Every odd process sends SIGUSR1 signal to one of the even processes created prior to
it. Even process is chosen randomly.
*When an even process receives more than M signals, it terminates itself after sending a
SIGTERM followed by SIGKILL to the last process which has sent SIGUSR1 to it. M is
taken as an argument.
*Everey process should print its pid, pid of the sending process, and the number of
signals received. Should print “Terminated Self” when exiting in case of even process. In
case of odd process, print “Terminated by <pid>”.
  
##Task2##
Write a program diningphilosopher.c for the following requirement.
*it takes N number of philosophers as CLA and creates N processes modelling each
philosopher as a process
*should continuously give chances to the maximum possible number of philosophers to eat
*it should print the process id which gets to eat.
*Program should run until Ctrl-C is pressed.
  
##Task3##
Write C program validateWebServer.c that does the following.
*This program reads from a text file 'webpages.txt' that has one webpage URL per line.
*Each webpage is a url such as https://www.bits-pilani.ac.in/Uploads/BITSAT-2020/BITSAT2020-Brochure.pdf It has domain name and the path of the file.
  *Verify whether domain exists or not
  *Verify whether that domain runs a webserver at 80
  *Find which other important port numbers (1-1024) are used by services in that server
*Do these things (1-3) for every webpage specified in the text file. Print the results to
screen.
