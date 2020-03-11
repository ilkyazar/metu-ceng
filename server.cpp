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

std::vector<int> prev_status_of_bidders;

int current_highest_bid = starting_bid;
int highest_bidder_id = 0;

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
    ii* input_data;
    oi* output_data;
    cm client_msg;

    for (int i = 0; i < number_of_bidders; i++) {
      int client_id = i + 1;
      int process_id = pids[i];

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
          input_data->info = client_msg.params;
          input_data->pid = process_id;
          input_data->type = client_msg.message_id;
          print_input(input_data, client_id);

          sm server_response;
          if (input_data->type == CLIENT_CONNECT) {
            server_response.message_id = SERVER_CONNECTION_ESTABLISHED;

            cei conn_established;
            conn_established.client_id = client_id;
            conn_established.starting_bid = starting_bid;
            conn_established.current_bid = current_highest_bid;
            conn_established.minimum_increment = minimum_increment;

            server_response.params.start_info = conn_established;

            write(fds[i][1], &server_response, sizeof(server_response));

            output_data->type = CLIENT_CONNECT;
            output_data->pid = process_id;
            output_data->info = server_response.params;
            print_output(output_data, client_id);
          }

          else if (input_data->type == CLIENT_BID) {
            server_response.message_id = SERVER_BID_RESULT;

            bi bid_info;
            
            int bid = client_msg.params.bid;

            if (bid < starting_bid) {
              bid_info.result = BID_LOWER_THAN_STARTING_BID;
            }
            else if (bid < current_highest_bid) {
              bid_info.result = BID_LOWER_THAN_CURRENT;
            }
            else if (bid - current_highest_bid < minimum_increment) {
              bid_info.result = BID_INCREMENT_LOWER_THAN_MINIMUM;
            }
            else {
              if (bid > current_highest_bid) {
                current_highest_bid = bid;
                highest_bidder_id = client_id;
              }
              bid_info.result = BID_ACCEPTED;
            }
            bid_info.current_bid = current_highest_bid;

            server_response.params.result_info = bid_info;

            write(fds[i][1], &server_response, sizeof(server_response));

            output_data->type = CLIENT_BID;
            output_data->pid = process_id;
            output_data->info = server_response.params;
            print_output(output_data, client_id);
          }

          else {
            // CLIENT_FINISHED
            int status = input_data->info.status;
            prev_status_of_bidders.push_back(status);
            open_count--;
          }
        }
      }
      
      else
        //std::cout << "No data." << std::endl;
        //open_count--;
        ;
    }

  }

  sm final_server_msg;
  final_server_msg.message_id = SERVER_AUCTION_FINISHED;

  wi winner_info;
  winner_info.winner_id = highest_bidder_id;
  winner_info.winning_bid = current_highest_bid;

  final_server_msg.params.winner_info = winner_info;

  for (int i = 0; i < number_of_bidders; i++) {
    write(fds[i][1], &final_server_msg, sizeof(final_server_msg));
  }

  print_server_finished(highest_bidder_id, current_highest_bid);

  oi* final_data;

  for (int i = 0; i < number_of_bidders; i++) {
    final_data->type = CLIENT_FINISHED;
    final_data->pid = pids[i];


    final_data->info = final_server_msg.params;
    print_output(final_data, i + 1);
  }

  
  int w;
  for(int i = 0; i < number_of_bidders; i++) {
    int wait_status;
    pid_t bidder_pid = waitpid(pids[i], &wait_status, 0); 
    int client_id = i + 1;

    if (WIFEXITED(wait_status)) {
      wait_status = WEXITSTATUS(wait_status);

      int status_match;

      (wait_status == prev_status_of_bidders[i]) ? (status_match = 1) : (status_match = 0); 

      print_client_finished(i + 1, wait_status, status_match);
    }

    close(fds[i][1]);
  }

  
    
  return 0;
}