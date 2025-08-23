//
// Created by VC365 on 8/22/25.
//
#ifndef TEMP_H
#define TEMP_H
#include <signal.h>

char temp_dir[50];

static void temp_cleanup() {
    char cmd[300];
    snprintf(cmd, sizeof(cmd), "rm -rf %s", temp_dir);
    system(cmd);
}

static void temp_kill(int sig)
{
    printf("What?");
    temp_cleanup();
    _exit(1);
}

int temp_root()
{
    strcpy(temp_dir, "/tmp/grub-XXXXXX");
    if (mkdtemp(temp_dir) == NULL) {
        perror("mkdtemp failed");
        exit(1);
    }
    //  signal
    signal(SIGINT, temp_kill);   // Ctrl+C
    signal(SIGTERM,  temp_kill);  // kill
    signal(SIGHUP,  temp_kill);   // logout

    atexit(temp_cleanup);
    return 1;
}

int copy(const char* this, const char* to, const char* check)
{
    char command[212];
    snprintf(command, sizeof(command), "cp -r \"%s\" \"%s\"", this, to);

    const int result = system(command);

    if (result != 0 || access(check, F_OK) == -1) {
        printf("can not copy %s to %s",this,to);
        return 0;
    }
    return 1;
}

int run(const char* exec,const char* argument, const char* optional_arg, const char* msg_error)
{
    char command[212];
    snprintf(command, sizeof(command), "%s %s %s 2>/dev/null", exec,argument,optional_arg);
    const int result = system(command);

    if (result != 0) {
        printf("%s\n",msg_error);
        return 0;
    }
    return 1;
}

#endif //TEMP_H
