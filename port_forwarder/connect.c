#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int do_connect(char *local_ip,char *hostname, short port){

	struct sockaddr_in addr,local_addr;
	struct hostent *host;
	int sock;
	
	if(!(host=gethostbyname(hostname))){
		herror(hostname);
		return(0);
	}

	addr.sin_family=AF_INET;/* inicializirame adresa */
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=((struct in_addr *)host->h_addr)->s_addr;

	
/* bindvame client socketa - tova e tainata =) */

	local_addr.sin_family=AF_INET;
	local_addr.sin_port=htons(0);// ura ura
	local_addr.sin_addr.s_addr=inet_addr(local_ip);

	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1){
		perror("cannot create socket");
		exit(1);
	}

	if(bind(sock,(struct sockaddr *)&local_addr,sizeof(local_addr))){
		perror(inet_ntoa(local_addr.sin_addr));
		exit(1);
	}

	if(connect(sock,(struct sockaddr *)&addr,sizeof(addr))){
		perror("connect");
		exit(1);
	}

	return sock;
}
