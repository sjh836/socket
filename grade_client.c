/*
	17-1학기 네트워크 프로그래밍 4A
	2012335054 이정훈
	제출일: 2017.3.27
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define MAX 3 //스택 배열의 크기

void error_check(int validation, char *message); //실행 오류 검사
int value_check(int score); //점수 유효값 검사

void main(int argc, char *argv[])
{
	int socket_fd, array_len, recv_len, recv_count;
	struct sockaddr_in server_adr;
	int top=0; //스택포인터
	int score[MAX]; //점수를 쌓아두는 스택
	char result; //학점

	if(argc!=3)
	{
		printf("./실행파일 IP주소 PORT번호 형식으로 실행해야 합니다.\n");
		exit(1);
	}

	socket_fd=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP소켓 생성
	error_check(socket_fd, "소켓 생성");

	memset(&server_adr, 0, sizeof(server_adr)); //구조체 변수 값 초기화
	server_adr.sin_family=AF_INET; //IPv4
	server_adr.sin_port=htons(atoi(argv[2])); //포트 할당
	server_adr.sin_addr.s_addr=inet_addr(argv[1]); //IP 할당

	error_check(connect(socket_fd, (struct sockaddr *)&server_adr, sizeof(server_adr)), "연결 요청");

	printf("국어점수를 입력해주세요 : ");
	scanf("%d", &score[top]);
	score[top]=value_check(score[top]);
	top++;

	printf("영어점수를 입력해주세요 : ");
	scanf("%d", &score[top]);
	score[top]=value_check(score[top]);
	top++;

	printf("수학점수를 입력해주세요 : ");
	scanf("%d", &score[top]);
	score[top]=value_check(score[top]);

	array_len=write(socket_fd, score, sizeof(score)); //점수배열을 서버로 전송
	printf("%d 바이트를 전송하였습니다\n", array_len);
	error_check(array_len, "데이터전송");

	recv_len=0;
	while(recv_len!=sizeof(result)) // 패킷이 잘려서 올수도 있으므로 예외처리를 위한 조건문
	{
		recv_count=read(socket_fd, &result, sizeof(result));
		error_check(recv_count, "데이터수신");
		printf("%d 바이트를 수신하였습니다\n", recv_count);
		recv_len+=recv_count;
	}
	printf("-> 수신 학점 : %c\n", result);

	close(socket_fd);

	printf("프로그램을 종료합니다\n");
}
void error_check(int validation, char *message)
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
int value_check(int score)
{
	if(score<0||score>100)
	{
		printf("값이 유효하지 않습니다. 다시입력해주세요 : ");
		scanf("%d", &score);
		score=value_check(score); //유효값 입력할때까지 재귀호출
	}
	return score;
}