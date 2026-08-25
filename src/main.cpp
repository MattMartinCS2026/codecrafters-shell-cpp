#include <iostream>
#include <string>

void eval(std::string command) {
  if (command == "exit") {std::exit(EXIT_SUCCESS);}
  else {std::cout << command << ": command not found\n";}
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string command;
  while (true) {
    std::cout << "$ ";
    std::cin >> command;
    eval(command);
  }

  return 0;
}