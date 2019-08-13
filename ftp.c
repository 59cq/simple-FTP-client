#include "ftp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int ftp_send(NetWork *ser_nw, char *buf)
{
    int ret = nsend(ser_nw, buf, strlen(buf));
    return ret;
}

int ftp_recv(NetWork *ser_nw, char *buf)
{
    bzero(buf, BUF_LEN);
    int ret = nrecv(ser_nw, buf, BUF_LEN);
    puts(buf);
    return ret;
}

int ftp_download(NetWork *ser_nw, char *buf, char *name)
{
    __u_char ip1, ip2, ip3, ip4;
    uint16_t p1, p2;
    sscanf(strchr(buf, '(') + 1, "%hhu,%hhu,%hhu,%hhu,%hu,%hu).", &ip1, &ip2, &ip3, &ip4, &p1, &p2);
    NetWork *data_nw = open_network('c', SOCK_STREAM, ser_ip, p1 * 256 + p2);

    bzero(buf, BUF_LEN);
    sprintf(buf, "RETR %s", name);
    buf[strlen(buf)] = '\n';
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);

    int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (0 > fd)
    {
        perror("open");
        close(data_nw->fd);
        return -1;
    }

    int ret;
    while (ret = nrecv(data_nw, buf, BUF_LEN))
    {
        write(fd, buf, ret);
    }
    close(fd);
    close(data_nw->fd);
    ftp_recv(ser_nw, buf);
}

int ftp_upload(NetWork *ser_nw, char *buf, char *name)
{

    __u_char ip1, ip2, ip3, ip4;
    uint16_t p1, p2;
    sscanf(strchr(buf, '(') + 1, "%hhu,%hhu,%hhu,%hhu,%hu,%hu).", &ip1, &ip2, &ip3, &ip4, &p1, &p2);
    NetWork *data_nw = open_network('c', SOCK_STREAM, ser_ip, p1 * 256 + p2);
    sprintf(buf, "STOR %s", name);
    buf[strlen(buf)] = '\n';
    ftp_send(ser_nw, buf);
    ftp_recv(ser_nw, buf);
    if (0 == strncmp(buf, "553", 3))
    {
        return -1;
    }
    bzero(buf, BUF_LEN);

    int fd = open(name, O_RDONLY);
    if (0 > fd)
    {
        perror("open");
        return -1;
    }
    int ret;
    while (ret = read(fd, buf, BUF_LEN))
    {
        nsend(data_nw, buf, ret);
    }

    // struct stat st;
    // fstat(fd,&st);

    // struct tm* t;
    // t = localtime(&st.st_mtime);

    // char time[30];
    // sprintf(time,"%d%d%d%d%d%d ",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    // strcpy(buf,"MDTM ");
    // strncat(buf,time,strlen(time));
    // strncat(buf,name,strlen(name));
    // strcat(buf,"\n");

    //nsend(ser_nw,buf,strlen(buf));
    close(fd);
    close(data_nw->fd);

    ftp_recv(ser_nw, buf);
}