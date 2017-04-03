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
	int client_fd, str_len;
	char message[SIZE];
	struct sockaddr_in server_adr, from_adr;
	socklen_t from_adr_size;

	if(argc!=3)
	{
		fprintf(stderr, "./실행파일 ip주소 port번호 형식으로 입력해야합니다\n");
		exit(1);
	}
	client_fd=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	error_check(client_fd, "UDP소켓 생성");

	memset(&server_adr, 0, sizeof(server_adr));
	memset(&from_adr, 0, sizeof(from_adr));
	server_adr.sin_family=AF_INET;
	server_adr.sin_port=htons(atoi(argv[2]));
	server_adr.sin_addr.s_addr=inet_addr(argv[1]);

	from_adr_size=sizeof(from_adr);
	while(1)
	{
		printf("메세지를 입력해주세요(나가기:quit) : ");
		fgets(message, SIZE-1, stdin);
		if(strcmp(message, "quit\n")==0) break;
		error_check(sendto(client_fd, message, strlen(message), 0, (struct sockaddr *)&server_adr, sizeof(server_adr)), "데이터 송신");
		str_len=recvfrom(client_fd, message, SIZE, 0, (struct sockaddr *)&from_adr, &from_adr_size);
		error_check(str_len, "데이터 수신");
		message[str_len]=0;
		printf("서버의 메세지: %s", message);
	}

	close(client_fd);
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