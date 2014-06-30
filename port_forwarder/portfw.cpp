#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mad/mad.h>
#include <mad/prop.h>


#define PROGNAME			"portfw"

#define DEFAULT_CONFIG	"/etc/portfw.conf"
#define DEFAULT_PORT	80

#define IP_FORMAT	"ip_"
#define IP_FORMAT_LEN	3 + 10
//#define IP_FORMAT_LEN = strlen(IP_FORMAT) + 10;

void help(){
	printf("\nShifting Port Forwarder Version 1.0\n");
	printf(  "===================================\n");
	printf("CopyLeft (c) 06.2003, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>\n");
	printf("							 \"MOmchil Nenov\" <momchil@nmmm.nu>\n\n");

	printf("Usage:\n");
	printf("\t%s host [port] [config_file]\n", PROGNAME);
	printf("\n");
	
	printf("Compilled in settings:\n");
	printf("\tConfiguration file : %s\n", DEFAULT_CONFIG);
	printf("\tDefault port		 : %d\n", DEFAULT_PORT);
	printf("\n");
	
	printf("Configuration file syntax:\n");
	printf("\tport = xxx # default port\n");
	printf("\t%s1 = xxx.xxx.xxx.xxx # ip# 1\n", IP_FORMAT);
	printf("\t%s2 = xxx.xxx.xxx.xxx # ip# 2\n", IP_FORMAT);
	printf("\t%s3 = xxx.xxx.xxx.xxx # ip# 3\n", IP_FORMAT);
	printf("\t...\n");
	printf("\t%sn = xxx.xxx.xxx.xxx # ip# n\n", IP_FORMAT);
	printf("\n");

	exit(1);
}

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

#include <unistd.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int do_connect(const char *local_ip,const char *hostname, short port){

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
	bzero(&(addr.sin_zero),8);

	
/* bindvame client socketa - tova e tainata =) */

	local_addr.sin_family=AF_INET;
	local_addr.sin_port=0;// ura ura
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

#define MAXIPS 10000
#define BSIZE 0x10

inline char *sprintf1(char *buff, int i){
    sprintf(buff,"%s%d", IP_FORMAT, i); 
    return buff; 
}

int randomize(unsigned int limit){

	int entropy,seed;

	entropy=open("/dev/random",O_RDONLY);
	if(entropy==-1){
		perror("/dev/random");
		exit(1);
	}

	read(entropy,(void *)&seed,sizeof(int));
	close(entropy);

	srand(seed);

	return 1+(int)((double)limit*rand()/(RAND_MAX+1.0));

}

#define TIMEOUT 90

void comm(int sock){

	fd_set active_set,read_set;
	struct timeval tv;
	char buffer[BSIZE];
	int n;

	FD_ZERO(&read_set);
	FD_SET(sock,&read_set);
	FD_SET(STDIN_FILENO,&read_set);

	while(1){

		tv.tv_sec=TIMEOUT;
		tv.tv_usec=0;

		active_set=read_set;

		n=select(FD_SETSIZE,&active_set,NULL,NULL,&tv);
		if(n==-1){
			perror("select");
			exit(1);
		}else if(n>0){/* stuff to read on at least one fd */
			if(FD_ISSET(STDIN_FILENO,&active_set)){
				n=read(STDIN_FILENO,buffer,BSIZE);
				write(sock,buffer,n);
			}
			if(FD_ISSET(sock,&active_set)){
				n=read(sock,buffer,BSIZE);
				if(n==-1){
					perror("read");
					exit(1);
				}
				if(n==0)break;
				write(STDOUT_FILENO,buffer,n);
			}
		}else{
			break;/* timeout */
		}
	}

	close(sock);
}

int main(int argc, char *argv[]){

	if (argc < 1 + 1)
		help();

	const char *host = argv[1] ? argv[1] : "www.invalid.com";
	const unsigned short int port = argc < 1 + 1 ? DEFAULT_PORT : atoid(argv[2], DEFAULT_PORT);
	const char *conf_file = DEFAULT_CONFIG;

	
	Properties *conf = new Properties();
	conf->load(conf_file, '=');
//	conf->dump();
	
	unsigned int sum = 0;
	
	char buff[IP_FORMAT_LEN];

	for (int i=1;i<MAXIPS && !isempty(conf->get(sprintf1(buff,i)));i++)
		sum++;

	comm(do_connect(conf->get(sprintf1(buff,randomize(sum))),host,port));
	
	delete(conf);
	return 0;
}
