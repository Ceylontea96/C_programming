# 프로젝트 목적

1. 1:N 소켓 통신 구현
2. Multi-Thread 사용
3. Mutex를 통한 임계구역

## 개발환경
- VIsual Studio 2019
- Microsoft Visual C++ 2019

## 프로그램 시퀀스
1. main을 통한 프로그램 시작
2. 프로그램 시작 시 Argument 입력값을 통한 server 및 client 구분
3. Server는 접속한 Client 관리 및 메시지 전달
4. Client는 특정 Server에 연결하여 Client간 채팅

### 프로그램 시작 cmd
1. [Server의 경우] socket.exe (ip) (port) ()
2. [Client의 경우] socket.exe 

### trouble shooting
1. client socket file discrupter 관리
   - Server에 연결된 Client socket을 관리하는 과정에서, 연결이 끊긴 socket을 제외하지 않던 문제 해결
2. Mutex 사용
   - 다수의 Client socket을 관리할 때 client socket 배열과 인덱스가 동시 동작 thread로 인해 제대로 관리되지 못했던 문제
   - Mutex로 공유 변수를 제어하여 해결
   

