//
// Created by VC365 on 8/22/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "src/temp.h"
#include <sys/stat.h>
#include <getopt.h>

//# variables
char grub_dir[20];
char grub_dirX[50];
char grub_cfgX[50];
char monitor_res[50];
char iso[50];
char dir[50];
char boot_dir[50];
char theme_dir[156];
char themeX_dir[100];
const char* version="0.1.1";

//argument variables
char arg_static[256];
char* q_arg_optional="";
int use_theme=1;
char* kvm="";
char* fullS="--full-screen";
char* custom_grub_dir="";

int arguments(const int argc, char* argv[],int a)
{
    int opt;
    const struct option options[] = {
        {"qemu", required_argument, NULL, 'q'},
        {"enable-kvm", no_argument, NULL, 'L'},
        {"disable-theme", no_argument, NULL, 'T'},
        {"custom-grub", required_argument, NULL, 'c'},
        {"disable-full-screen", no_argument, NULL, 'X'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "q:hLvTcX", options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'q':
            if (!optarg)
            {
                fprintf(stderr, "Error: Missing value for --qemu.\n");
                exit(1);
            }
            q_arg_optional=optarg;
            break;
        case 'L':
            kvm="--enable-kvm";
            break;
        case 'c':
            if (access(optarg,F_OK)==-1)
            {
                fprintf(stderr, "%s: No grub directory.\n",optarg);
                exit(1);
            }
            custom_grub_dir=optarg;
            break;
        case 'T':
            use_theme=0;
            break;
        case 'X':
            fullS="";
            break;
        case 'h':
            printf("Usage: %s [--size <icon_size>] [--utime <milliseconds>] [-h] [-v] [-f] [-n]\n", argv[0]);
            printf("Options:\n");
            printf("  -q, --qemu \"Argument\"           Use custom argument for QEMU.\n");
            printf("  -c, --custom-grub \"Directory\"   Use custom grub.\n");
            printf("  --enable-kvm                      Enable KVM for QEMU.\n");
            printf("  --disable-full-screen             Disable full screen mode.\n");
            printf("  --disable-theme                   Disable theme grub.\n\n");
            printf("  -h, --help                        Show this help message.\n");
            printf("  -v, --version                     Show version.\n");
            printf("                 Credit : VC365 (https://github.com/VC365) \n\n");
            exit(0);
        case 'v':
            printf("grub-qemu %s",version);
            exit(0);
        default:
            fprintf(stderr, "Invalid argument. Use -h or --help for usage info.\n");
            exit(1);
        }
    }
    return 1;
}

int initialization(const int a)
{
    //  Grub DIR
    if (access("/boot/grub", F_OK) != -1)
    {
        snprintf(grub_dir, sizeof(grub_dir), "%s", "/boot/grub");
    }
    else if (access("/boot/grub2", F_OK) != -1)
    {
        snprintf(grub_dir, sizeof(grub_dir), "%s", "/boot/grub2");
    }
    else
    {
        printf("grub folder not found");
        _exit(1);
    }
    //  End--------------

    //  Resolution
    FILE* resolution = popen("xrandr | grep '*' | awk '{print $1}'", "r");
    if (resolution == NULL)
    {
        resolution = popen("wlr-randr | grep '*' | awk '{print $1}'", "r");
    }
    if (resolution != NULL)
    {
        if (fgets(monitor_res, sizeof(monitor_res), resolution) == NULL) {
            fprintf(stderr, "Error: Failed to read resolution\n");
            exit(1);
        }
        monitor_res[strcspn(monitor_res, "\n")] = 0;
        if (strcmp(monitor_res, "1366x768") == 0) { snprintf(monitor_res, sizeof(monitor_res), "1360x768"); }
    }else {
        fprintf(stderr, "Error: Resolution command failed\n");
        exit(1);
    }
    fclose(resolution);
    //  End--------------

    //  Get Theme DIR
    char command[256];
    snprintf(command, sizeof(command),
             "dirname \"$(grep \"set theme=\" \"%s/grub.cfg\" 2>/dev/null | cut -d'=' -f2 | tr -d '\"' | sed 's|(\\$root)||')\"",
             grub_dir);
    FILE* get_theme_dir = popen(command, "r");
    if (get_theme_dir != NULL)
    {
        fgets(theme_dir, sizeof(theme_dir), get_theme_dir);
        theme_dir[strcspn(theme_dir, "\n")] = 0;
        if (strcmp(theme_dir, ".") == 0) { snprintf(theme_dir, sizeof(theme_dir), "notfound");use_theme=0; }
    }
    fclose(get_theme_dir);
    //  End--------------

    snprintf(iso, sizeof(iso), "%s/iso", temp_dir);
    snprintf(dir, sizeof(dir), "%s/VC", temp_dir);

    snprintf(boot_dir, sizeof(boot_dir), "%s/boot", dir);
    snprintf(grub_dirX, sizeof(grub_dirX), "%s%s", dir, grub_dir);
    snprintf(grub_cfgX, sizeof(grub_cfgX), "%s/grub.cfg", grub_dirX);
    snprintf(themeX_dir, sizeof(themeX_dir), "%s/themeX", grub_dirX);
    return 1;
}

