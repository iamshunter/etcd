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


#include "yaml.h"

using namespace YAML;
using namespace std;

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
    char cmd[4096];

    if ( argc < 3 )
    {
       printf("We need arguments you only gave %d\n", argc);
       printf("EX: %s CMD KEY <val>\n", argv[0]);
       return -1;
    }

    char bigBuf[4096]={0};
    int  nn;
    FILE *fptr = fopen("../yaml/config.yaml", "r");
    nn=fread(bigBuf, 1, sizeof(bigBuf), fptr);
    printf("Read %d bytes\n", nn);
    fclose(fptr);

    for ( int ii=0; ii < argc; ii++)
    {
       printf("ARG %d %s\n", ii, argv[ii]);
    }

    // Look for a "put" command.  That means we need an argument
    if ( memcmp(argv[CMD_IDX], "put", 3) == 0 )
    {
       putFlag = true;
    }

    sprintf(cmd, "etcdctl --endpoints=\"http://127.0.0.1:2379\" %s %s", argv[CMD_IDX], argv[KEY_IDX]);
    if ( putFlag )
    {
        sprintf(cmd, "%s \"%s\"", cmd, bigBuf);
    }
    printf("COMMAND:  %s\n", cmd);

    char in[4096];
    FILE *fp = popen(cmd, "r");
    if (fp != NULL)
    {   
        int kt = 10; 
        int nn=fread(in, 1, sizeof(in), fp);
        printf("[%d bytes]\n", nn);

        // Look for the key
        if ( strncmp(in, argv[KEY_IDX], strlen(argv[KEY_IDX])) == 0 )
        {
            printf("Found the key %s %d\n", argv[KEY_IDX], strlen(argv[KEY_IDX]) );
        }
        else
        {
            printf("NO key %s %s %d\n", argv[KEY_IDX], in, strlen(argv[KEY_IDX]) );
            return -1;
        }

        // Look for a '\n'

        printf("Decode the node ... ");

        // The response from etcdctl is the key on the 1st line, followed
        // by the value on the next line.
        // Get a pointer the the string just after the key and its '\n'.
        char *cPtr = &in[strlen(argv[KEY_IDX])+1];
        YAML::Node node = YAML::Load(cPtr);
        printf("done\n");
        Node cmdl = node["CMDL"];
        Node vort = node["Vortex"];
        Node mt2  = node["T2"];
        printf("Type %x\n", node.Type());

        YAML::Node tempNode;
        for ( int arg=1; arg<4; arg++ )
        {
           string str;
           switch(arg)
           {
              case 1:
                  tempNode = node["CMDL"];
                  cout << "Node name CMDL size " << tempNode.size() << " " << endl;
                  break;
              case 2:
                  tempNode = node["Vortex"];
                  cout << "Node name Vortex size " << tempNode.size() << " " << endl;
                  break;
              case 3:
                  tempNode = node["T2"];
                  cout << "Node name T2 size " << tempNode.size() << " " << endl;
                  break;
           }

           cout << "Freq -- : int " << tempNode["freq"].as<int>()    << endl;
           cout << "band -- : str " << tempNode["band"].as<string>() << endl;
        }

    }
}
