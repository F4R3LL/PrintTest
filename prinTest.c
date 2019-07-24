#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int	main()
{
  printf("\t\t-- LOGS  --\n\tMODEL.NO : YOUR MODEL\n\tCall Systems : socket(), connect(), write()\n\n");

  struct sockaddr_rc	addr = { 0 };
  int			s, status;
  char			err[256];

  // BD_ADDR
  char dest[18] = "00:00:00:00:00:00";	// BD_ADDR

  // allocate a socket
  if ((s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) != -1)
    printf("LOGS -> [socket] = %d\t| SUCESS\n", s);
  else
    perror("LOGS -> [socket] | FAILED : ");

  // set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = (uint8_t) 1; // Check your channel -> sdptool browse 00:00:00:00:00:00 | egrep "Service Name:|Channel:"
  str2ba( dest, &addr.rc_bdaddr );

  // connect to printer
    if (!(status = connect(s, (struct sockaddr *)&addr, sizeof(addr))))
          printf("LOGS -> [connect] = %d\t| SUCESS\n", s);
    else
      {
	sprintf(err, "LOGS -> [connect] = %d\t| FAILED ", status);
	perror(err);
      }

  // send a message
  if (!status)
    {
      char *str = "-- TEST --\n\n\n\n"; // Impression du message -- TEST --
      int i = 0;
      for (;str[i]; ++i);
      status = write(s, str, i);
      printf("LOGS -> [write] = %d\t| SUCESS\n", s);
    }
  else
    {
      sprintf(err, "LOGS -> [write] = %d\t| FAILED ", status);
      perror(err);
    }

  close(s);
  return 0;
}
