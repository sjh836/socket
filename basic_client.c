#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_check(int num, char *message);

void main(int argc, char *argv[])
{
	int socket_fd, byte_size;
	struct sockaddr_in server_adr;
	char message[100]={0};

	if(argc!=3)
	{
		printf("./실행파일 IP주소 PORT번호 형식으로 실행해야 합니다.\n");
		exit(1);
	}

	socket_fd=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	error_check(socket_fd, "소켓 생성");

	memset(&server_adr, 0, sizeof(server_adr));
	server_adr.sin_family=AF_INET;
	server_adr.sin_port=htons(atoi(argv[2]));
	server_adr.sin_addr.s_addr=inet_addr(argv[1]);

	error_check(connect(socket_fd, (struct sockaddr *)&server_adr, sizeof(server_adr)), "연결 요청");

	byte_size=read(socket_fd, message, sizeof(message));
	error_check(byte_size, "데이터 수신");
	printf("서버 메세지: %s\n", message);
	printf("%d 바이트를 수신했습니다\n", byte_size);

	close(socket_fd);

	printf("프로그램을 종료합니다\n");
}
void error_check(int num, char *message)
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