#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX, INT_MIN
#include <stdlib.h>  // for strtol

// decide what type ACM to use, look in the handout for pseudo-code
#define slot_size 23 // 22 is the max size for game and one more is for the 0 symbol

char slots[3][slot_size]; // 3 slots; add the size of each slot instead of ...
int w=0, r=1, l=1; // define and initialise the variables as in the handout
int global_delay = 0;

void reader_thread ()
{
  while(1)  // reader loop; includes a delay to reduce the output data rate
    {
      if(l != r)
      {
        r = l;
        printf ("%s", slots[r]); // access slot; slots[i] is a pointer to slots[i][0]
                                  // (slots[i][0],slots[i][1],... is a 0-terminated string)
        fflush (stdout);
      }
      usleep(global_delay);     // limit output rate; calculate your own value (>500000)
    }
}

char inp() // getchar() wrapper which checks for End Of File EOF
{
  char c;
  c=getchar();
  if(c==EOF) exit(0); // exit the whole process if input ends
  return c;
}


int main( int argc, char *argv[] )  {


   if( argc == 2 ) {
   char *p;
   int delay;

   errno = 0;
   long conv = strtol(argv[1], &p, 10);

   // Check for errors: e.g., the string does not represent an integer
   // or the integer is larger than int
   if (errno != 0 || *p != '\0' || conv > INT_MAX || conv < INT_MIN) {
       printf("No an integer");
       return 0;
   } else {
       global_delay = conv;
       //...; // variable declarations/initialisation, if needed
       pthread_t thread_id;
       //...; // create reader thread
       pthread_create (&thread_id, NULL,(void*) &reader_thread, NULL);

       int lu[3][3] = {{2, 2, 1}, {2, 0, 0}, {1, 0, 0}};
       int j;

       while (1)  // writer loop
         {

           // access slot; modify this according to the output format of your game
           // in this example I keep reading until '*' appears,
           // then the symbol 0 is added, which is the sting terminator.
           // The terminator is needed for printf("%s",...) of the reader.
           //usleep(10000000);
           j = 0;
           //usleep(5000000);
           while ((slots[w][j++] = inp ()) != '*'); // the actual computation takes place inside the condition
           slots[w][j] = 0; // append the terminating symbol to the string

           l = w;

           w = lu[l][r];

         }
       pthread_cancel ((void*) &thread_id);
       return 0;
   }
   }
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
   }
   else {
      printf("One argument expected.\n");
   }
}
