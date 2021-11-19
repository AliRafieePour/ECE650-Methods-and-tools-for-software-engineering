#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>



int main (int argc, char **argv) {

    int fd_graph_gen2bfs[2]; // ece650-a1.py to ece650-a2.cpp
    int fd_street_gen2graph_gen[2]; // random streets to ece650-a1.py: the street generator is this file which generates random streets


    pipe(fd_graph_gen2bfs);
    pid_t pid_graph_gen2bfs = fork();
    if (pid_graph_gen2bfs < 0) { // fork failed
      std::cout << "Fork failed!" << std::endl;
      exit(1);
    } else if (pid_graph_gen2bfs == 0) {
      dup2(fd_graph_gen2bfs[0], STDIN_FILENO);
      close(fd_graph_gen2bfs[1]);

      char* const bfs_command[2] = {(char*) "./ece650-a2", NULL};
      execvp(bfs_command[0], bfs_command);
      
    }


    pipe(fd_street_gen2graph_gen);
    pid_t pid_street_gen2graph_gen = fork();
    if (pid_street_gen2graph_gen < 0) { // fork failed
      std::cout << "Fork failed!" << std::endl;
      exit(1);
    } else if (pid_street_gen2graph_gen == 0) {
      dup2(fd_street_gen2graph_gen[0], STDIN_FILENO);
      close(fd_street_gen2graph_gen[1]);

      dup2(fd_graph_gen2bfs[1], STDOUT_FILENO);
      close(fd_graph_gen2bfs[0]);

      char* const gg_command[3] = {(char*) "python", (char*) "ece650-a1.py", NULL};
      execvp(gg_command[0], gg_command);
    }
  


    std::string value;
    int option;
    int defaultNumberOfStreets, defaultNumberOfSegments, waitTime, intRange;
    while ((option = getopt(argc, argv, "s:n:l:c:")) != -1) {
      switch (option) {
      case 's': {
        value = optarg;
        defaultNumberOfStreets = atoi(value.c_str());
        if (defaultNumberOfStreets < 2) {
          std::cerr << "Error: The range of the command value is invalid!";
          exit(1);
        }
        break;
      }
      case 'n': {
        value = optarg;
        defaultNumberOfSegments = atoi(value.c_str());
        if (defaultNumberOfSegments < 1) {
          std::cerr << "Error: The range of the command value is invalid!";
          exit(1);
        }
        break;
      }
      case 'l': {
        value = optarg;
        waitTime = atoi(value.c_str());
        if (waitTime < 5) {
          std::cerr << "Error: The range of the command value is invalid!";
          exit(1);
        }
        break;
      }
      case 'c': {
        value = optarg;
        intRange = atoi(value.c_str());
        if (intRange < 1) {
          std::cerr << "Error: The range of the command value is invalid!";
          exit(1);
        }
        break;
      }
      }
    }

    char* const rgen_command[10] = {(char*) "./rgen", (char*)  "-s", (char*) std::to_string(defaultNumberOfStreets).c_str(),
                                  (char*) "-n", (char*) std::to_string(defaultNumberOfSegments).c_str(),
                                  (char*) "-l", (char*) std::to_string(waitTime).c_str(),
                                  (char*) "-c", (char*) std::to_string(intRange).c_str(), NULL};


    pid_t pid_rgen = fork();
    if (pid_rgen < 0) { // fork failed
      std::cout << "Fork failed!" << std::endl;
      exit(1);
    } else if (pid_rgen == 0) {
      dup2(fd_street_gen2graph_gen[1], STDOUT_FILENO);
      close(fd_street_gen2graph_gen[0]);
      execvp(rgen_command[0], rgen_command);
    }


    dup2(fd_graph_gen2bfs[1], STDOUT_FILENO);
    close(fd_graph_gen2bfs[0]);
    std::string ss_input;
    while(true){
      std::getline(std::cin, ss_input);
      std::cout << ss_input << std::endl;
    }
    

    waitpid(pid_rgen, NULL, 0);
    waitpid(pid_street_gen2graph_gen, NULL, 0);
    waitpid(pid_graph_gen2bfs, NULL, 0);


    return 0;
}
