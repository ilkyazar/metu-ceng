#include <iostream>
#include <sys/socket.h>
#include <map>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
//#include "./lib/logging.h"
#include "logging.h"
#include <cstring>
#include <stdlib.h>
#include <string>
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

int starting_bid;
int minimum_increment;
int number_of_bidders;

std::multimap<const char*, std::vector<std::string>> bidder_lines; // Executable and other arguments
std::multimap<std::string, int> bidder_arg_counts; // Executable and argument count

void readBidderLines(int n) {
  const char* bidder_executable;
  int number_of_arguments;

  std::string bidder_exec_str;

  std::cin >> bidder_exec_str >> number_of_arguments;

  bidder_executable = bidder_exec_str.c_str();

  /*std::cout << "Bidder Executable is: ";
  for (int i=0; bidder_executable[i]; i++) {
    std::cout << bidder_executable[i];
  }
  std::cout << " | Number of Arguments: " << number_of_arguments;
  */
  std::vector<std::string> args;

  int arg_count = 0;
  while (arg_count < number_of_arguments) {
    std::string arg;
    std::cin >> arg;

    args.push_back(arg);

    //std::cout << " | Argument " << arg_count << ": " << arg;

    arg_count++;
  }
  //std::cout << std::endl;

  bidder_lines.insert(std::pair <const char*, std::vector<std::string>> (bidder_executable, args));
  bidder_arg_counts.insert(std::pair <std::string, int> (bidder_exec_str, number_of_arguments)); 
}

void server(int fd[]) {

  int open = 1; // keep a flag for if pipe is still open
  fd_set readset;
  int m = fd[0] + 1; // maximum of file descriptors
  int r;

  ii* data;

  while (open) {
    // initialize a blocking set for select()
    FD_ZERO(&readset);
    FD_SET(fd[0], &readset);

    // Wait up to five seconds
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    // block until any of the bidders have data to read
    int retval = select(m, &readset, NULL, NULL, &tv);  // no wset, eset, timeout

    std::cout << "retval = " << retval << std::endl;
    
    /*
    if (retval == -1) {
      perror("select()");
      std::cout << "Select returned -1." << std::endl;
    }
    */
    
    // now readset contains the fd's with available data
    // check if the pipe is in readset

    if (FD_ISSET(fd[0], &readset)) { 
      
      r = read(fd[0], data, 50);
      if (r == -1) {
        std::cout << "An error occured while reading from the file descriptor." << std::endl;
        open = 0;
      }
			else if (r == 0) { // EOF
        std::cout << "EOF" << std::endl;
				open = 0;
      }
			else {
        std::cout << data->type << std::endl;
        print_input(data, 0);
      }
      
      
    }

    else
      std::cout << "No data within 5 seconds." << std::endl;

  }

}

int main() {

  std::cin >> starting_bid >> minimum_increment >> number_of_bidders;

  int n = 0;
  
  
  while (n < number_of_bidders) {
    readBidderLines(n); //key: executable value: args
    n++;
  }
  

  int fds[number_of_bidders][2];
  pid_t pids[number_of_bidders];

  char* arg1 = (char *) malloc(100);
  char* arg2 = (char *) malloc(100);
  

  for (int i = 0; i < number_of_bidders; i++) {
    std::multimap<const char*, std::vector<std::string>>::iterator it;
    it = bidder_lines.begin();

    std::multimap<std::string, int>::iterator c;
    c = bidder_arg_counts.begin();

    strcpy(arg1, it->first);
    strcpy(arg2, it->second[0].c_str());
    char* const args[4] = {arg1, arg2, NULL};

    PIPE(fds[i]);

    pids[i] = fork();
    
    if (pids[i] == 0) {
      dup2(fds[i][0], 1);
      dup2(fds[i][0], 0);
      close(fds[i][1]);
      //close(fds[i][0]);
      //dup2(fds[i][1], 0);
      //execv("./bin/Bidder", argv);
      execvp(c->first.c_str(), args);
      /*
      if (execvp("./Bidder", args) < 0) {
        std::cout << "exec err" << std::endl;
      }
      */
    }
    
    close(fds[i][0]);
    

    it++;
  }

  fd_set readset;
  int open[number_of_bidders];
  int open_count = number_of_bidders;

  for (int i = 0; i < number_of_bidders; i++) {
    open[i] = 1;
  }

  int m = 0;
  for (int i = 0; i < number_of_bidders; i++) {
    if (fds[i][1] > m) {
      m = fds[i][1];
    }
  }

  m = m + 1;

  struct timeval tv;
  tv.tv_sec = 3;
  tv.tv_usec = 0;

  while (open_count > 0) {
    FD_ZERO(&readset);
    for (int i = 0; i < number_of_bidders; i++) {
      if (open[i] == 1) {
        //std::cout << "set.." << std::endl;
        FD_SET(fds[i][1], &readset);
      }
    }

    select(m, &readset, NULL, NULL, &tv);
    int r;
    ii* data;
    cm client_msg;

    for (int i = 0; i < number_of_bidders; i++) {
      if (FD_ISSET(fds[i][1], &readset)) {
        r = read(fds[i][1], (void *) &client_msg, sizeof(client_msg));
        if (r == -1) {
          //std::cout << "An error occured while reading from the file descriptor." << std::endl;
          open[i] = 0;
          open_count--;
        }
        else if (r == 0) { // EOF
          //std::cout << "EOF" << std::endl;
          open[i] = 0;
          open_count--;
        }
        else {
          //std::cout << data->type << std::endl;
          data->info = client_msg.params;
          data->pid = pids[i];
          data->type = client_msg.message_id;
          print_input(data, i+1);
        }
      }
      else
        //std::cout << "No data." << std::endl;
        open_count--;
    }

  }

  


/*
int w;
for(int i = 0; i < number_of_bidders; i++)
  wait(&w);
*/
    
return 0;
}