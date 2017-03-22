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
#define BACKLOG 3 //연결대기 큐 숫자
#define MAX 3 //스택 배열의 크기

void error_check(int validation, char *message); //실행 오류 검사
char grade(double score); //학점계산

void main(int argc, char *argv[])
{
	int server_fd, client_fd, array_len, i, recv_len, recv_count;
	struct sockaddr_in server_adr, client_adr;
	socklen_t client_adr_size;
	int top=0; //스택 포인터
	int score[MAX]={0}; //점수 배열
	double avg; //평균점수는 소수점이 나올수도 있다
	char result; //학점

	if(argc!=2)
	{
		fprintf(stderr, "./실행파일 port번호 형식으로 입력해야합니다\n");
		exit(1);
	}
	server_fd=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP 소켓 생성
	error_check(server_fd, "소켓 생성");

	memset(&client_adr, 0, sizeof(client_adr)); //구조체 변수 값 초기화
	memset(&server_adr, 0, sizeof(server_adr)); //구조체 변수 값 초기화
	server_adr.sin_family=AF_INET; //IPv4
	server_adr.sin_port=htons(atoi(argv[1])); //포트 할당
	server_adr.sin_addr.s_addr=htonl(INADDR_ANY); //IP주소 할당

	error_check(bind(server_fd, (struct sockaddr *)&server_adr,sizeof(server_adr)), "소켓주소 할당"); //주소 바인딩
	error_check(listen(server_fd, BACKLOG), "연결요청 대기");

	client_adr_size=sizeof(client_adr);

	client_fd=accept(server_fd, (struct sockaddr *)&client_adr, &client_adr_size); //특정 클라이언트와 데이터 송수신용 TCP소켓 생성
	printf("* %s:%d의 연결요청\n", inet_ntoa(client_adr.sin_addr), ntohs(client_adr.sin_port));
	error_check(client_fd, "연결요청 승인");

	recv_len=0;
	while(recv_len!=sizeof(score)) // 패킷이 잘려서 올수도 있으므로 예외처리를 위한 조건문
	{
		recv_count=read(client_fd, score, sizeof(score));
		error_check(recv_count, "데이터수신");
		printf("%d 바이트를 수신하였습니다\n", recv_count);
		recv_len+=recv_count;
	}

	for(i=0;i<3;i++)
	{
		printf("score[%d]=%d\n",i,score[i]); //수신된 값 출력, 디버깅용
	}
	avg=(score[0]+score[1]+score[2])/(double)3; //평균점수 연산
	printf("평균점수=%lf\n",avg);
	result=grade(avg);
	printf("-> 학점 : %c\n",result);

	array_len=write(client_fd, &result, sizeof(result));
	printf("%d 바이트를 전송하였습니다\n", array_len);
	error_check(array_len, "데이터전송");

	close(client_fd);
	printf("* %s:%d의 연결종료\n", inet_ntoa(client_adr.sin_addr), ntohs(client_adr.sin_port));
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
char grade(double score)
{
	if(score>=90) return 'A';
	else if(score>=80) return 'B';
	else if(score>=70) return 'C';
	else if(score>=60) return 'D';
	else return 'F';
}