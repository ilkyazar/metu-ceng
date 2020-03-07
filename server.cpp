#include <iostream>
#include <sys/socket.h>
#include <map>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include "./lib/logging.h"
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

int starting_bid;
int minimum_increment;
int number_of_bidders;

std::multimap<std::string, int> info_of_bidders;

void readBidderInfo() {
  std::string bidder_executable;
  int number_of_arguments;
  int delay;

  std::cin >> bidder_executable >> number_of_arguments >> delay;

  int arg_count = 0;
  while (arg_count < number_of_arguments - 1) {
    int arg;
    std::cin >> arg;
    arg_count++;
  }

  info_of_bidders.insert(std::pair <std::string, int> (bidder_executable, delay)); 
}

void server(int fd[]) {
  std::cout << "SERVER" << std::endl;

  int open = 1; // keep a flag for if pipe is still open
  fd_set readset;
  int m = fd[0] + 1; // maximum of file descriptors
  int r;

  ii* data;

  while (open) {
    std::cout << "WHILE" << std::endl;
    // initialize a blocking set for select()
    FD_ZERO(&readset);
    FD_SET(fd[0], &readset);

    // Wait up to five seconds
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    
    // block until any of the bidders have data to read
    int retval = select(m, &readset, NULL, NULL, &tv);  // no wset, eset, timeout
    if (retval == -1) {
      perror("select()");
    }
    
    // now readset contains the fd's with available data
    // check if the pipe is in readset

    else if (FD_ISSET(fd[0], &readset)) { 
      std::cout << "else if" << std::endl;
      
      r = read(fd[0], data, 50);
      std::cout << "r = " << r << std::endl;
      if (r == -1) {
        std::cout << "En error occured while reading from the file descriptor." << std::endl;
        open = 0;
      }
			else if (r == 0) { // EOF
				open = 0;
        std::cout << "if" << std::endl;
      }
			else {
        //std::cout << data->type << std::endl;
        print_input(data, 0);
        std::cout << "else" << std::endl;
      }
      
    }

    else
      std::cout << "No data within three seconds." << std::endl;

  }

}

int main(int argc, char** argv) {

  std::cin >> starting_bid >> minimum_increment >> number_of_bidders;

  int n = 0;
  while (n < number_of_bidders) {
    readBidderInfo();
    n++;
  }

  for (auto it = info_of_bidders.cbegin(); it != info_of_bidders.cend(); ++it) {
    std::cout << it->first << " " << it->second << std::endl;
  }

  int i = 0;
  while (i < number_of_bidders) {
    auto it = info_of_bidders.cbegin();

    int fd[2];
    // support communication via bidirectional sockets
    PIPE(fd);

    int p;
    int w;
    /*
      fd[0] --> read
      stdin: 0
      stdout: 1
      stderr: 2
    */

    if (p = fork()) { // PID of the child process is returned in the parent
		  close(fd[1]);
      dup2(fd[0], 0);
      dup2(fd[0], 2);
      close(fd[0]);
      server(fd);
      //wait(&w);
    }
    else { // 0  is returned in the child
      close(fd[0]);
      //dup2(fd[1], 1);
      //std::cout << "DUP2 FD 1 -> 1" << std::endl;
      //dup2(fd[1], 0);
      //std::cout << "DUP2 FD 1 -> 0" << std::endl;
      close(fd[1]);
      execl((it->first).c_str(), (char *) 0);
    }
  
    i++;
    it++;
  }

  return 0;
}