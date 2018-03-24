
#include "common.h"
#include "onionui.h"
#include "message.h"

using namespace newmsg;
int curX = 0, curY = 0;
vector<string> msgList;

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
                //ui->ShowMenu();
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

    void OnionUI::UIRecvThread(WINDOW* win, int maxY, int maxX) {
        while(1) {
            if(qRecvMsg.empty() == 0) {
                string str = qRecvMsg.front();
                if(str.compare("/exit") == 0) {
                    break;
                }
                msgList.push_back("You: " + str + "\n");
                clear();
                mvprintw(0, 0, onionlogo);
                mvprintw(maxY - 1, 0, ">");
                refresh();
                qRecvMsg.pop();
            }
        }
    }

    void OnionUI::UISendThread(WINDOW *win, int maxY, int maxX) {
        string str = "";
        //keypad(win, true);
        mvwprintw(win, 0, 0, "He█r");
        k_mutex.lock();
        curY = 1;
        k_mutex.unlock();
        while(1) {
            //string str;
            mvwprintw(win, maxY - 1, 0, ">");
            wprintw(win, str.c_str());
            //getstr(&str[0]);
            wrefresh(win);
            int input = wgetch(win);
            //mvwprintw(win, 0, 5, "pressed : %d", input);
            //wmove(win, maxY -1, str.length() + 1);
            if(input >= 0x20 && input <= 0x7e) {
                str.push_back(input);
            } else if(input == KEY_UP) {
                k_mutex.lock();
                if(msgList.size() > 22 && curY > 23) {
                    curY--;
                }
                k_mutex.unlock();
            } else if(input == KEY_DOWN) {
                k_mutex.lock();
                if(msgList.size() > 22 && curY < msgList.size() + 1) {
                    curY++;
                }
                k_mutex.unlock();
            } else if(input == KEY_EXIT) {
                break; //return;
            } else if(input == 10) {
                k_mutex.lock();
                msgList.push_back("Me: " + str + "\n");
                curY = msgList.size() + 1;
                k_mutex.unlock();
                //Message *msg = new Message("1","true","127.0.0.1",str);
                //s_mutex.lock();
                //qSendMsg.push(str);
                //s_mutex.unlock();
                str = "";
            }
            wclear(win);
            wrefresh(win);
            mvwprintw(win, 0, 0, "Her");
            wmove(win, 1, 0);
            if(curY >= maxY - 1) {
                for( int i = curY - maxY + 1; i < curY - 1 ; i++ ) {
                    wprintw(win, msgList.at(i).c_str());
                    wrefresh(win);
                }
            } else {
                for( int i = 0 ; i < msgList.size(); i++ ) {
                    wprintw(win, msgList.at(i).c_str());
                }
            }
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
