#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <readline/history.h>
#include <readline/readline.h>

#include "calcParser.hpp"
#include "input_bindings.hpp"

constStr progName = NULL;
constStr *progArgs = NULL;
size_t progArgsCount = 0;
char prompt[500] = ">> ";
str input = NULL;
bool isQuiet = false;
bool quitAfterExecutingFile = true;
constStr welcomeMessage = {
  "This is free software with ABSOLUTELY NO WARRANTY.\n"
  "For details type `warranty'.\n"
  "Type help and press return to know more.\n"
};


int main(int argc, str argv[]) {

  makeOperatorHashes();

  progName = *argv;
  progArgs = (constStr *)argv + 1;
  progArgsCount = argc;

  bool executingFile = false;
  // Processing Shell Arguments
  while (true) {
    char option = getopt(argc, argv, "cf:q");
    if (option == -1)
      break;
    switch (option) {
    case 'c':
      quitAfterExecutingFile = false;
    case 'f': {
      // Use optarg as filename
      std::ifstream f(optarg);
      if (f.is_open()) {
        executingFile = true;
        char in[32768];
        while (not f.eof()) {
          f.getline(in, 32768);
          str input = trimSpaces(in);
          if (input[0] != '\0') {
            try { // Parsing the line
              calcParse<float64_t> parser(input);
              parser.startParsing();
              Printf(">> %s = ", input);
              Println("%lg", parser.Ans());
            } catch (ERROR *e) { // Catch any errors
              std::cerr << e->toString() << std::endl;
              delete e;
            }
          }
          delete input;
        }
        f.close();
      } else {
        Println("'%s': is not a file", optarg);
        exit(-1);
      }
      break;
    }
    case 'q':
      isQuiet = true;
    }
  }
  if (executingFile == true && quitAfterExecutingFile == true)
    exit(0);

  Printf("%s", welcomeMessage);

  init_readline();

  // A cleaner approach to write infinite loop without considering it to be
  // infinite instead quiting at a certain condition
 take_input:
  if (input)
    free(input);

  // Using the GNU Readline library to take input
  input = readline(prompt);

  // Quit if there is an EOF or "exit" as an input
  if (!input || !strcmp(input, "exit")) {
    Printf("exit\nGood bye!\nHave a nice Day");
    Println("");
    exit(0);
  }

  // Do nothing if its a comment. Instead loop again.
  //if (input[0] == '#') {
  //Println("");
  //goto take_input;
  //}

  // Add to GNU Readline history to access it using up arrow and C-r
  add_history(input);

  try { // Parsing the input
    calcParse<float64_t> parser(input);
    parser.startParsing();
    Printf(" = ");
    Println("%lg", parser.Ans());
  } catch (ERROR *e) { // Catch any errors
    if (e->isSet()) {
      std::cerr << std::endl << e->toString();
    }
    std::cerr << std::endl;
    delete e;
  }

  goto take_input;
}
