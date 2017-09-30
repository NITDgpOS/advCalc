#include <malloc.h>
#include <readline/readline.h>

static bool trim_spaces = false;

static int return_pressed(int x, int input_char) {
  if ((input_char == RETURN || input_char == NEWLINE) && rl_end) {
    if (rl_line_buffer[rl_end - 1] == SPACE &&
        strncasecmp(rl_line_buffer, "prompt=", 7))
      rl_line_buffer[--rl_end] = '\0';
    rl_point = rl_end;
    rl_redisplay();
    rl_done = true;
  }
  return x;
}

static int space_pressed(int x, int input_char) {
  if (input_char == SPACE) {
    if (rl_point && rl_line_buffer[rl_point - 1] != SPACE &&
        (rl_point == rl_end || rl_line_buffer[rl_point] != SPACE)) {
      rl_insert(x, input_char);
    } else if (!strncasecmp(rl_line_buffer, "prompt=", 7)) {
      trim_spaces = true;
      rl_insert(x, input_char);
    }
  }
  return x;
}
/* under dev code, edit/use with precaution */
static int rubout_pressed(int x, int input_char) {
  if (trim_spaces == true) {
    long i = 0, j = 0, k;

    for (; rl_line_buffer[i] == SPACE; ++i) {
      /* counting spaces */
    }

    if (i != 0) {
      if (rl_point < i)
        rl_point = 0;

      while (rl_line_buffer[i] != '\0') {
        rl_line_buffer[j++] = rl_line_buffer[i++];
      }
      rl_line_buffer[j] = '\0';
    }

    for (; rl_line_buffer[i] != '\0'; ++i) {
      if (rl_line_buffer[i] != SPACE)
        continue;

      for (j = i; rl_line_buffer[j] == SPACE; ++j) {
        /* counting spaces */;
      }

      for (k = i + 1; rl_line_buffer[j] != '\0'; ++k, ++j) {
        rl_line_buffer[k] = rl_line_buffer[j];
      }
      rl_line_buffer[k] = '\0';

      if (k == i + 1)
        break;
    }

    if (rl_point == rl_end)
      rl_point = i;
    rl_end = i;

    trim_spaces = false;
  }
  return rl_delete(x, input_char);
}
/* **************************************** */
static int do_nothing(int x, int input_char) { return x + input_char; }

void init_readline() {
  rl_bind_key(SPACE, space_pressed);
  rl_bind_key(TAB, do_nothing);
  rl_bind_key(RETURN, return_pressed);
  rl_bind_key(NEWLINE, return_pressed);
  /* under dev line, use/edit with precaution */
  rl_bind_key(RUBOUT, rubout_pressed);
  /* **************************************** */
}
