
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
                g_km->ShowList();
            }
            break;
            case '2':
            {
                cout << "=====================================" << endl;
                cout << "Who do you want to talk to?" << endl;
                g_km->ShowList();
                cout << "githubID> ";
                string githubID;
                cin >> githubID; // validation check routine ++++

                int maxX = 0, maxY = 0;
                screen = "";
                initscr();
                raw();
                getmaxyx(stdscr, maxY, maxX);
                scrollok(stdscr, true);
                std::thread t1(OnionUI::UIRecvThread, githubID, maxY, maxX);
                std::thread t2(OnionUI::UISendThread, githubID, maxY, maxX);
                t1.join();
                if(t2.joinable()) t2.join();
                endwin();
                ui->ShowMenu();
            }
            break;
            case '3':
            {
                g_km->SendKeyDie();
                cout << "Good Bye!" <<endl;
                sleep(3);
                exit(1);
            }
            break;
            default:
                continue;
            }
        }
    }

    void OnionUI::UIRecvThread(string githubID, int maxY, int maxX) {
        Message *msg = new Message();
        while(1){
            if(qRecvMsg.empty() == 0) {
                string str = qRecvMsg.front();
                json tmp;
                tmp = json::parse(str);
                string tmp_content = tmp.at("content").get<std::string>();
                string tmp_ip = tmp.at("sendip").get<std::string>();
                string tmp_githubID = g_km->FindgithubID(tmp_ip);
                if(tmp_githubID.compare(githubID) == 0){
                    screen.append("You: " + tmp_content + "\n");
                    clear();
                    mvprintw(0, 0, screen.c_str());
                    mvprintw(maxY - 1, 0, ">");
                    refresh();
                    qRecvMsg.pop();
                }
                else{
                    continue;
                }
            }
        }
    }

    void OnionUI::UISendThread(string githubID, int maxY, int maxX) {
        Message *msg = new Message();
        while(1) {
            string str;
            mvprintw(maxY - 1, 0, ">");
            for(int i = 0 ; i < maxX - 2 ; i++) {
                addch(' ');
            }
            move(maxY - 1, 1);
            getstr(&str[0]);
            string str2(str.c_str());
            if(str2.compare(0, 5, "/exit", 0, 5) == 0) {
                //endwin();
                break;
            }
            screen.append("Me: " + str2 + "\n");
            clear();
            mvprintw(0, 0, screen.c_str());
            refresh();
            string tmp_ip = g_km->Findip(githubID);
            msg->SetMessage(githubID, tmp_ip, str2);
            msg->SendMessage();
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
