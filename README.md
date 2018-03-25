# Onion Messenger - Team3 HER

Onion Messenger - Team3 HER

### Abstract
- PGP를 이용한 암호화 메신저

### Installation
```sh
# ./AutoDocker.sh
```

### Usages
```sh
# ./startMessenger.sh
# ./onionMessenger/onionMessenger [githubID] [PassPhrase]
Done!
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

- githubB user
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
|  SeongIl Wi     |    TA    | |
|  DongHyeon Oh   |   팀장   | |
|  SeungYeop Lee  |   팀원   | |
|  NakJun Choi    |   팀원   | |
|  SeJin Jeong    |   팀원   | |

### Insight

### Reference
 - https://github.com/nlohmann/json


