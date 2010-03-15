/* Simple test program that uses readline in callback mode. Whenever rlwrap misbehaves,
   please run this program to make sure it is not a readline problem */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void show_history_list() {
  HISTORY_STATE *state =  history_get_history_state();
  int i;
  printf("History list now:\n");
  for (i=0; i < state->length; i++) {
    printf("%d: '%s'%s\n", i, state->entries[i]->line, (i == state->offset? "*":""));
  }     
}       

void line_handler(char *line) {
  if (!line) 
    exit (0);
  add_history(line);
  printf("You typed: '%s'\n", line);
  show_history_list();
}

int main() {
  char c;
   
  using_history();
  rl_callback_handler_install("Enter a line: ", &line_handler);

  while (read(STDIN_FILENO, &c, 1)) {
    rl_stuff_char(c);
    rl_callback_read_char();
  }
  return 0;
}
  
    
