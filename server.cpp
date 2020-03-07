#include <iostream>
#include <sys/socket.h>
#include <map>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include "./lib/logging.h"
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

int starting_bid;
int minimum_increment;
int number_of_bidders;

std::multimap<std::string, int> info_of_bidders; // Executable and delay
std::multimap<std::string, std::vector<std::string>> bidder_lines; // Executable and other arguments

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

void readBidderLines() {
  std::string bidder_executable;
  int number_of_arguments;

  std::cin >> bidder_executable >> number_of_arguments;

  std::vector<std::string> args;

  int arg_count = 0;
  while (arg_count < number_of_arguments) {
    int arg;
    std::cin >> arg;
    args[arg_count] = arg;
    arg_count++;
  }

  bidder_lines.insert(std::pair <std::string, std::vector<std::string>> (bidder_executable, args)); 
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

    std::cout << retval << std::endl;
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
        std::cout << data->type << std::endl;
        open = 0;
      }
			else if (r == 0) { // EOF
				open = 0;
      }
			else {
        //std::cout << data->type << std::endl;
        print_input(data, 0);
      }
      
    }

    else
      std::cout << "No data within 5 seconds." << std::endl;

  }

}

int main(int argc, char** argv) {

  std::cin >> starting_bid >> minimum_increment >> number_of_bidders;

  int n = 0;
  while (n < number_of_bidders) {
    readBidderInfo();
    //readBidderLines(); //key: executable value: args
    n++;
  }


  int i = 0;
  while (i < number_of_bidders) {
    auto it = info_of_bidders.cbegin();
    //auto it = bidder_lines.cbegin();

    int fd[2];
    PIPE(fd);

    int p;
    int w;


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
      dup2(fd[1], 1);
      dup2(fd[1], 0);
      close(fd[1]);
      char arr[] = {"2000"};
      char* args[1];
      args[0] = "2000";
      //execl(("./bin/" + it->first + "2000").c_str(), arr, (char *) 0);
      //execl(("./bin/" + it->first).c_str(), "2000", NULL);

      //execvp(("./bin/" + it->first).c_str(), NULL); // ---> delay should be given message
      execvp(("./bin/" + it->first).c_str(), args);
    }
  
    i++;
    it++;
  }

  return 0;
}