int create_folders(const int a)
{
    mkdir(iso, 0777);
    mkdir(dir, 0777);
    mkdir(boot_dir, 0777);
    if (!copy(grub_dir, boot_dir, grub_dirX))
    {
        printf("Error: Unable to copy grub directory!!");
        printf("Try sudo\n");
        exit(0);
    }
    if (use_theme)
    {
        if (!copy(theme_dir, themeX_dir, themeX_dir))
        {
            printf("Error: Unable to copy theme directory!\n");
        }
    }
    printf("Folders created.\n");
    return 1;
}

int create_iso(const int a)
{
    snprintf(arg_static, sizeof(arg_static), "-i -E \"s|^[[:space:]]*set[[:space:]]+gfxmode=.*|set gfxmode=%s|\"",
         monitor_res);
    if (use_theme)
    {
        if (!run("sed", "-i -E \"s|^[[:space:]]*set[[:space:]]+theme=.*|set theme=\\$prefix/themeX/theme.txt|\"", grub_cfgX,
                 "can not edit"))
        {
            printf("Error: Failed to apply theme settings to %s\n",grub_cfgX);
        }

        if (!run("sed", "-i -E \"s|^[[:space:]]*loadfont[[:space:]]+.*/([^/]+\\.pf2)|loadfont \\$prefix/themeX/\\1|\"",
                 grub_cfgX, "can not edit"))
        {
            printf("Error: Failed to apply theme settings to %s\n",grub_cfgX);
        }

    }
    if (!run("sed", arg_static, grub_cfgX, "can not create iso!"))
    {
        printf("Error: Failed to apply resolution settings to %s\n",grub_cfgX);
    }

    snprintf(arg_static, sizeof(arg_static), "-o \"%s/VC365.iso\" \"%s\"", iso, dir);
    if (run("grub-mkrescue", arg_static, "", "can not create iso!"))
    {
        printf("iso created!!\n");
        return 1;
    }
    return 0;
}

void qemu_run(const int a)
{
    if (a)
    {
        printf("Running QEMU ...\n");
        snprintf(arg_static, sizeof(arg_static),"-drive file=\"%s/VC365.iso\",media=cdrom -bios /usr/share/ovmf/x64/OVMF.4m.fd -display gtk -device virtio-vga %s %s" , iso,fullS,kvm);
        run("qemu-system-x86_64",arg_static,q_arg_optional,"can not run qemu");
    }
}

int main(const int argc, char** argv)
{
    qemu_run(create_iso(create_folders(arguments(argc, argv,initialization(temp_root())))));
    return 0;
}
