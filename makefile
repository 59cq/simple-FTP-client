all:
	gcc ftp_client.c network.c ftp.c -lpthread -o fftp


clean:
	rm fftp