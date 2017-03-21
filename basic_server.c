#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BACKLOG 5

void error_check(int num, char* message);

void main(int argc, char *argv[])
{
	int server_fd, client_fd, byte_size;
	struct sockaddr_in server_adr, client_adr;
	socklen_t client_adr_size;
	char message[]="C언어로 웹서버 프레임워크를 만들고야 말겠다";

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
	client_fd=accept(server_fd, (struct sockaddr *)&client_adr, &client_adr_size);
	printf("%s:%d의 연결요청\n", inet_ntoa(client_adr.sin_addr), ntohs(client_adr.sin_port));
	error_check(client_fd, "연결요청 승인");

	byte_size=write(client_fd, message, sizeof(message));
	printf("%d 바이트를 전송하였습니다\n", byte_size);
	error_check(byte_size, "데이터전송");

	close(client_fd);
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