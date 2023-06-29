#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void INThandler()
{
  exit(0);
}

int main()
{
  char devname[] = "/dev/input/event2";
  int device = open(devname, O_RDONLY);
  struct input_event ev;

  signal(SIGINT, INThandler);

  while (1)
  {
    read(device, &ev, sizeof(ev));
    if (ev.type == 1 && ev.value == 1)
    {
      if (30 == ev.code && 1 == ev.value)
      {
        // F1 Key pressed
        int status = system("rauc install http://192.168.1.20:8080/estalor-reterminal-debug-bundle.raucb");
      }
      else if (31 == ev.code && 1 == ev.value)
      {
        // F2 Key pressed
        printf("Key: %i State: %i\n", ev.code, ev.value);
      }
      else if (32 == ev.code && 1 == ev.value)
      {
        // F2 Key pressed
        printf("Key: %i State: %i\n", ev.code, ev.value);
      }
      else if (33 == ev.code && 1 == ev.value)
      {
        // Green Key pressed
        printf("Key: %i State: %i\n", ev.code, ev.value);
      }
      else if (142 == ev.code && 1 == ev.value)
      {
        // Suspend key pressed
        printf("Key: %i State: %i\n", ev.code, ev.value);
      }
      else
      {
        printf("Unknown key pressed.");
        printf("Key: %i State: %i\n", ev.code, ev.value);
      }
    }
  }
}