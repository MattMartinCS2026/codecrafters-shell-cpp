#include <cstddef>
#include <cstdlib>
#include "unistd.h"
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR = ';';
#else
constexpr char PATH_LIST_SEPARATOR = ':';
#endif

/* contains all built in commands */
const std::unordered_map<std::string, int> COMMAND_MAP = {
  {"exit", 1},
  {"echo", 2},
  {"type", 3}
};

struct command {
  int code = 0;
  std::string command = "";
  std::string parameters = "";
};

/* 
  take in a potential PATH and search through it.  If its found, return the path, if not, return "".
*/
std::string find_file(std::string file_name) {
  std::string env = getenv("PATH");
  std::filesystem::path directory_path;
  size_t delimiter;
  std::string curr_file;
  size_t file_delimiter;

  while (true) {
    delimiter = env.find(PATH_LIST_SEPARATOR);
    directory_path = env.substr(0, delimiter);
    
    if (exists(directory_path) && is_directory(directory_path)) {
      for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory_path)) {
        file_delimiter = std::string(entry.path()).rfind("/");
        curr_file = std::string(entry.path()).substr(file_delimiter + 1);
        if (curr_file == file_name && !access(entry.path().c_str(), X_OK)) {
          return entry.path();
        }
      }
    }

    if (env.find(PATH_LIST_SEPARATOR) == std::string::npos) {
      break;
    }
    env = env.substr(delimiter + 1);
  }

  return "";
}

/* 
  Separates user input into command code, command, and parameters.  Then combines
  it into a command struct.
*/
command parse(std::string input, command& com) {
  size_t first_space = input.find(" ");

  if (first_space != std::string::npos) {
    com.command = input.substr(0, first_space);
    com.parameters = input.substr(first_space + 1);
  } else {
    com.command = input;
  }

  auto command_code = COMMAND_MAP.find(com.command);
  com.code = (command_code == COMMAND_MAP.end()) ? com.code = 0 : com.code = command_code->second;
  return com;
}

/* 
  Takes command and evaluates it based on its command code.
*/
void eval(command com) {
  switch(com.code) {
    case 1: // exit
      std::exit(EXIT_SUCCESS);
      break;
    case 2: // echo
      std::cout << com.parameters << std::endl;
      break;
    case 3: { // type
      auto command_code = COMMAND_MAP.find(com.parameters);
      if (command_code != COMMAND_MAP.end()) {
        std::cout << com.parameters << " is a shell builtin" << std::endl;
      } else {
        std::string path = find_file(com.parameters);
        if (path != "") {
          std::cout << com.parameters << " is " << path << std::endl;
        } else {
          std::cout << com.parameters << ": not found" << std::endl;
        }
      }
      break;
    }
    default:
      if (com.parameters != "") {
        std::cout << com.command << " " << com.parameters << ": command not found" << std::endl;
      } else {
        std::cout << com.command << ": command not found" << std::endl;
      }
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;
  command com;
  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);
    parse(input, com);
    eval(com);
    com.code = 0;
    com.command = "";
    com.parameters = "";
  }

  return 0;
}