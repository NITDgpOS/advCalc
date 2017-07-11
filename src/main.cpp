#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "calcParser.hpp"
#include "input_bindings.hpp"

const char *prog_name = NULL;
char prompt[500] = ">> ";
char *input = NULL;

int main(int argc, char *argv[]) {

  makeOperatorHashes();

  /* ******************************************************************* */
  /*                             Welcome message                         */
  fprintf(stdout, "This is free software with ABSOLUTELY NO WARRANTY.\n"
                  "For details type `warranty'.\n"
                  "Type help and press return to know more.\n");
  /* ******************************************************************* */

  init_readline();

  do {
    if (input)
      free(input);

    input = readline(prompt);
    calcParse<float64_t> parser(input);

    if (!strcmp(input, "#")) // A comment
      continue;              // Do nothing

    add_history(input);

    parser.startParsing(); // Calculate
  } while (!!strcmp(input, "exit"));

  return 0;
}
