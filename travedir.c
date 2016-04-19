#include "travedir.h"
int trave_dir(char* path,char** filename)
{
    DIR *d; //声明一个句柄
    struct dirent *file; //readdir函数的返回值就存放在这个结构体中
    struct stat sb;
    int i =0;
    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\n",path);
        return -1;
    }
    while((file = readdir(d)) != NULL)
    {
        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
        if(file->d_name[strlen(file->d_name)-1] == 't'){
                filename[i] = malloc( (strlen(path)+strlen(file->d_name) + 2 )*sizeof(path[0]) );
                strcpy(filename[i],path);
                strcat(filename[i],file->d_name);
                i++;
        }
    }
    closedir(d);
    printf("tarve complete\n");
    return i;
}
