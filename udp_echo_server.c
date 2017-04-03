#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SIZE 128

void error_check(int validation, char *message);

void main(int argc, char *argv[])
{
	int server_fd, str_len;
	char message[SIZE];
	struct sockaddr_in server_adr, client_adr;
	socklen_t client_adr_size;

	if(argc!=2)
	{
		fprintf(stderr, "./실행파일 port번호 형식으로 입력해야합니다\n");
		exit(1);
	}
	server_fd=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	error_check(server_fd, "UDP소켓 생성");

	memset(&server_adr, 0, sizeof(server_adr));
	memset(&client_adr, 0, sizeof(client_adr));
	server_adr.sin_family=AF_INET;
	server_adr.sin_port=htons(atoi(argv[1]));
	server_adr.sin_addr.s_addr=htonl(INADDR_ANY);

	error_check(bind(server_fd, (struct sockaddr *)&server_adr, sizeof(server_adr)), "서버 소켓주소 할당");

	client_adr_size=sizeof(client_adr);
	while(1) //TCP처럼 EOF가 날라와서 break할수 없기때문에 무한반복된다. 강제로 끄는 수밖에.. 물론 udp클라이언트가 quit를 전송해서 끊을수야 있겠지만!
	{
		str_len=recvfrom(server_fd, message, SIZE, 0, (struct sockaddr *)&client_adr, &client_adr_size);
		printf("%s:%d의 UDP통신\n", inet_ntoa(client_adr.sin_addr), ntohs(client_adr.sin_port));
		error_check(str_len, "데이터 수신");
		printf("수신한 메세지 : %s", message);

		error_check(sendto(server_fd, message, str_len, 0, (struct sockaddr *)&client_adr, client_adr_size), "데이터 송신");
	}

	close(server_fd);
	printf("프로그램을 종료합니다\n");
}
void error_check(int validation, char* message)
{
	if(validation==-1)
	{
		fprintf(stderr, "%s 오류\n", message);
		exit(1);
	}
	else
	{
		fprintf(stdout, "%s 완료\n", message);
	}
}