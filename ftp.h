#ifndef FTP_H
#define FTP_H
#include "network.h"
#include <string.h>
#include <strings.h>

#define BUF_LEN 1024

extern char ser_ip[20];

int ftp_send(NetWork *ser_nw, char *buf);

int ftp_recv(NetWork *ser_nw, char *buf);

int ftp_download(NetWork *ser_nw, char *buf, char *name);

int ftp_upload(NetWork *ser_nw, char *buf, char *name);

#endif //FTP_H