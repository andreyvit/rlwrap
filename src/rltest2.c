/* Simple test program that uses readline in "normal" (i.e. non-callback) mode. */


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

int main() {
  using_history();  
  while(1) {
    char *line = readline ("Enter a line: ");
    if(!line)
      break;
    add_history(line);
    printf("You typed: '%s'\n", line);
    show_history_list();
  }
  return 0;
}
  
    
