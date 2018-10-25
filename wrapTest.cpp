#include <arpa/inet.h>
#include <fstream>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <iostream>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

// ETCDCTL commands I need:
//                         get
//                         put
//                         del
//                         watch
#define CMD_IDX (1)
#define KEY_IDX (2)
#define VAL_IDX (3)
int main (int argc, char **argv)
{

    bool putFlag=false;
    char cmd[1024];

    if ( argc < 3 )
    {
       printf("We arguments you only gave %d\n", argc);
       return -1;
    }

    for ( int ii=0; ii < argc; ii++)
    {
       printf("ARG %d %s\n", ii, argv[ii]);
    }

    // Look for a "put" command.  That means we need an argument
    if ( memcmp(argv[CMD_IDX], "put", 3) == 0 )
    {
       if ( argc != 4 )
       {
          printf("This is a put, we need a single value\n");
          return -2;
       }
       putFlag = true;
    }

    sprintf(cmd, "etcdctl --endpoints=\"http://127.0.0.1:2379\" %s %s", argv[CMD_IDX], argv[KEY_IDX]);
    if ( putFlag )
    {
        sprintf(cmd, "%s \"%s\"", cmd, argv[VAL_IDX]);
    }
    printf("COMMAND:  %s\n", cmd);

    char in[4096];
    FILE *fp = popen(cmd, "r");
    if (fp != NULL)
    {   
        int kt = 10; 
        while ( (fgets(in, 1024, fp) != NULL) && kt )
        {
            kt--;
            printf("%s", in);
        }
    }   
}
