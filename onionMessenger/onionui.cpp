
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
 ██████╗ ███╗   ██╗██╗ ██████╗ ███╗   ██╗███╗   ███╗███████╗███████╗███████╗███████╗███╗   ██╗ ██████╗ ███████╗██████╗ \n\
██╔═══██╗████╗  ██║██║██╔═══██╗████╗  ██║████╗ ████║██╔════╝██╔════╝██╔════╝██╔════╝████╗  ██║██╔════╝ ██╔════╝██╔══██╗\n\
██║   ██║██╔██╗ ██║██║██║   ██║██╔██╗ ██║██╔████╔██║█████╗  ███████╗███████╗█████╗  ██╔██╗ ██║██║  ███╗█████╗  ██████╔╝\n\
██║   ██║██║╚██╗██║██║██║   ██║██║╚██╗██║██║╚██╔╝██║██╔══╝  ╚════██║╚════██║██╔══╝  ██║╚██╗██║██║   ██║██╔══╝  ██╔══██╗\n\
╚██████╔╝██║ ╚████║██║╚██████╔╝██║ ╚████║██║ ╚═╝ ██║███████╗███████║███████║███████╗██║ ╚████║╚██████╔╝███████╗██║  ██║\n\
 ╚═════╝ ╚═╝  ╚═══╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝╚══════╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝";

    static char *onionmemu = (char *)"\n1. List\n2. Talk\n3. Exit\n> ";

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

    void OnionUI::UIRecvThread(int maxY, int maxX) {
        while(1) {
            if(qRecvMsg.empty() == 0) {
                string str = (new Message(qRecvMsg.front()))->getContent();
                if(str.compare(0, 5, "/exit", 0, 5) == 0) {
                    //endwin();
                    break;
                }
                screen.append("You: " + str + "\n");
                clear();
                mvprintw(0, 0, screen.c_str());
                mvprintw(maxY - 1, 0, ">");
                /*curY++;
                if(maxY < curY) {
                    scrl(-1);
                    curY--;
                }*/
                refresh();
                //cout << str << "\n";
                /*mvprintw(0, 0, "You: ");
                printw(str.c_str());
                move(maxY - 1, 1);*/
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
            //cout << ">";
            //getline(cin, str);
            getstr(&str[0]);
            string str2(str.c_str());
            screen.append("Me: " + str2 + "\n");
            clear();
            mvprintw(0, 0, screen.c_str());
            /*curY++;
            if(maxY < curY) {
                scrl(1);
                curY--;
            }*/
            refresh();
            //cout << str << endl;

            /*mvprintw(1, 0, "Me: ");
            printw(str.c_str());
            move(maxY - 1, 1);*/
            if(str2.compare("/exit") == 0) {
                qSendMsg.push("{\"id\":1,\"bullian\":true,\"IP\":\"192.168.0.1\",\"content\":\"/exit\"}");
                break;
            }
            Message *msg = new Message(0,true,"192.168.0.1",string(str.c_str()));
            qSendMsg.push(msg->getJason().dump());
        }
    }

    void OnionUI::ShowLogo(){
        cout << onionlogo;
    }

    void OnionUI::ShowMenu(){
        cout << onionmemu;
    }

}

