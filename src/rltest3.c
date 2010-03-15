/* Simple test program that uses readline in callback mode. Identical to rltest1, except
   that CTRL-G is bound to a function that writes all keybindings, variable settings and macros
   to stdout */


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

static int
dump_all_keybindings(int count, int key)
{
  rl_dump_functions(count,key);
  rl_variable_dumper(0);
  rl_macro_dumper(0);
  return 0;
}

static Keymap getmap(const char *name) {
  Keymap km = rl_get_keymap_by_name(name);
  if (!km) {
    printf ("Could not get keymap '%s'\n", name);
    exit(0);
  }     
  return km;
}

static void bindkey(int key, rl_command_func_t *function, const char *maplist) {
  char *mapnames[] = {"emacs-standard","emacs-ctlx","emacs-meta","vi-insert","vi-move","vi-command",NULL};
  char **mapname;
  for (mapname = mapnames; *mapname; mapname++) 
    if(strstr(maplist, *mapname)) {
      Keymap kmap = getmap(*mapname);
      if (rl_bind_key_in_map(key, function, kmap)) {
        printf("Could not bind key %d in keymap '%s'\n", key, *mapname);
        exit(0);
      } 
    }
}       


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
  bindkey(7, dump_all_keybindings,"emacs-standard emacs-ctlx emacs-meta vi-insert vi-move vi-command");
  rl_callback_handler_install("Enter a line: ", &line_handler);

  while (read(STDIN_FILENO, &c, 1)) {
    rl_stuff_char(c);
    rl_callback_read_char();
  }
  return 0;
}
  
    
