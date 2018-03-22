
#include "common.h"
#include "onionui.h"
#include "message.h"

using namespace newmsg;

namespace oniui{
    OnionUI::OnionUI(){}
    OnionUI::~OnionUI(){}

    static char *onionlogo = (char *)"\
 ██████╗ ███╗   ██╗██╗ ██████╗ ███╗   ██╗███╗   ███╗███████╗███████╗███████╗███████╗███╗   ██╗ ██████╗ ███████╗██████╗ \n\
██╔═══██╗████╗  ██║██║██╔═══██╗████╗  ██║████╗ ████║██╔════╝██╔════╝██╔════╝██╔════╝████╗  ██║██╔════╝ ██╔════╝██╔══██╗\n\
██║   ██║██╔██╗ ██║██║██║   ██║██╔██╗ ██║██╔████╔██║█████╗  ███████╗███████╗█████╗  ██╔██╗ ██║██║  ███╗█████╗  ██████╔╝\n\
██║   ██║██║╚██╗██║██║██║   ██║██║╚██╗██║██║╚██╔╝██║██╔══╝  ╚════██║╚════██║██╔══╝  ██║╚██╗██║██║   ██║██╔══╝  ██╔══██╗\n\
╚██████╔╝██║ ╚████║██║╚██████╔╝██║ ╚████║██║ ╚═╝ ██║███████╗███████║███████║███████╗██║ ╚████║╚██████╔╝███████╗██║  ██║\n\
 ╚═════╝ ╚═╝  ╚═══╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝╚══════╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝";

    static char *onionmemu = (char *)"\\n1. List\n2. Talk\n3. Exit\n> ";

    void OnionUI::MainUI(){
        OnionUI *ui = new OnionUI();
        ui->ShowLogo();
        int value = 0;
        while(1){
            ui->ShowMenu();
            // let's check if input is integer..
            // someCheckFunction();
            cin >> value;
            switch(value){
            case 1:
            {
                if(qRecvMsg.empty() == 0) {
                    string str = qRecvMsg.front();
                    cout << (new Message(str))->getContent() << "\n";
                    qRecvMsg.pop();
                }
            }
            break;
            case 2:
            {
                std::thread t1(OnionUI::UIRecvThread);
                std::thread t2(OnionUI::UISendThread);
                t1.join();
            }
            break;
            case 3:
            {
                exit(1);
            }
            break;
            default:
            continue;
            }
        }
    }

    void OnionUI::UIRecvThread() {
        while(1) {
            if(qRecvMsg.empty() == 0) {
                string str = (new Message(qRecvMsg.front()))->getContent();
                if(str.compare("/exit") == 0) {
                    break;
                }
                cout << str << "\n";
                qRecvMsg.pop();
            }
        }
    }

    void OnionUI::UISendThread() {
        //string str;
        while(1) {
            string str;
            printf(">");
            getline(cin, str);
            if(str.compare("/exit") == 0) {
                qSendMsg.push("{\"id\":1,\"bullian\":true,\"IP\":\"192.168.0.1\",\"content\":\"/exit\"}");
                break;
            }
            qSendMsg.push(str);
        }
    }

    void OnionUI::ShowLogo(){
        cout << onionlogo;
    }

    void OnionUI::ShowMenu(){
        cout << onionmemu;
    }

}

