
#include "common.h"
#include "onionui.h"
#include "message.h"

#define LISTWIN_HEIGHT  10
#define LISTWIN_WIDTH   30

#define CHATWIN_HEIGHT  80
#define CHATWIN_WIDTH   24

#define CHAT_DELIMETER  2   // length of ': ' used for dividing user id and message
#define CHAT_INPUTCHAR  1   // length of '>'
#define LOGO_HEIGHT     1   // height of logo 'Her'

using namespace newmsg;
unsigned int curX = 0, curY = 0;
unsigned int curInputLine = 1;
//vector<string> msgList;
string typing = "";
bool end_flag = true;

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

                WINDOW *listWin, *chatWin;
                string githubID;

                unsigned int maxX = 0, maxY = 0;
                int listX = 0, listY = 0;
                unsigned int highlight = 1;
                unsigned int choice = 0;
                int c;
                unsigned int listLen = nodeMap->size();
                initscr();
                clear();
                noecho();
                //cbreak();
                raw();
                getmaxyx(stdscr, maxY, maxX);

                listX = (maxX - LISTWIN_WIDTH) / 2;
                listY = (maxY - LISTWIN_HEIGHT) / 2;
                listWin = newwin(LISTWIN_HEIGHT, LISTWIN_WIDTH, listY, listX);
                keypad(listWin, true);
                curs_set(0);
                //scrollok(listWin, true);
                wrefresh(listWin);
                curY = LISTWIN_HEIGHT - 2;
                while(1) {
                    int x, y, i = 0;
                    x = 2;
                    y = 1;
                    wclear(listWin);
                    box(listWin, 0, 0);
                    nodeIter = nodeMap->begin();
                    if(curY > LISTWIN_HEIGHT - 2) {
                        for( unsigned int j = 0 ; j < curY - (LISTWIN_HEIGHT - 2); j ++) {
                            nodeIter++;
                        }
                    }
                    for(int j = 0; nodeIter != nodeMap->end() && j < (LISTWIN_HEIGHT - 2) ; j++)
                    {

                        if(highlight == j + 1 + (curY - (LISTWIN_HEIGHT - 2))) /* High light the present choice */
                        {	wattron(listWin, A_REVERSE);
                            mvwprintw(listWin, y, x, "%s", string(nodeIter->first).c_str());
                            githubID = string(nodeIter->first);
                            wattroff(listWin, A_REVERSE);
                        }
                        else
                            mvwprintw(listWin, y, x, "%s", string(nodeIter->first).c_str());
                        i++;
                        ++y;
                        nodeIter++;
                    }
                    wrefresh(listWin);
                    if(choice != 0) break;
                    c = wgetch(listWin);
                    switch(c)
                    {
                    case KEY_UP:
                    {
                        if(highlight == 1) {
                            highlight = listLen;
                            if(listLen > LISTWIN_HEIGHT - 2) {
                                curY = listLen;
                            }
                        } else {
                            highlight--;
                        }
                        if(nodeMap->size() > (LISTWIN_HEIGHT - 2) && curY - (LISTWIN_HEIGHT - 2) > highlight ) {
                            curY--;
                        }
                        continue;
                    }
                    case KEY_DOWN:
                    {
                        if(highlight == listLen) {
                            highlight = 1;
                            curY = (LISTWIN_HEIGHT - 2);
                        } else {
                            highlight++;
                        }
                        if(nodeMap->size() > (LISTWIN_HEIGHT - 2) && curY < highlight ) {
                            curY++;
                        }
                        continue;
                    }
                    case 10:
                    {
                        choice = highlight;
                        break;
                    }
                    case 27:
                    {
                        choice = listLen + 1;
                        break;
                    }
                    default:
                    {
                        refresh();
                        continue;
                    }
                    }
                    break;
                }
                wclear(listWin);
                delwin(listWin);
                if(choice <= listLen) {
                    chatWin = newwin(maxY, maxX, 0, 0);
                    curs_set(1);
                    keypad(chatWin, true);
                    noecho();
                    wrefresh(chatWin);
                    end_flag = true;
                    std::thread t1(OnionUI::UIRecvThread, chatWin, githubID, maxY, maxX);
                    std::thread t2(OnionUI::UISendThread, chatWin, githubID, maxY, maxX);
                    t1.join();
                    if(t2.joinable()) t2.join();
                    wclear(chatWin);
                    wrefresh(chatWin);
                    delwin(chatWin);
                }
                endwin();
                //ui->ShowMenu();
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

    void OnionUI::PrintChat(WINDOW* win, unsigned int maxY, unsigned int maxX) {
        k_mutex.lock();
        //curY += (tmp_githubID.length() + tmp_content.length() + CHAT_DELIMETER) / maxX;
        wclear(win);
        vector<string>* msgList = get<0>(chatRoomIter->second);
        mvwprintw(win, 0, 0, "Her");
        wmove(win, 1, 0);
        if(curY + 1 > maxY - LOGO_HEIGHT - curInputLine) {
            for( unsigned int i =  curY; i > curY - maxY - curInputLine - LOGO_HEIGHT ; i-- ) {
                if( msgList->at(i).length() < maxX) {
                    mvwprintw(win, i+1, 0, msgList->at(i).c_str());
                } else if( msgList->at(i).length() == maxX ) {
                    // no need to divde these cases?
                    mvwprintw(win, i+1, 0, msgList->at(i).c_str());
                } else {
                    unsigned int lines = msgList->at(i).length() / maxX;
                    if( msgList->at(i).length() % maxX > 0) lines++;
                    mvwprintw(win, i+1, 0, msgList->at(i).substr(lines*maxX).c_str());
                    //unsigned int index = maxX - 1;
                    for(; lines > 1; lines--) {
                        mvwprintw(win, i+1, 0, msgList->at(i).substr(lines*maxX,maxX).c_str());
                        i--;
                    }
                }
            }
        } else {
            for( unsigned int i = 0 ; i < msgList->size(); i++ ) {
                wprintw(win, msgList->at(i).c_str());
            }
        }
        mvwprintw(win, maxY - curInputLine, 0, ">");
        wprintw(win, typing.c_str());
        wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX < maxX )? curX + CHAT_INPUTCHAR : curX % maxX + 1);
        wrefresh(win);
        k_mutex.unlock();
    }

    void OnionUI::UIRecvThread(WINDOW* win, string githubID, unsigned int maxY, unsigned int maxX) {
        while(end_flag){
            k_mutex.lock();
            chatRoomIter = chatRoomMap->find(githubID);
            k_mutex.unlock();
            std::unique_lock<std::mutex> lck(r_mutex);
            r_cv.wait(lck, RecvAvailable);
            if(!end_flag) {
                break;
            }
            k_mutex.lock();
            curY = get<0>(chatRoomIter->second)->size() - 1;
            PrintChat(win, maxY, maxX);
            /*string str = qRecvMsg.front();
            json tmp;
            tmp = json::parse(str);
            string tmp_content = tmp.at("content").get<std::string>();
            string tmp_ip = tmp.at("sendip").get<std::string>();
            // WHY NOT tmp.at("githubID")???
            string tmp_githubID = g_km->FindgithubID(tmp_ip);
            if(tmp_githubID.compare(githubID) == 0){
                k_mutex.lock();
                if( tmp_githubID.length() + tmp_content.length() + CHAT_DELIMETER < maxX) {
                    msgList.push_back(tmp_githubID + ": " + tmp_content + "\n");
                } else if( tmp_githubID.length() + tmp_content.length() + CHAT_DELIMETER == maxX ) {
                    msgList.push_back(tmp_githubID + ": " + tmp_content);
                } else {
                    int iter = ( tmp_githubID.length() + tmp_content.length() + CHAT_DELIMETER) / maxX;
                    int index = maxX - tmp_githubID.length() - CHAT_DELIMETER - 1;
                    msgList.push_back(tmp_githubID + ": " + tmp_content.substr(0, index));
                    for(int i = 0; i < iter - 1; i++) {
                        msgList.push_back(tmp_content.substr(index,maxX));
                        index += maxX;
                    }
                    msgList.push_back(tmp_content.substr(index) + "\n");
                }
                curY = msgList.size() + 1;
                //curY += (tmp_githubID.length() + tmp_content.length() + CHAT_DELIMETER) / maxX;
                wclear(win);
                mvwprintw(win, 0, 0, "Her");
                wmove(win, 1, 0);
                if(curY >= maxY - 1) {
                    for( unsigned int i = curY - maxY + 1; i < curY - 1 ; i++ ) {
                        wprintw(win, msgList.at(i).c_str());
                    }
                } else {
                    for( unsigned int i = 0 ; i < msgList.size(); i++ ) {
                        wprintw(win, msgList.at(i).c_str());
                    }
                }
                mvwprintw(win, maxY - curInputLine, 0, ">");
                wprintw(win, typing.c_str());
                wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX < maxX )? curX + CHAT_INPUTCHAR : curX % maxX + 1);
                wrefresh(win);
                k_mutex.unlock();

                r_mutex.lock();
                qRecvMsg.pop();
                r_mutex.unlock();
            }
            else{
                continue;
            }*/

        }
        wclear(win);
    }


    void OnionUI::UISendThread(WINDOW *win, string githubID,unsigned int maxY, unsigned int maxX) {
        Message *msg = new Message();
        vector<string>* msgList = get<0>(chatRoomIter->second);
        //string str = "";
        k_mutex.lock();
        curY = 1;
        curX = 0;
        curInputLine = 1;
        k_mutex.unlock();
        while(1) {
            k_mutex.lock();
            mvwprintw(win, 0, 0, "Her");
            mvwprintw(win, maxY - curInputLine, 0, ">");
            wprintw(win, typing.c_str());
            wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX < maxX )? curX + CHAT_INPUTCHAR : curX % maxX + 1);
            wrefresh(win);
            k_mutex.unlock();
            int input = wgetch(win);
            if(input >= 0x20 && input <= 0x7e) {
                typing.insert(curX, 1, input);
                k_mutex.lock();
                curInputLine = 1 + (typing.length() + CHAT_INPUTCHAR) / maxX;
                curX++;
                k_mutex.unlock();
            } else if(input == KEY_UP) {
                k_mutex.lock();
                if(msgList->size() > (maxY - 1 - LOGO_HEIGHT) && curY > (maxY - LOGO_HEIGHT)) {
                    curY--;
                }
                k_mutex.unlock();
            } else if(input == KEY_DOWN) {
                k_mutex.lock();
                if(msgList->size() > (maxY - 1 - LOGO_HEIGHT) && curY < msgList->size() + 1) {
                    curY++;
                }
                k_mutex.unlock();
            } else if(input == 27) {
                k_mutex.lock();
                end_flag = false;
                wclear(win);
                k_mutex.unlock();
                break; //return;
            } else if(input == 10) {
                k_mutex.lock();
                if( string("Me").length() + typing.length() + CHAT_DELIMETER < maxX) {
                    msgList->push_back("Me: " + typing + "\n");
                } else if(string("Me").length() + typing.length() + CHAT_DELIMETER == maxX) {
                    msgList->push_back("Me: " + typing );
                } else {
                    int iter = ( string("Me").length() + typing.length() + CHAT_DELIMETER) / maxX;
                    int index = maxX - string("Me").length() - CHAT_DELIMETER - 1;
                    msgList->push_back("Me: " + typing.substr(0, index));
                    for(int i = 0; i < iter - 1; i++) {
                        msgList->push_back(typing.substr(index,maxX));
                        index += maxX;
                    }
                    msgList->push_back(typing.substr(index) + "\n");
                }
                curY = msgList->size() + 1;
                //curY += ( string("Me").length() + typing.length() + CHAT_DELIMETER ) / maxX;
                curX = 0;
                curInputLine = 1;
                k_mutex.unlock();
                string tmp_ip = g_km->Findip(githubID);
                msg->SetMessage(githubID, tmp_ip, typing);
                msg->EncMessage(githubID);
                msg->SendMessage();
                typing = "";
            } else if(input == KEY_BACKSPACE) {
                if(typing.length() > 0 && curX > 0) {
                    typing.erase(curX - 1, 1);
                    k_mutex.lock();
                    curInputLine = 1 + (typing.length() + CHAT_INPUTCHAR) / maxX;
                    curX--;
                    k_mutex.unlock();
                }
            } else if(input == KEY_LEFT) {
                if(curX > 0) {
                    k_mutex.lock();
                    curX--;
                    k_mutex.unlock();
                }
            } else if(input == KEY_RIGHT) {
                if(curX < typing.length()) {
                    k_mutex.lock();
                    curX++;
                    k_mutex.unlock();
                }
            } else if(input == 330) {
                if(curX < typing.length()) {
                    k_mutex.lock();
                    typing.erase(curX, 1);
                    curInputLine = 1 + (typing.length() + CHAT_INPUTCHAR) / maxX;
                    k_mutex.unlock();
                }
            }
            k_mutex.lock();
            wclear(win);
            mvwprintw(win, 0, 5, "pressed : %d", input);
            wmove(win, 1, 0);
            if(curY >= maxY - curInputLine) {
                for( unsigned int i = curY - (maxY - curInputLine) ; i < curY - curInputLine ; i++ ) {
                    wprintw(win, msgList->at(i).c_str());
                    //i += (string("Me").length() + msgList.at(i).length() + CHAT_DELIMETER ) / maxX;
                }
            } else {
                for( unsigned int i = 0 ; i < msgList->size(); i++ ) {
                    wprintw(win, msgList->at(i).c_str());
                    //i += (string("Me").length() + msgList.at(i).length() + CHAT_DELIMETER ) / maxX;
                }
            }
            k_mutex.unlock();
        }
        k_mutex.lock();
        wclear(win);
        wrefresh(win);
        k_mutex.unlock();
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
