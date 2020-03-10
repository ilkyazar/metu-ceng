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

std::vector<std::string> bidder_executables;
std::vector<std::vector<std::string>> bidder_arguments;
std::vector<int> bidder_arg_counts;

void readBidderLines() {
  int number_of_arguments;
  std::string bidder_exec;
  std::vector<std::string> args;

  std::cin >> bidder_exec >> number_of_arguments;

  /*
  std::cout << "Bidder Executable is: " << bidder_exec;
  std::cout << " | Number of Arguments: " << number_of_arguments;
  */
  

  int arg_count = 0;
  while (arg_count < number_of_arguments) {
    std::string arg;
    std::cin >> arg;

    args.push_back(arg);

    //std::cout << " | Argument " << arg_count << ": " << arg;

    arg_count++;
  }
  //std::cout << std::endl;

  bidder_executables.push_back(bidder_exec);
  bidder_arg_counts.push_back(number_of_arguments);
  bidder_arguments.push_back(args);
}


int main() {

  std::cin >> starting_bid >> minimum_increment >> number_of_bidders;

  int n = 0;
  
  
  while (n < number_of_bidders) {
    readBidderLines(); //key: executable value: args
    n++;
  }
  

  int fds[number_of_bidders][2];
  pid_t pids[number_of_bidders];

  char* executable_file = (char *) malloc(100);

  for (int i = 0; i < number_of_bidders; i++) {

    strcpy(executable_file, bidder_executables[i].c_str());
    char* args[bidder_arg_counts[i] + 2] = {executable_file, NULL};

    for (int arg_nu = 0; arg_nu < bidder_arg_counts[i]; arg_nu++) {
      char* arg = (char *) malloc(100);
      strcpy(arg, bidder_arguments[i][arg_nu].c_str());
      char* arg_mutable = const_cast<char*>(arg);
      args[arg_nu + 1] = arg_mutable;
    }

    PIPE(fds[i]);

    pids[i] = fork();
    
    if (pids[i] == 0) {
      dup2(fds[i][0], 1);
      dup2(fds[i][0], 0);
      close(fds[i][1]);
      execvp(bidder_executables[i].c_str(), args);
    }
    
    close(fds[i][0]);
    
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


  while (open_count > 0) {
    FD_ZERO(&readset);
    for (int i = 0; i < number_of_bidders; i++) {
      if (open[i] == 1) {
        FD_SET(fds[i][1], &readset);
      }
    }

    select(m, &readset, NULL, NULL, NULL);

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