#include <stdio.h>
#include <libevdev/libevdev.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static int handle_event(struct input_event *ev)
{
	if (ev->type != EV_SYN)
		printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %d\n",
			ev->input_event_sec,
			ev->input_event_usec,
			ev->type,
			libevdev_event_type_get_name(ev->type),
			ev->code,
			libevdev_event_code_get_name(ev->type, ev->code),
			ev->value);

    if (30 == ev->code && 1 == ev->value)
    {
      // F1 Key pressed
      int status = system("rauc install http://192.168.1.19:8888/reterminal-bundle.raucb");
      printf("Return status: %i", status);
      if (status != -1)
      {
        system("reboot");
      }
    }
    else if (31 == ev->code && 1 == ev->value)
    {
      // F2 Key pressed
      printf("Key: %i State: %i\n", ev->code, ev->value);
    }
    else if (32 == ev->code && 1 == ev->value)
    {
      // F3 Key pressed
      printf("Key: %i State: %i\n", ev->code, ev->value);
    }
    else if (33 == ev->code && 1 == ev->value)
    {
      // Green Key pressed
      printf("Key: %i State: %i\n", ev->code, ev->value);
    }
    else if (142 == ev->code && 1 == ev->value)
    {
      // Suspend key pressed
      printf("Key: %i State: %i\n", ev->code, ev->value);
    }
    else
    {
      printf("Unknown key pressed.");
      printf("Key: %i State: %i\n", ev->code, ev->value);
    }
	return 0;
}

int main()
{
  struct libevdev *dev = NULL;
  const char *file;
  int fd;
  int rc = 1;
  char *files[] = {
      "/dev/input/event0",
      "/dev/input/event1",
      "/dev/input/event2",
      "/dev/input/event3",
      "/dev/input/event4",
      "/dev/input/event5",
      "/dev/input/event6",
  };

  // Iterate over the possibility to find the correct event device
  for (int i = 0; i < 5; ++i)
  {
    // Read the input
    file = files[i];
    fd = open(file, O_RDONLY);
    if (fd < 0)
    {
      // File cannot be opened, exit...
      perror("Failed to open device");
      goto out;
    }

    rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0)
    {
      fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
      goto out;
    }

    if (!strcmp("gpio_keys", libevdev_get_name(dev)))
    {
      printf("Found the correct device");
      printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
      break;
    }

  }

  do {
		struct input_event ev;
		rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL|LIBEVDEV_READ_FLAG_BLOCKING, &ev);
		if (rc == LIBEVDEV_READ_STATUS_SUCCESS)
			handle_event(&ev);
	} while (rc == LIBEVDEV_READ_STATUS_SYNC || rc == LIBEVDEV_READ_STATUS_SUCCESS || rc == -EAGAIN);

	if (rc != LIBEVDEV_READ_STATUS_SUCCESS && rc != -EAGAIN)
		fprintf(stderr, "Failed to handle events: %s\n", strerror(-rc));
  rc = 0;

out:
  libevdev_free(dev);
  return rc;
}