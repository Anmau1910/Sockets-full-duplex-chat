#include "chat.h"

#define PORTA 1234
#define PORTB 1235

int a;

void *t_server(void *arg){
	int fd_server, rc, client_fd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t sin_size = sizeof(struct sockaddr_in);
	ssize_t br;
  	int enable = 1, ret;
  	sigset_t set;

  	sigemptyset(&set);
  	sigaddset(&set, SIGUSR1);

  	pthread_sigmask(SIG_BLOCK, &set, NULL);

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server == -1 ){
		perror("Error in socket()");
		exit(EXIT_FAILURE);
  	}
  	
	ret = setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	if (ret < 0) perror("setsockopt(SO_REUSEADDR) failed");

  	
  	server.sin_family = AF_INET;         
  	if(!a) {
  		server.sin_port = htons(PORTA);       
	} else {
  		server.sin_port = htons(PORTB);
	}
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server.sin_zero), 8);       

  	rc = bind(fd_server, (struct sockaddr*)&server, sizeof(struct sockaddr));
	if(rc == -1){
		perror("Error in bind()");
		exit(EXIT_FAILURE);
	}

	rc = listen(fd_server, 10);
	if(rc == -1){
		perror("Error in listen()");
		exit(EXIT_FAILURE);
	}

  
	client_fd = accept(fd_server, (struct sockaddr *)&client, &sin_size);
	if(client_fd == -1) {
		if (errno != EINTR)
			perror("Error in accept()");
	}

	ip = client;
	sem_post(&asig);

	while ((br = recv(client_fd, buffer_l, 1023, 0)) > 0) {
		kill(getpid(), SIGUSR1);
	}
}

void *client_a(void * arg){
	sem_wait(&asig);
	int fd;
  	sigset_t set;

  	sigemptyset(&set);
  	sigaddset(&set, SIGUSR1);

  	pthread_sigmask(SIG_BLOCK, &set, NULL);
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1 ){
		perror("Error in socket()");
		exit(1);
  	}
	socklen_t sin_size = sizeof(struct sockaddr_in);
	ip.sin_port = htons(PORTB);
	
	if ((connect(fd, (struct sockaddr *)&ip, sin_size)) == -1)
	{
		perror("Error en connect()");
		exit(1);
	}

	while (1){
		sem_wait(&listo);
		if(send(fd, buffer_e, strlen(buffer_e), 0) < 0) puts("Error in send");
		bzero(buffer_e, 1024);
	}
}

void *client_b(void *iparg){
	char *ip = (char *)iparg;
	struct addrinfo *res;
	struct sockaddr_in *ipv4;
	struct sockaddr_in server;
	socklen_t sin_size = sizeof(struct sockaddr_in);
	int fd;
  	sigset_t set;

  	sigemptyset(&set);
  	sigaddset(&set, SIGUSR1);

  	pthread_sigmask(SIG_BLOCK, &set, NULL);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1 ) {
		perror("Error in socket()");
		exit(EXIT_FAILURE);
  	}
	getaddrinfo(ip, NULL, NULL, &res);

	server.sin_family = AF_INET;         
	server.sin_port = htons(PORTA);  
	ipv4 = (struct sockaddr_in *)res->ai_addr;
	server.sin_addr.s_addr = ipv4->sin_addr.s_addr; 
	bzero(&(server.sin_zero), 8);

	if((connect(fd, (struct sockaddr *)&server, sin_size)) < 0) {
		perror("Error in connect()");
		exit(EXIT_FAILURE);
	} 

	while (1) {
		sem_wait(&listo);
		if(send(fd, buffer_e, strlen(buffer_e), 0) < 0) puts("error lmao");
		bzero(buffer_e, 1024);
	}
}

int main_a(void){
	sem_init(&asig, 0, 0);
	pthread_t as, ac;
	pthread_create(&as, NULL, t_server, NULL);
	pthread_create(&ac, NULL, client_a, NULL);
	return 0;
}

int main_b(char *ip){
	pthread_t bs, bc;
	pthread_create(&bs, NULL, t_server, NULL);
	pthread_create(&bc, NULL, client_b, (void *)ip);

	return 0;
}
