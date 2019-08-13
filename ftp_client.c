#include "network.h"
#include "ftp.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "getch.h"

int cmd_ls(NetWork *ser_nw);
int cmd_get(NetWork *ser_nw, char *_buf);
int cmd_cd(NetWork *ser_nw, char *_buf);
int cmd_put(NetWork *ser_nw, char *_buf);
int cmd_bye(NetWork *ser_nw);

char ser_ip[20];

void *list_recv(void *arg)
{
    char buf[1024];
    strcpy(buf, (char *)arg);
    __u_char ip1, ip2, ip3, ip4;
    uint16_t p1, p2;
    sscanf(strchr(buf, '(') + 1, "%hhu,%hhu,%hhu,%hhu,%hu,%hu).", &ip1, &ip2, &ip3, &ip4, &p1, &p2);
    char ip[20];
    sprintf(ip, "%hhu.%hhu.%hhu.%hhu\n", ip1, ip2, ip3, ip4);
    uint16_t pot = p1 * 256 + p2;
    //printf("%hu\n",pot);
    //printf("--%s--",ip);
    NetWork *list_nw = open_network('c', SOCK_STREAM, ser_ip, pot);
    int ret = 0;
    while (1)
    {
        ret = nrecv(list_nw, buf, BUF_LEN);
        if (ret < sizeof(buf))
        {
            buf[ret] = '\0';
            printf("%s", buf);
            break;
        }
        printf("%s", buf);
        bzero(buf, sizeof(buf));
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    strcpy(ser_ip, argv[1]);
    char buf[1024];
    NetWork *ser_nw = open_network('c', SOCK_STREAM, ser_ip, 21);
    //printf("%d,%d,%u,%p",ser_nw->fd,ser_nw->type,ser_nw->len,ser_nw->addr);
    if (ser_nw == NULL)
    {
        return -1;
    }

    ftp_recv(ser_nw, buf);

    char username[20];
    printf("NAME:>");
    scanf("%s", username);
    bzero(buf, BUF_LEN);
    sprintf(buf, "USER %s", username);
    buf[strlen(buf)] = '\n';
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    int i = 0;
    int ch;
    char pswd[BUF_LEN];
    bzero(pswd, BUF_LEN);
    printf(">");
    stdin->_IO_read_end = stdin->_IO_read_ptr;
    while (1)
    {
        ch = getch();
        if (ch == 10)
        {
            break;
        }
        pswd[i] = (char)ch;
        i++;
    }
    printf("\n");
    bzero(buf, BUF_LEN);
    sprintf(buf, "PASS %s", pswd);
    buf[strlen(buf)] = '\n';
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    strcpy(buf, "SYST\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    strcpy(buf, "FEAT\n\0");
    ftp_send(ser_nw, buf);

    while (1)
    {
        ftp_recv(ser_nw, buf);
        if (NULL != strstr(buf, "211 End"))
        {
            break;
        }
    }

    strcpy(buf, "PWD\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    strcpy(buf, "CWD /\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    strcpy(buf, "PWD\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    strcpy(buf, "PASV\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    pthread_t pid_list;
    pthread_create(&pid_list, NULL, list_recv, buf);

    usleep(10);

    strcpy(buf, "LIST -al\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    pthread_join(pid_list, NULL);

    ftp_recv(ser_nw, buf);

    while (1)
    {
        bzero(buf, sizeof(buf));
        printf("------------------------------\nftp >");
        gets(buf);
        if (NULL != strstr(buf, "ls"))
        {
            cmd_ls(ser_nw);
        }
        else if (NULL != strstr(buf, "cd"))
        {
            cmd_cd(ser_nw, buf);
        }
        else if (NULL != strstr(buf, "put"))
        {
            cmd_put(ser_nw, buf);
        }
        else if (NULL != strstr(buf, "get"))
        {
            cmd_get(ser_nw, buf);
        }
        else if (NULL != strstr(buf, "bye"))
        {
            cmd_bye(ser_nw);
            return 0;
        }
        else
        {
            printf("unknow cmd\n");
        }
    }
}

int cmd_ls(NetWork *ser_nw)
{

    char buf[BUF_LEN];
    strcpy(buf, "PASV\n");
    nsend(ser_nw, buf, strlen(buf));
    bzero(buf, BUF_LEN);

    ftp_recv(ser_nw, buf);

    pthread_t pid_list;

    pthread_create(&pid_list, NULL, list_recv, buf);

    usleep(10);

    strcpy(buf, "LIST -al\n");
    nsend(ser_nw, buf, strlen(buf));
    bzero(buf, BUF_LEN);

    ftp_recv(ser_nw, buf);

    pthread_join(pid_list, NULL);

    ftp_recv(ser_nw, buf);
}

int cmd_cd(NetWork *ser_nw, char *_buf)
{
    char path[BUF_LEN];
    char buf[BUF_LEN];
    strcpy(path, strstr(_buf, "cd") + 3);

    bzero(buf, BUF_LEN);
    sprintf(buf, "CWD %s", path);
    buf[strlen(buf)] = '\n';
    //puts(buf);
    nsend(ser_nw, buf, strlen(buf));
    bzero(buf, BUF_LEN);

    ftp_recv(ser_nw, buf);

    strcpy(buf, "PWD\n");
    nsend(ser_nw, buf, strlen(buf));
    bzero(buf, BUF_LEN);

    ftp_recv(ser_nw, buf);

    cmd_ls(ser_nw);
}

int cmd_put(NetWork *ser_nw, char *_buf)
{
    char name[BUF_LEN];
    char buf[BUF_LEN];
    strcpy(name, strstr(_buf, "put") + 4);

    strcpy(buf, "TYPE I\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    bzero(buf, BUF_LEN);
    sprintf(buf, "SIZE %s", name);
    buf[strlen(buf)] = '\n';
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    strcpy(buf, "PASV\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    ftp_upload(ser_nw, buf, name);
}

int cmd_get(NetWork *ser_nw, char *_buf)
{
    char name[BUF_LEN];
    char buf[BUF_LEN];
    strcpy(name, strstr(_buf, "get") + 4);

    strcpy(buf, "TYPE I\n\0");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    bzero(buf, BUF_LEN);
    sprintf(buf, "SIZE %s", name);
    buf[strlen(buf)] = '\n';
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    bzero(buf, BUF_LEN);
    sprintf(buf, "MDTM %s", name);
    buf[strlen(buf)] = '\n';

    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    strcpy(buf, "PASV\n");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    ftp_download(ser_nw, buf, name);
}

int cmd_bye(NetWork *ser_nw)
{
    char buf[BUF_LEN];
    bzero(buf, BUF_LEN);
    strcpy(buf, "QUIT\n");
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);
}