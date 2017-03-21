#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BACKLOG 3
#define BUF_SIZE 128

void error_check(int num, char* message);

void main(int argc, char *argv[])
{
	int server_fd, client_fd, str_len, i;
	struct sockaddr_in server_adr, client_adr;
	socklen_t client_adr_size;
	char message[BUF_SIZE]={0};

	if(argc!=2)
	{
		fprintf(stderr, "./실행파일 port번호 형식으로 입력해야합니다\n");
		exit(1);
	}
	server_fd=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	error_check(server_fd, "소켓 생성");

	memset(&client_adr, 0, sizeof(client_adr));
	memset(&server_adr, 0, sizeof(server_adr));
	server_adr.sin_family=AF_INET;
	server_adr.sin_port=htons(atoi(argv[1]));
	server_adr.sin_addr.s_addr=htonl(INADDR_ANY);

	error_check(bind(server_fd, (struct sockaddr *)&server_adr,sizeof(server_adr)), "소켓주소 할당");
	error_check(listen(server_fd, BACKLOG), "연결요청 대기");

	client_adr_size=sizeof(client_adr);
	for(i=0; i<BACKLOG; i++)
	{
		client_fd=accept(server_fd, (struct sockaddr *)&client_adr, &client_adr_size);
		printf("%s:%d의 연결요청\n", inet_ntoa(client_adr.sin_addr), ntohs(client_adr.sin_port));
		error_check(client_fd, "연결요청 승인");

		while(1)
		{
			str_len=read(client_fd, message, BUF_SIZE);
			error_check(str_len, "데이터수신");
			if(str_len==0) break;
			printf("%d 바이트를 수신하였습니다\n", str_len);

			str_len=write(client_fd, message, str_len);
			printf("%d 바이트를 전송하였습니다\n", str_len);
			error_check(str_len, "데이터전송");
		}

		close(client_fd);
		printf("%s:%d의 연결종료\n", inet_ntoa(client_adr.sin_addr), ntohs(client_adr.sin_port));
	}
	close(server_fd);

	printf("프로그램을 종료합니다\n");
}
void error_check(int num, char* message)
{
	if(num==-1)
	{
		fprintf(stderr, "%s 오류\n", message);
		exit(1);
	}
	else
	{
		fprintf(stdout, "%s 완료\n", message);
	}
}