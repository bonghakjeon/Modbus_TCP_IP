#pragma comment(lib, "WS2_32.lib")
// Windows 소켓 라이브러리(WS2_32.lib)를 사용함. 

#include <Winsock2.h>   // 윈속 라이브러리를 추가함. 
#include <stdio.h>      
#include <stdlib.h>     // atoi 함수 사용
#include <conio.h>      // 콘솔에서 입출력하기 위해


int main(int argc, char* argv[])
// argc는 명령행 옵션의 개수가 저장되는 곳
// argv는 프로그램을 실행할 때 지정해준 "명령행 옵션의 문자열들"
// 이 실제로 저장되는 배열
{
    if (argc < 5)
        // 명령행 옵션의 갯수가 5개 미만일 경우 
        // 에러 메시지 출력
    {
        // 5 0 10 을 어떻게 처리하는지?? num_regs는 어떻게 사용 하는지?
        // reg_no  : Start Address(ADC PORT 시작번지)
        // num_reg : Length (Read) 시작번지부터 읽을 워드의 개수 
        // unit : 장치 ID
        // ip_addr : 198.202.138.72, unit : 5, reg_no : 0, num_regs = 10 이 저장됨.
        printf("test1의 ip_addr unit reg_no num_regs\n"
            "ex : test1 198.202.138.72 5 0 10\n");
        exit(1);   // 에러시 강제 종료
    }

    char* ip_addr = argv[1];
    // argv[0]에는 실행 파일명(test1)이 저장됨.
    // argv[1]에 실행파일 test1의 첫번째 인자의 주소를 가리킴(198.202.138.72)

    unsigned short unit = atoi(argv[2]);
    // 실행파일 test1의 두번째 인자의 주소 값을 정수로 변경 후
    // unsigned short형 변수인 unit에 저장함.(5)

    unsigned short reg_no = atoi(argv[3]);
    /* 실행파일 test1의 세번째 인자의 주소 값을 정수로 변경 후
       unsigned short형 변수인 reg_no에 저장함.(0) */

    unsigned short num_regs = atoi(argv[4]);
    /* 실행파일 test1의 네번째 인자의 주소 값을 정수로 변경 후
       unsigned short형 변수인 num_regs에 저장함.(10) */

    printf("ip_addr의 첫번째 인자의 주소 값 = %s, unit = %d, reg_no = %d, num_regs = %d\n",
        ip_addr, unit, reg_no, num_regs);

    // 윈속 초기화
    static WSADATA wsaData;
    // 윈속 초기화 정보를 담을 구조체 변수 wsaData 선언
    /* WSAStartup 함수
       WSAStartip 함수는 두 개의 인수가 있다.
       첫 번째 인수는 윈속의 버전을 지정한다.
       해당 소스에서는 2.2 버전을 쓰고 있다.

       MAKEWORD 함수
       MAKEWORD는 2개의 인수는 WORD로 pack하는 매크로 함수이다.
       WORD는 16비트의 부호가 없는 변수로 WSAStartup 함수에서는 WORD의 16비트에서
       상위 8비트에 Major 번호 하위 8비트에 Minor 번호를 넣어 넘겨주어야 한다.

       WSAStartup 함수의 두 번째 인수(wsaData)는 WSAStartup이 초기화된 상태를
       저장하는 변수를 넘겨준다.
       WSAStartup은 윈속을 초기화하고 초기화한 상태를 WSADATA 구조체에 저장한다.
       */


       /* WSAStartup 함수를 사용하여 0을 반환하면 윈속 초기화에 성공함을 의미하고,
          0이 아닌 다른 값을 반환하면 윈속 초기화에 실패함을 의미한다. */

          /* 윈속 초기화가 안 되는 경우
             1. 네트워크 접속 준비를 못한 경우

             2. 요구한 윈속 버전이 서포트가 안 될 경우

             3. 블로킹 조작이 실행 중인 경우

             4. 최대 윈속 프로세스 처리 수가 초과된 경우

             5. 두 번째 인수(wsaData)의 포인터가 무효할 경우(주소값이 안 맞을 경우)
           */
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("윈속 초기화 실패\n");
        exit(1);  // 에러시 강제 종료
    }

    // 소켓 생성
    SOCKET s; // 변수선언

    // TCP 소켓 핸들 생성(PF_INET - IPv4인터넷 프로토콜)
    // SOCK_STREAM : TCP 통신의 한 종류
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* sockaddr_in 구조체란?
       소켓의 주소를 담는 기본 구조체 역할을 하며,
       구조체의 멤버인 sa_family가 AF_INET인 경우에 사용하는 구조체이다.
       이 구조체에서 사용하는 IP주소는 IPv4 주소체계를 사용한다. */
    struct sockaddr_in server;

    /* AF_INET이란?
       IPv4 인터넷 주소 체계를 표현할 때 사용한다.

       PF_INET이란?
       AF_INET과 비슷하지만 다른 점은 IPv4 인터넷 프로토콜 체계를 표현할 때 사용함.

       struct sockaddr_in {
            short sin_family;          // 주소체계 : AF_INET
            u_short sin_port;          // 16 비트 포트 번호, network byte order
            struct in_addr sin_addr;   // 32 비트 IP 주소
            char sin_zero[8];          // 전체 크기를 16비트로 맞추기 위한 dummy
       };

       struct in_addr {
            u_long s_addr;             // 32 비트 IP 주소를 저장할 구조체, network byte order
       };

       */
    server.sin_family = AF_INET;
    server.sin_port = htons(502); // ASA standard port(기본 포트번호는 TCP 502번)
    // ip 주소(ip_addr)
    /* inet_addr 함수
       문자열로 표현한 IPv4 주소를 네트워크 바이트 정렬 방식의 4바이트 정수로 변환

       inet_addr 함수 원형
       unsigned long inet_addr(const char * ip);
       ip - 문자열로 표현한 IPv4 주소

       inet_ntoa 함수
       네트워크 바이트 정렬 방식의 4바이트 정수의 IPv4 주소를 문자열 주소로 변환

       inet_ntoa 함수 원형
       char * inet_ntoa(struct in_addr in);

       in_addr 구조체
       네트워크 바이트 정렬 방식의 4바이트 정수인 IPv4 주소를 표현할 때 사용하는 구조체 */
    server.sin_addr.s_addr = inet_addr(ip_addr);
    int i;

    /* connect 함수를 호출하여 서버로 연결 요청을 함.
       연결 성공 시 0 반환. 실패 시 SOKET_ERROR 반환 */
    i = connect(s, (sockaddr*)&server, sizeof(sockaddr_in));

    // 연결 실패할 경우!
    if (i < 0)
    {
        /* WSAGetLastError 함수를 호출하여 가장 최근 발생한
           소켓 에러에 대응되는 에러코드를 반환한다.*/
        printf("연결 실패 %d\n", WSAGetLastError());

        // 함수 closesocket을 호출하여 소켓을 닫는다.
        closesocket(s);

        // 함수 WSACleanup을 호출하여 윈속라이브러리를 운영체제에 반환함.
        WSACleanup();

        exit(1);   // 에러시 강제 종료
    }

    // 소켓을 그룹짓기 위하여 fd_set 구조체 변수 fds 선언
    /* fd_set 구조체 멤버
       __FD_SETSIZE : FD 집합의 크기(fd_set 소켓 그룹에 포함 가능한 소켓의 최대 갯수)
       __fd_mask : FD를 비트단위로 저장할 때 4바이트 단위로 나누어서 저장함.
       */
    fd_set fds;

    // FD_ZERO 매크로 함수를 호출하여 fd_set 구조체로 선언된 변수 fds를 초기화함.
    FD_ZERO(&fds);


    /*  timeval 구조체 정의

     struct timeval
     {
         long tv_sec;
         long tv_usec;
     }

     timeval 구조체 멤버 변수의 의미

     tv_sec -> 1초
     tv_usec -> 1 / 1000000 초

     아래는 예제 코드이다.

     sec 와 usec 를 구분해서 사용할 수 있고,
     그 시간만큼 지연(delay) 시킬 수 있다. */

    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    // wait for permission to send
    /* 함수 FD_SET를 호출하여 소켓 s의 값을 fd_set으로 선언된 fds 변수에 추가함.*/
    FD_SET(s, &fds);

    /* select 함수를 호출하여 한 프로세스에 여러개의 소켓을 관리함.

       함수 원형
       select(검사하고자 하는 소켓 + 1, 읽기셋 주소, 쓰기셋 주소, 예외셋 주소,
              타임아웃 시간 설정)

       반환 값 종류
       -1 : 오류 발생
        0 : 타임아웃에 의한 리턴
        0 이상 : 변화 발생한 디스크립터 갯수 */
    i = select(32, NULL, &fds, NULL, &tv); // write만 진행.

    // 오류가 발생한 경우 
    if (i <= 0)
    {
        /* WSAGetLastError 함수를 호출하여 가장 최근 발생한
           소켓 에러에 대응되는 에러코드를 반환한다.*/
        printf("select 에러 발생!(소켓에 데이터 전송 오류) %d\n", WSAGetLastError());

        // 함수 closesocket을 호출하여 소켓을 닫는다.
        closesocket(s);

        // 함수 WSACleanup을 호출하여 윈속라이브러리를 운영체제에 반환함.
        WSACleanup();

        // 에러시 강제 종료
        exit(1);
    }

    // build request of form 0 0 0 0 0 6 ui 3 rr rr nn nn
    unsigned char obuf[261];   // 출력 버퍼
    unsigned char ibuf[261];   // 입력 버퍼

    for (i = 0; i < 5; i++) obuf[i] = 0; // 출력 버퍼 0으로 초기화
    obuf[5] = 6;
    /* 숫자 6의 의미?
       프로토콜 ID 또는 프로토콜 번호를 의미함.
       주요 프로토콜 번호들
       ICMP : 1, IGMP : 2, TCP : 6, EGP : 8, UDP : 17, DSPF : 89 등 */
    obuf[6] = unit;
    obuf[7] = 3;   // Function code(03 또는 0x03) Read Holding Register를 의미함.
    // 상위장치(Master)에서 메모리 읽기 가능

    // reg_no 비트 값을 오른쪽으로 8번 이동(reg_no / (2^8))
    obuf[8] = reg_no >> 8;

    /* reg_no과 0xff를 비트 AND 연산 진행
       0xFF는 2 진수로 11111111 인 16 진수 상수이다.
       이 상수와 비트 AND (&)를 사용하면 원본의 마지막 8 비트만 남음 */
    obuf[9] = reg_no & 0xff;


    // num_regs 비트 값을 오른쪽으로 8번 이동(reg_no / (2^8))
    obuf[10] = num_regs >> 8;

    // num_regs와 0xff를 비트 AND 연산 진행
    /* 0xFF는 2 진수로 11111111 인 16 진수 상수이다.
       이 상수와 비트 AND (&)를 사용하면 원본의 마지막 8 비트만 남음 */
    obuf[11] = num_regs & 0xff;

    /* send 함수를 호출하여 윈도우 소켓 기반으로 된 전송할 데이터를
       출력버퍼 obuf에 저장함.
       성공 시 전송된 바이트 수 반환, 실패 시 SOCKET_ERROR 반환. */
    i = send(s, (const char*)obuf, 12, 0);

    // 출력버퍼에 전송된 바이트 수가 12 바이트보다 작을 경우 
    if (i < 12)
    {

        printf("12 문자를 전송하는데 실패하였습니다! \n");
    }

    // wait for response
    /* 함수 FD_SET를 호출하여 소켓 s의 값을 fd_set으로 선언된 fds 변수에 추가함.*/
    FD_SET(s, &fds);

    /* select 함수를 호출하여 한 프로세스에 여러개의 소켓을 관리함.

       함수 원형
       select(검사하고자 하는 소켓 + 1, 읽기셋 주소, 쓰기셋 주소, 예외셋 주소,
              타임아웃 시간 설정)

       반환 값 종류
       -1 : 오류 발생
        0 : 타임아웃에 의한 리턴
        0 이상 : 변화 발생한 디스크립터 갯수 */
    i = select(32, &fds, NULL, NULL, &tv); //read만 진행.


    // 오류가 발생한 경우
    if (i <= 0)
    {
        printf("select 에러 발생!(소켓에 데이터 읽기 오류)\n");

        // 함수 closesocket을 호출하여 소켓을 닫는다.
        closesocket(s);

        // 함수 WSACleanup을 호출하여 윈속라이브러리를 운영체제에 반환함.
        WSACleanup();

        // 에러시 강제 종료
        exit(1);
    }

    // read response
    /* recv 함수를 호출하여 윈도우 소켓 기반으로 된 수신할 데이터를
       입력버퍼 ibuf에 저장함.
       성공 시 수신한 바이트 수 반환, 실패 시 SOCKET_ERROR 반환.*/
    i = recv(s, (char*)ibuf, 261, 0);

    // 입력버퍼에 전송된 바이트 수가 9 바이트보다 작을 경우 
    if (i < 9)
    {
        if (i == 0)   // 수신한 바이트가 0이면
        {
            printf("원격 연결이 예기치 않게 종료되었습니다. \n");
        }
        else
        {
            printf("수신된 문자열이 너무 짧습니다. - 수신된 문자열의 수는 %d 개 입니다.\n", i);
        }
    }


    else if (ibuf[7] & 0x80)
        /* ibuf 배열의 7번지에 존재하는 unsigned char형 ASCII 문자 한개와
           0x80의 비트 AND 연산 진행함.
           ASCII가 아닌 문자일 경우 TRUE 반환하여 else if문 실행함.
           ASCII 문자면 false 가 반환되어 실행 못함. */
    {
        printf("MODBUS 예외 응답 - 문자 타입은 %d 입니다. \n", ibuf[8]);
    }


    else if (i != (9 + 2 * num_regs))
    {
        printf("대략적으로 수신된 문자열의 길이는 %d이고, 예상은 %d 입니다. \n", i, (9 + 2 * num_regs));
    }


    else
    {
        for (i = 0; i < num_regs; i++)
        {
            /* 입력 버퍼에 저장된 값들을 unsigned short형 변수 w에 저장한 후
               for문을 사용하여 하나씩 출력함. */
            unsigned short w = (ibuf[9 + i + i] << 8) + ibuf[10 + i + i];
            printf("%d번째 입력된 문자 = %d\n", i, w);
        }
    }


    // close down
    // 함수 closesocket을 호출하여 소켓을 닫는다.
    closesocket(s);

    // 함수 WSACleanup을 호출하여 윈속라이브러리를 운영체제에 반환함.
    WSACleanup();

    return 0;
}
