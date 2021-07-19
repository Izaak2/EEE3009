#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX, INT_MIN
#include <stdlib.h>  // for strtol

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
       delay = conv;
       char c;
       for(;;)
         {
           c=getc(stdin);
           if (c == EOF) return 0;
           usleep(delay);
           printf("%c",c);
           fflush(stdout);
         }
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
