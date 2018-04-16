# SEHer Messenger - IS521 Activity 5

### SEHer Messenger : Security Enhanced Her Messenger
- 더욱 안전해 더더욱 믿고 쓰는 SEHer 메신저
- Fingerprint를 사용한 암호화로 안전성 강화
- Fingerprint 검증으로 위조 공격 방지

### Abstract
- GnuPG 암호화 시 Fingerprint를 사용해 키 충돌 방지
- GnuPG Fingerprint 소개
- Fingerprint 검증 시 Logic Error
- 해당 Logic Error를 악용하면, stealthy sniffing 가능

### New Features for SEHer
- 암호화시 Fingerprint 사용 : Short Key ID(8자) 사용으로 인한 충돌 방지
- Pubkey와 Fingerprint 검증 : 검증되지 않은 사용자 거절

### GnuPG Fingerprint
GnuPG에서 모든 Key들은 Fingerprint를 부여받는다. Fingerprint는 20바이트이고, 따라서 Hex 값을 출력하면 40바이트가 된다. GnuPG를 사용한 Encryption 시 수신자의 이름, 이메일 이외에도 Fingerprint를 사용할 수 있다(-r, --recipient 옵션). 뿐만 아니라 Fingerprint의 일부 값을 따온 Long Key Id와 Short Key ID도 사용할 수 있다. Fingerprint, Long Key Id, Short Key ID간의 관계는 다음과 같다.
```
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 byte(40 byte)
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|C|5|D|8|0|4|C|8|E|5|A|6|C|8|4|7|6|6|C|1|A|8|3|B|9|1|E|0|7|6|0|0|D|9|5|9|5|B|4|8|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                               |               |               |
|                                               |               |<--  8 byte -->| : Short Key ID
|                                               |                               |
|                                               |<------------------ 16 byte -->| : Long Key ID
|                                                                               |
|<------------------------------------------------------------------ 40 byte -->| : Fingerprint
```

### Vulnerabilities
- Logic Error 1
GnuPG를 사용해 Encryption 하는 코드에서 사용되는 커맨드는 다음과 같다.
```
"gpg --encrypt --armor --yes --trust-model always -r " + recipientFpr(40 byte)
```
각각의 사용자는 Key Protocol을 통해 Fingerprint를 교환하며, 이 때 Fingerprint의 길이가 40인지 검증하게 된다. 이를 통해 악의적인 공격자가 Key Protocol을 조작해서 자신의 Fingerprint도 끼워넣는 다음과 같은 공격을 방지할 수 있다.
```
"gpg --encrypt --armor --yes --trust-model always -r " + recipientFpr(40 byte) + " -r " + attackerFpr(40 byte)
```
하지만 이 로직은 GnuPG가 Encryption 시 Key ID도 정상적인 인자로 처리한다는 사실을 간과하고 있다. 따라서, 공격자는 정상적인 Fingerprint를 잘라서 Long Key ID를 얻어내고, 자신의 Public Key에 대한 Long Key ID와 함께 이어붙여서 40자 이내로 페이로드를 만들 수 있다.
```
"gpg --encrypt --armor --yes --trust-model always -r " + recipientKeyID(16 byte) + " -r " + attackerKeyID(16 byte)
```
따라서 메신저의 Encryption 관련 코드는 GnuPG의 동작을 제대로 이해하지 못한 것으로 인한 Logic Error를 가지고 있다.

- Logic Error 2
Key Protocol에서 다른 사용자의 Public Key와 함께 Fingerprint를 받으면, 서로 검증을 하게 된다.
메신저는 먼저 Public Key를 import한다
```
gpg --import pubkey
```
해당 커맨드는 popen을 통해 실행되고, 실행 결과를 fgetc()함수를 통해 받아오기 때문에, 다음과 같은 실행 결과를 얻을 수 있다.
```
gpg: key D9595B48: public key "githubB" imported
gpg: Total number processed: 1
gpg:               imported: 1  (RSA: 1)
```
여기서 GnuPG가 새로이 import한 Public Key의 Short Key ID만 출력해주기 때문에, 메신저는 해당 내용을 가지고 Fingerprint를 검증하기 위해 Fingerprint의 마지막 8 byte만 사용한다.
```
Imported Public Key's Short Key ID(8 byte) == Received Fingerprint(40 byte)'s substring(32 ~ 40)?
```
하지만 공격자가 악의적인 Fingerprint를 만들 때, 정상적인 Short Key ID만 마지막 8바이트에 위치시킨다면 해당 검증을 우회할 수 있다. 나머지 32바이트를 사용하여 공격자는 자신의 Long Key ID 또는 Short Key ID를 넣을 수 있다.

### Protocol Design

| Field   | Message Protocol | Key Protocol |
|--------|------------------|--------------|
| id     | id=1 : 메시지 | id=0 : Key |
| bullian| bool? 전달 : 내 메시지 | bool? 로그인:로그아웃 |
| sendIP|  sender IP(마지막 메시지)       | sender IP        |
| recvIP | receiver IP     | receiver IP             |
| githubID | sender id(마지막 메시지)  | sender id |
| content| 암호화된 메시지 | sender Public Key |
| fpr | - | Public Key's Fingerprint |

- 변경된 Key 전송 scheme
```  
 {"id":"0", "bullian":"1","sendIP" : "172.17.0.4", "fpr" : "<PGP Public key Fingerprint", "recvip":"172.17.0.3","githubID":"skyshiri","content":"<PGP Public key>"}
```

### Installation
- 프로그램 다운로드 및 Docker 설치
```
$ git clone https://github.com/KAIST-IS521/2018s-vuln-2dvorak.git
$ curl -fsSL https://get.docker.com/ | sudo sh
```
- Put [Your ID].key, [Your ID].pub in testkey directory
```
$ cd 2018s-vuln-2dvorak/testkey  
$ gpg --armor --export [githubID] > [githubID].pub
$ gpg --export-secret-keys -a [githubID] > [githubID].key
$ cd ..
```
- Docker 설정
```
# ./AutoDocker.sh
```

### Usage
SEHer 메신저
- startMessenger.sh 실행 후 ID와 passphrase 입력
- 디버깅을 위한 testkey pw 제공 : githubA/githubAPW, githubB/githubBPW ... 
```
#./startMessenger.sh
Your Github ID :[YOUR id]
Your passphrase :[YOUR passphrase]
  _____  ______  _    _             
 / ____||  ____|| |  | |            
| (___  | |__   | |__| |  ___  _ __ 
 \___ \ |  __|  |  __  | / _ \| '__|
 ____) || |____ | |  | ||  __/| |   
|_____/ |______||_|  |_| \___||_|   
1. List
2. Talk
3. Exit
```
snipHer 스니퍼
- https://github.com/KAIST-IS521/2018s-vuln-2dvorak/exploit

### Environment
- Git issue와 milestone을 사용하여 계획적인 프로젝트 진행
- JSON을 사용하여 빠르고 편한 파싱 및 헤더 구성
- ncurses를 이용한 보기 좋은 TUI환경 구성
- Qt Creator를 사용하여 Makefile의 자동 생성 및 편한 개발환경 구축

### Reference
 - https://github.com/nlohmann/json
 - https://github.com/KAIST-IS521/2018s-onion-team3
