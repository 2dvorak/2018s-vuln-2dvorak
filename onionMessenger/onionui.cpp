
#include "common.h"
#include "onionui.h"
#include "message.h"

using namespace newmsg;
int curX = 0, curY = 0;
string screen = "";

namespace oniui{
    OnionUI::OnionUI(){}
    OnionUI::~OnionUI(){}

    static char *onionlogo = (char *)"\
██╗  ██╗███████╗██████╗ \n\
██║  ██║██╔════╝██╔══██╗\n\
███████║█████╗  ██████╔╝\n\
██╔══██║██╔══╝  ██╔══██╗\n\
██║  ██║███████╗██║  ██║\n\
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝";

    static char *onionmemu = (char *)"\n1. List\n2. Talk\n3. Exit\n> ";

    void OnionUI::MainUI(){
        OnionUI *ui = new OnionUI();
        ui->Init();
        char value = 0;
        while(1){
            ui->ShowMenu();
            cin >> value;
            switch(value){
            case '1':
            {

                if(qRecvMsg.empty() == 0) {
                    string str = qRecvMsg.front();
                    cout << (new Message(str))->getContent() << "\n";
                    qRecvMsg.pop();
                }
            }
            break;
            case '2':
            {
                int maxX = 0, maxY = 0;
                screen = "";
                initscr();
                raw();
                getmaxyx(stdscr, maxY, maxX);
                scrollok(stdscr, true);
                std::thread t1(OnionUI::UIRecvThread, maxY, maxX);
                std::thread t2(OnionUI::UISendThread, maxY, maxX);
                t1.join();
                if(t2.joinable()) t2.join();
                endwin();
                ui->ShowMenu();
            }
            break;
            case '3':
            {
                exit(1);
            }
            break;
            default:
                continue;
            }
        }
    }

    void OnionUI::UIRecvThread(int maxY, int maxX) {
        while(1) {
            if(qRecvMsg.empty() == 0) {
                string str = qRecvMsg.front();
                if(str.compare(0, 5, "/exit", 0, 5) == 0) {
                    //endwin();
                    break;
                }
                screen.append("You: " + str + "\n");
                clear();
                mvprintw(0, 0, screen.c_str());
                mvprintw(maxY - 1, 0, ">");
                refresh();
                qRecvMsg.pop();
            }
        }
    }

    void OnionUI::UISendThread(int maxY, int maxX) {
        //string str;
        while(1) {
            string str;
            mvprintw(maxY - 1, 0, ">");
            for(int i = 0 ; i < maxX - 2 ; i++) {
                addch(' ');
            }
            move(maxY - 1, 1);
            getstr(&str[0]);
            string str2(str.c_str());
            screen.append("Me: " + str2 + "\n");
            clear();
            mvprintw(0, 0, screen.c_str());
            refresh();
            Message *msg = new Message(0,true,"192.168.0.1",string(str.c_str()));
            qSendMsg.push(msg->getJason().dump());
        }
    }

    void OnionUI::Init(){
        cout << onionlogo;
    }

    void OnionUI::ShowLogo(){
        cout << onionlogo;
    }

    void OnionUI::ShowMenu(){
        cout << onionmemu;
    }

}
