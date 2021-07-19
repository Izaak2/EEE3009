#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define width  48
#define height 18
#define buf_length (width*height)
#define fox_init_x (width/3*2)
#define fox_init_y (height/3*2)
#define fox_init_dir 'u'
#define rabbit_init_x (width/3)
#define rabbit_init_y (height/3)
#define rabbit_init_dir 'd'

//cd Desktop/Uni\ Stage\ 3/EEE3009/

//---- set keyboard mode -- please copy and use this function
struct termios
tty_prepare ()
{
  struct termios tty_attr_old, tty_attr;
  tcgetattr (0, &tty_attr);
  tty_attr_old = tty_attr;
  tty_attr.c_lflag &= ~(ECHO | ICANON);
  tty_attr.c_cc[VMIN] = 1;
  tcsetattr (0, TCSAFLUSH, &tty_attr);
  return tty_attr_old;
}

//---- restore keyboard mode -- please copy and use this function
void
tty_restore (struct termios tty_attr)
{
  tcsetattr (0, TCSAFLUSH, &tty_attr);
}

//---- fox direction
char fox_dir = fox_init_dir;
char rabbit_dir = rabbit_init_dir;

void
set_fox_dir( char dir)
{
  switch(dir)
  {
      case 'A':
          fox_dir = 'u';
          break;
      case 'B':
          fox_dir = 'd';
          break;
      case 'C':
          fox_dir = 'r';
          break;
      case 'D':
          fox_dir = 'l';
          break;
  }
}
//---- keyboard thread function
void
keys_thread ()
{
  while(1)
  {
    if(getchar() == '\033')
    {
      getchar();
      set_fox_dir(getchar());
    }
  }
}



//---- update x and y coord-s according to direction; used in main()
void
update_coord (int *x_ptr, int *y_ptr, char dir) // call by reference to x and y
{
  switch (dir)
    {
    case 'u':
      if(*y_ptr > 1)
      {
        (*y_ptr)--;
        break;
      }else
      {
      *y_ptr = height - 1;
      }
    case 'd':
     if(*y_ptr < height)
     {
       (*y_ptr)++;
       break;
     }else
     {
       *y_ptr = 1;
     }
    case 'l':
      if(*x_ptr > 1)
      {
        (*x_ptr)--;
        break;
      }else
      {
        *x_ptr = width - 1;
      }
    case 'r':
      if(*x_ptr < width)
      {
        (*x_ptr)++;
        break;
      }else
      {
        *x_ptr = 1;
      }
    }
}

//---- the program starts its execution from here
int main ()
{
  pthread_t thread_id;
  int rabbit_x = rabbit_init_x;
  int rabbit_y = rabbit_init_y;
  int fox_x = fox_init_x;
  int fox_y = fox_init_y;
  struct termios term_back = tty_prepare ();

  pthread_create (&thread_id, NULL,(void*) &keys_thread, NULL);

  while (1)
    {
      usleep(500000);

      update_coord (&fox_x, &fox_y, fox_dir);

      if(random()%2) rabbit_dir = "udlr"[random() % 4];

      update_coord (&rabbit_x, &rabbit_y, rabbit_dir);

      printf ("\033[2J\033[%d;%dH@\033[%d;%dH*", fox_y, fox_x, rabbit_y, rabbit_x);

      fflush (stdout);

      if(rabbit_y == fox_y && rabbit_x == fox_x)
      {
          break;
      }
    }

  exit(0);
  pthread_cancel((void*) &keys_thread);
  tty_restore (term_back);
  return 0;
}
