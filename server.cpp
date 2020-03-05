#include <iostream>
#include<sys/socket.h>
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

int starting_bid;
int minimum_increment;
int number_of_bidders;

void readBidderInfo() {
  std::string bidder_executable;
  int number_of_arguments;

  std::cin >> bidder_executable >> number_of_arguments;

  int arg_count = 0;
  while (arg_count < number_of_arguments) {
    int arg;
    std::cin >> arg;
    arg_count++;
  } 
}

int main(int argc, char** argv) {

  std::cin >> starting_bid >> minimum_increment >> number_of_bidders;

  int i = 0;
  while (i < number_of_bidders) {
    readBidderInfo();
    i++;
  }

  return 0;
}