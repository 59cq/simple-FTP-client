# simple-FTP-client
简易FTP客户端，模拟ftp命令：ls/cd/put/get/bye

## 环境
- `Linux`

## USAGE
- 终端下`make`编译
- 终端下`fftp yourftpip`连接服务器,然后跟随提示输入密码,密码不可见
- 登录成功后的命令:
    - `ls` 列出当前目录下的所有文件信息,无复加参数
    - `cd path` path可以是绝对路径/多级路径
    - `put filename` filename为本地路径下的文件的文件名,可以是绝对路径/多级路径
    - `get filename` filename为服务器当前路径下的文件的文件名,可以是绝对路径/多级路径
## 例
```
wcq@DESKTOP-PL6DIJT:~$ make
gcc ftp_client.c network.c ftp.c -lpthread -o fftp
ftp_client.c: In function ‘main’:
ftp_client.c:146:9: warning: implicit declaration of function ‘gets’ [-Wimplicit-function-declaration]
         gets(buf);
         ^~~~
/tmp/ccCxuINm.o: In function `main':
ftp_client.c:(.text+0x6d4): warning: the `gets' function is dangerous and should not be used.
wcq@DESKTOP-PL6DIJT:/mnt/c/Users/WChQ/OneDrive/博客/FTPclient/network$ ./fftp [yourftpip]
220 (vsFTPd 3.0.3)

NAME:>[yourusername]
331 Please specify the password.

>[yourpswd]
230 Login successful.

215 UNIX Type: L8

211-Features:
 EPRT
 EPSV
 MDTM
 PASV

 REST STREAM
 SIZE
 TVFS
211 End

257 "/" is the current directory

250 Directory successfully changed.

257 "/" is the current directory

227 Entering Passive Mode (***,***,***,***,85,217).

150 Here comes the directory listing.

drwxr-xr-x   22 0        0            4096 Jul 10 19:52 .
drwxr-xr-x   22 0        0            4096 Jul 10 19:52 ..
drwxr-xr-x    2 0        0            4096 Jul 10 20:53 bin
drwxr-xr-x    3 0        0            4096 Jul 10 20:54 boot
drwxr-xr-x   18 0        0            3720 Aug 12 12:04 dev
drwxr-xr-x   91 0        0            4096 Aug 12 14:33 etc
drwxr-xr-x    5 0        0            4096 Aug 12 12:02 home
lrwxrwxrwx    1 0        0              33 Jun 20 16:25 initrd.img -> boot/initrd.img-4.4.0-151-generic
lrwxrwxrwx    1 0        0              33 Jun 20 16:25 initrd.img.old -> boot/initrd.img-4.4.0-131-generic
drwxr-xr-x   20 0        0            4096 Jun 20 16:25 lib
drwxr-xr-x    2 0        0            4096 Jun 20 16:25 lib64
drwx------    2 0        0           16384 Jun 20 16:18 lost+found
drwxr-xr-x    4 0        0            4096 Jun 20 16:18 media
drwxr-xr-x    2 0        0            4096 Jul 31  2018 mnt
drwxr-xr-x    2 0        0            4096 Jul 31  2018 opt
dr-xr-xr-x   98 0        0               0 Aug 12 12:04 proc
drwx------    7 0        0            4096 Aug 12 14:33 root
drwxr-xr-x   22 0        0             720 Aug 13 06:25 run
drwxr-xr-x    2 0        0           12288 Jul 10 20:53 sbin
drwxr-xr-x    3 0        0            4096 Aug 12 11:02 srv
dr-xr-xr-x   13 0        0               0 Aug 12 20:04 sys
drwxrwxrwt    7 0        0            4096 Aug 13 19:55 tmp
drwxr-xr-x   10 0        0            4096 Jun 20 16:18 usr
drwxr-xr-x   11 0        0            4096 Jun 20 16:18 var
lrwxrwxrwx    1 0        0              30 Jun 20 16:25 vmlinuz -> boot/vmlinuz-4.4.0-151-generic
lrwxrwxrwx    1 0        0              30 Jun 20 16:25 vmlinuz.old -> boot/vmlinuz-4.4.0-131-generic
226 Directory send OK.

------------------------------
ftp >cd home/wcq
250 Directory successfully changed.

257 "/home/wcq" is the current directory

227 Entering Passive Mode (***,***,***,***,84,221).

150 Here comes the directory listing.

drwxr-xr-x    2 1001     1001         4096 Aug 13 18:38 .
drwxr-xr-x    5 0        0            4096 Aug 12 12:02 ..
-rw-------    1 1001     1001           30 Aug 12 15:03 .bash_history
-rw-r--r--    1 1001     1001          220 Aug 12 12:02 .bash_logout
-rw-r--r--    1 1001     1001         3771 Aug 12 12:02 .bashrc
-rw-r--r--    1 1001     1001          655 Aug 12 12:02 .profile
-rwxrwxrwx    1 1001     1001         3820 Aug 13 19:37 haha.c
-rwxrwxrwx    1 1001     1001            5 Aug 07 01:58 tempCodeRunnerFile.c
226 Directory send OK.

------------------------------
ftp >get haha.c
200 Switching to Binary mode.

213 3820

213 20190813193736

227 Entering Passive Mode (***,***,***,***,81,144).

150 Opening BINARY mode data connection for haha.c (3820 bytes).

226 Transfer complete.

------------------------------
ftp >bye
221 Goodbye.

```
