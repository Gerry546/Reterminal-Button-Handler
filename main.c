#include <stdio.h>
#include <libevdev/libevdev.h>
#include <sys/stat.h>
#include <fcntl.h>

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
      fprintf(stderr, "Failed to init libevdev (%S)\n", strerror(-rc));
      goto out;
    }

    printf("Input device ID: bus %#x vendor %#x product %#x\n",
          libevdev_get_id_bustype(dev),
          libevdev_get_id_vendor(dev),
          libevdev_get_id_product(dev));
    printf("Evdev version: %x\n", libevdev_get_driver_version(dev));
    printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
    printf("Phys location: %s\n", libevdev_get_phys(dev));
    printf("Uniq identifier: %s\n", libevdev_get_uniq(dev));

    rc = 0;
  }

out:
  libevdev_free(dev);
  return rc;
}