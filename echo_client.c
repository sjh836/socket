#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 128

void error_check(int num, char *message);

void main(int argc, char *argv[])
{
	int socket_fd, str_len, recv_len, recv_count;
	struct sockaddr_in server_adr;
	char message[BUF_SIZE]={0};

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

	while(1)
	{
		printf("메세지를 입력해주세요(나가기:quit) : ");
		fgets(message, BUF_SIZE-1, stdin);
		if(strcmp(message, "quit\n")==0) break;

		str_len=write(socket_fd, message, strlen(message));
		printf("%d 바이트를 전송하였습니다\n", str_len);
		error_check(str_len, "데이터전송");

		recv_len=0;
		while(recv_len!=str_len)
		{
			recv_count=read(socket_fd, message, BUF_SIZE);
			error_check(recv_count, "데이터 수신");
			recv_len+=recv_count;
		}
		message[str_len]=0;
		printf("서버 메세지: %s", message);
		printf("%d 바이트를 수신했습니다\n", str_len);
	}
	
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