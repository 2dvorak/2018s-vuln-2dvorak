# HER Messenger - Team3

### HER messenger
- 빠르고 안전해 믿고 쓰는 HER 메신저!

### Abstract
- PGP를 이용한 암호화된 메시지 송/수신
- OnionRouting으로 중간단계에서 발신자, 수신자 익명화
-
-

### Protocol Design

| Field   | Message Protocol | Key Protocol |
|--------|------------------|--------------|
| id     | id=1 : 메시지 | id=0 : Key |
| bullian| bool? 전달 : 내 메시지 | bool? 로그인:로그아웃 |
| sendIP|  sender IP(마지막 메시지)       | sender IP        |
| recvIP | receiver IP     | receiver IP             |
| githubID | sender id(마지막 메시지)  | sender id |
| content| 암호화된 메시지 | sender Public Key |

- 메시지를 JSON 형식으로 만들어 전송
ex) Message : ```{“id”:“1”, “bullian”:“0”, “recvip”:“172.0.0.3”,“content”:“<암호화된 메시지>”}```

### OnionRouting
- ex) Message 전송순서 : A -> B -> C -> D

![onion](./images/onion.jpg)

### Installation
~$_ : 현재 사용자의 명령 대기
~#_ : 슈퍼 유저의 명령 대기

- 컴파일을 위한 패키지 설치
```
$ sudo apt-get install g++
$ sudo apt-get install make
$ sudo apt-get install qt4-qmake
$ sudo apt-get install libncurses-dev
```
- 패키지 설치 이후
```
$ git clone https://github.com/KAIST-IS521/2018s-onion-team3.git
$ curl -fsSL https://get.docker.com/ | sudo sh
$ cd 2018s-onion-team3  
$ ./AutoDocker.sh
```
- 본인 비밀키 파일 설정하기
testkey 폴더에 [Your id].key와 [Your id].pub를 넣어주기
명령어는 다음과 같다.
```
$
$
```
### Usages
```
# ./startMessenger.sh
# ./setup.sh
# cd onionMessenger
# mkdir -p .obj
# make
# ./onionMessenger
Your Github ID :[YOUR id]
Your passphrase :[YOUR passphrase]
██╗  ██╗███████╗██████╗
██║  ██║██╔════╝██╔══██╗
███████║█████╗  ██████╔╝
██╔══██║██╔══╝  ██╔══██╗
██║  ██║███████╗██║  ██║
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝
1. List
2. Talk
3. Exit

메뉴 1.
```

### Examples
- githubA user
```
# ./onionMessenger/onionMessenger githubA ???????
██╗  ██╗███████╗██████╗
██║  ██║██╔════╝██╔══██╗  
███████║█████╗  ██████╔╝  
██╔══██║██╔══╝  ██╔══██╗  
██║  ██║███████╗██║  ██║  
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝  
1. List
2. Talk
3. Exit
> 1  
 === Your List! ===
githubB : 172.17.0.2
 ==== Done! ====

1. List
2. Talk
3. Exit
> 2
                             ┌────────────────────────────┐                             
                             │ githubB                    │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             └────────────────────────────┘                             
Her                                                                                      
Me: hi                                                                                   
githubB: Hello                                                                           
Me: I am githubA                                                                         
githubB: i am github B~                                                                  
```

- github user
```
# ./onionMessenger/onionMessenger githubB ???????
██╗  ██╗███████╗██████╗
██║  ██║██╔════╝██╔══██╗
███████║█████╗  ██████╔╝
██╔══██║██╔══╝  ██╔══██╗
██║  ██║███████╗██║  ██║
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝
1. List
2. Talk
3. Exit
> 1
 === Your List! ===
githubA : 172.17.0.3
 ==== Done! ====

1. List
2. Talk
3. Exit
> 2

                             ┌────────────────────────────┐                             
                             │ githubA                    │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             │                            │                             
                             └────────────────────────────┘                             

Her
githubA: hi
Me: Hello
githubA: I am githubA
Me: i am github B~
```

### Environment
- Git issue와 milestone을 사용하여 편한 의사소통 및 협업 가능
- JSON을 사용하여 빠르고 편한 파싱 및 헤더 구성
- ncurses를 이용한 보기 좋은 TUI환경 구성
- Qt Creator를 사용하여 Makefile의 자동 생성 및 편한 개발환경 구축


### Team member

| Team member     | Position | Assignment |
|:---------------:|:--------:|------------|
|  SeongIl Wi     |    TA    | Advice |
|  DongHyeon Oh   |   팀장   |HER messenger 개발 총괄|
|  SeungYeop Lee  |   팀원   |HER messenger 개발|
|  NakJun Choi    |   팀원   |PITCHME.md 작성, 홍보영상 편집, Presentation|
|  SeJin Jeong    |   팀원   |Sockthread.cpp 함수 개발, README.md 작성 |

### Insight

### Reference
 - https://github.com/nlohmann/json
