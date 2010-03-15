/* In order to test rlwrap's behaviour when a client crashes, we need a "crash
   test dummy" program. kaboom can crash in a few different ways. 'rlwrap kaboom'
   should:
      1. report the crash,
      2. exit with the same exit status as 'kaboom' and      
      3. point out that rlwrap is not to blame.
*/


#include "rlwrap.h"

static void myerror2(char *message);
static void set_ulimit2(int resource, long cur, long max);
static int call_thyself(int x);


int main () {
  int answer = 0;
  printf("Kaboom is rlwrap's personal crash test dummy.\n"
         "Enter a number to choose an unhappy end:\n"
         "1. segfault\n"
         "2. division by zero\n"
         "3. illegal instruction\n"
         "4. exceeded cpu time\n"
         "5. exceeded file size\n"
         "6. stack overflow\n"
         "7. abort\n");
  fscanf(stdin, "%d", &answer);

  switch (answer) {
  case 1:  {
    char *p = NULL;
    *(p+1) = 1; /* provoke SEGV */
    break;
  }     
  case 2: {
    int i,j;
    for (j=0, i=10; i >= 0; i--) {
      j += i/(i-1); /* circuitous way to get 1/0, without the compiler complaining or optimising away */
    }   
  }     
  case 3: {
    char *illegal = "\xff\xff\xff\xff";  /* should (hopefully) be an illegal instruction on most processors  */
    typedef void(*FUNCTION)(void); /* Always use typedefs when dealing with function pointers, to avoid sea-sickness */
    ((FUNCTION) illegal) (); 
    break; 
  }     
  case 4: {
    float z = 0.5;
    set_ulimit2(RLIMIT_CPU,1,10); /* limit max CPU time to 1 sec. */
    printf("Wait a second...\n"); fflush(stdout);
    while(1) {
      z = 1-z*z;               
    }
    break;
  }     
  case 5:  {
    FILE *fp;
    set_ulimit2(RLIMIT_FSIZE,10,10); /* limit file size to 10 bytes (doesn't work on many systems, granularity
                                     of file size limit may be some multiple of block size ?) */
    fp = fopen("/tmp/baz", "w");
    if (!fp)
      myerror2("Could not write to /tmp/baz");
    fprintf(fp,"aaaaaaaaaaaaaaaaaaaaaaaaaaargh!!!\n");
    break;
  }
  case 6: {
    call_thyself(0);
    break;
  }
  case 7: {
    abort();
    break;
  }     
  default: {
    printf("I don't know how to fail, so I'll just exit");
    return 0;
  }
  }
  printf ("I somehow survived....\n");
  return 0;
 
}


static void myerror2(char *message) {
  fprintf(stderr, "%s (%s)\n", message, (errno ? strerror(errno) : "oh well"));
  exit(0);
}       

static void set_ulimit2(int resource, long cur, long max) {
#ifdef HAVE_SETRLIMIT
  struct rlimit limit;
  int result;
  limit.rlim_cur = cur;
  limit.rlim_max = max;
  result = setrlimit(resource, &limit);
#else
  myerror2("On this system I don't know how to set resource limits, sorry!");
#endif
}

static int call_thyself(int x) {
  int y = call_thyself(x+1);
  return y - 1; /* do some work to avoid tail recursion optimisation (do C compilers ever do that)? */
}       
