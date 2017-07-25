#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "calcParser.hpp"
#include "input_bindings.hpp"

constStr prog_name = NULL;
char prompt[500] = ">> ";
str input = NULL;

int main(int argc, str argv[]) {

  makeOperatorHashes();

  /* ******************************************************************* */
  /*                             Welcome message                         */
  fprintf(stdout, "This is free software with ABSOLUTELY NO WARRANTY.\n"
                  "For details type `warranty'.\n"
                  "Type help and press return to know more.\n");
  /* ******************************************************************* */

  init_readline();

// A cleaner approach to write infinite loop without considering it to be
// infinite and instead quiting at a certain condition
take_input:
  if (input)
    free(input);

  input = readline(prompt);

  if (!input || !strcmp(input, "exit")) {
    !input ? printf("exit\nGood bye!\nHave a nice Day") : 0;
    printf("\n");
    exit(0);
  }

  calcParse<float64_t> parser(input);

  if (!strcmp(input, "#")) // A comment
    goto take_input;       // Do nothing

  add_history(input);

  parser.startParsing();

  if (Error.isSet()) {
    printf("\n :( %s\n", Error.toString());
    Error.reset();
  } else {
    printf(" = %lg\n", parser.Ans());
  }

  goto take_input;
}
