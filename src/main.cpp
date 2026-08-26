#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, int> COMMAND_MAP = {
  {"exit", 1},
  {"echo", 2}
};

struct command {
  int code;
  std::string command;
  std::string parameters;
};

auto parse(std::string input, command& com) {
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

void eval(command com) {
  switch(com.code) {
    case 1:
      std::exit(EXIT_SUCCESS);
      break;
    case 2: {
      std::cout << com.parameters << std::endl;
      break;
    }
    default:
      std::cout << com.command << " " << com.parameters << ": command not found" << std::endl;
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