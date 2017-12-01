#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <readline/history.h>
#include <readline/readline.h>

#include "calcParser.hpp"
#include "input_bindings.hpp"

/* Name of the executable. It should be set in main() */
constStr progName = NULL;


/* Arguments to the program */
constStr *progArgs = NULL;


/* Number of arguments to the program */
size_t progArgsCount = 0;


/* Input prompt in the CLI */
char prompt[500] = ">> ";


/* Should we design the output?
   If its true then we just print the end result */
bool isQuiet = false;


/* if true print to stdout instead of stderr */
FILE* useOut4Err = stderr;

/* Switch can be turned on/off from the CLI */
bool quit = false;


/* Need a JSON output? Just use ‘-j’ flag */
bool JSONoutput = false;


/* The welcome message in the CLI */
constStr welcomeMessage = {
  "This is free software with ABSOLUTELY NO WARRANTY.\n"
};



inline void execute(constStr input) {
  try { // Parsing the input
    calcParse<float64_t> parser(input);
    parser.startParsing();
    if (JSONoutput == true) {
      printf("{ \"ans\": %lf }", parser.Ans());
    } else {
      Printf(" = ");
      printf("%lg", parser.Ans());
    }
    std::cout << std::endl;
  } catch (ERROR *e) { // Catch any errors
    if (e->isSet()) {
      if (JSONoutput == true) {
        println("{ \"error\": \"%s\" }", e->toString());
      } else {
        if (not isQuiet)
          fprintf(useOut4Err, "\n");
        fprintf(useOut4Err, "Error: %s\n", e->toString());
      }
    }
    delete e;
  }
}



int main(int argc, str argv[]) {

  // TODO: Make an init function if main() becomes bulky
  makeOperatorHashes();

  progName = *argv;
  progArgs = (constStr *)argv + 1;
  progArgsCount = argc;

  // Processing Shell Arguments
  while (true) {
    char option = getopt(argc, argv, "ce:f:jqs");
    if (option == -1)
      break;
    switch (option) {
    case 's':
      useOut4Err = stdout;
      break;
    case 'c':
      quit = true;
      break;
    case 'e':
      Printf(">> %s", optarg);
      execute(optarg);
      break;
    case 'f': {
      // Use optarg as filename
      std::ifstream f(optarg);
      if (f.is_open()) {
        char input[32768];
        while (not f.eof()) {
          f.getline(input, 32767);
          Printf(">> %s", input);
          execute(input);
        }
        f.close();
      } else {
        println("'%s' is not a file", optarg);
        exit(-1);
      }
      break;
    }
    case 'q':
      isQuiet = true;
      strcpy(prompt, "");
      break;
    case 'j':
      JSONoutput = true;
      break;
    }
  }
  if (quit == true)
    exit(0);

  Printf("%s", welcomeMessage);

  str input = nullptr;

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
    Printf("\nGood bye!\nHave a nice Day\n");
    exit(0);
  }

  // Add to GNU Readline history to access it using up arrow and C-r
  add_history(input);

  execute(input);

  goto take_input;
}
