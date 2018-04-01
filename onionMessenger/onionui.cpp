
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
unsigned int curLineIndexUp = 0, curLineIndexDown = 0;
int curIndexUp = 0;
unsigned int curIndexDown = 0;
unsigned int curInputLine = 1;
string chat = "";
string typing = "";
bool end_flag = true;
/*struct strCmp {
    bool operator() (const string& lhs, const string& rhs) const {
        return lhs < rhs;
    }
};*/
bool strCmp(const string& rhs, const string& lhs) {
    return lhs < rhs;
}

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
                unsigned int listLen = chatRoomMap->size();
                initscr();
                clear();
                noecho();
                //cbreak();
                raw();
                getmaxyx(stdscr, maxY, maxX);
                //start_color();
                //init_pair(1, COLOR_BLACK, COLOR_CYAN);
                //init_pair(2, COLOR_CYAN, COLOR_BLACK);
                listX = (maxX - LISTWIN_WIDTH) / 2;
                listY = (maxY - LISTWIN_HEIGHT) / 2;
                listWin = newwin(LISTWIN_HEIGHT, LISTWIN_WIDTH, listY, listX);
                keypad(listWin, true);
                curs_set(0);
                //scrollok(listWin, true);
                wrefresh(listWin);
                curY = LISTWIN_HEIGHT - 2;
                std::set<string, bool(*)(const string &lhs, const string &rhs)> myGroup(&strCmp);
                while(1) {
                    int x, y, i = 0;
                    x = 2;
                    y = 1;
                    wclear(listWin);
                    box(listWin, 0, 0);

                    chatRoomIter = chatRoomMap->begin();
                    for(int j = 0; chatRoomIter != chatRoomMap->end() && j < (LISTWIN_HEIGHT - 2) ; j++)
                    {

                        if(highlight == j + 1 + (curY - (LISTWIN_HEIGHT - 2))) // Highlight the present choice
                        {

                            githubID = string(chatRoomIter->first);
                            if(myGroup.find(githubID) != myGroup.end()) {
                                mvwprintw(listWin, y, x - 1, "+");
                                wattron(listWin, A_REVERSE | A_UNDERLINE);
                                //mvwprintw(listWin, y, x - 1, " ");
                                if(githubID.length() > LISTWIN_WIDTH - 2 - 5 - 6 - 3 ) {
                                    mvwprintw(listWin, y, x, "%s..", githubID.substr(0,LISTWIN_WIDTH - 2 - 5 - 6 - 3 - 2).c_str());
                                } else {
                                    mvwprintw(listWin, y, x, "%s", githubID.c_str());
                                }
                                wprintw(listWin, " (%d) ", get<0>(*(chatRoomIter->second))->size() - get<1>(*(chatRoomIter->second)));
                                if(get<2>(*(chatRoomIter->second)) > 0) {
                                    string timeStr = string(ctime(&get<2>(*(chatRoomIter->second))));
                                    mvwprintw(listWin, y, LISTWIN_WIDTH - 7, timeStr.substr(11,5).c_str());
                                }
                                wattroff(listWin, A_REVERSE | A_UNDERLINE);
                            } else if(githubID.at(0) == '#') {
                                wattron(listWin, A_REVERSE);
                                mvwprintw(listWin, y, x, "[G]");
                                unsigned int width = 3;
                                set<string> tmpSet;
                                unsigned long start = 1, end = 0;
                                while((unsigned long)(end = githubID.find('#', start)) != string::npos) {
                                    string tmpGithubID = githubID.substr(start, end - start);
                                    if(tmpGithubID.compare(g_km->ReturnGithubID()) == 0) {
                                        start = end + 1;
                                        continue;
                                    }
                                    tmpSet.insert(tmpGithubID);
                                    start = end + 1;
                                }
                                set<string>::iterator it = tmpSet.begin();
                                wprintw(listWin, it->c_str());
                                width += it->length();
                                it++;
                                for(;it != tmpSet.end();it++) {
                                    if(width + it->length() + 2< LISTWIN_WIDTH - 2 - 5 - 6 - 3) {
                                        wprintw(listWin, ", ");
                                        wprintw(listWin, it->c_str());
                                        width += it->length() + 2;
                                    } else {
                                        wprintw(listWin, "..");
                                        break;
                                    }
                                }
                                wprintw(listWin, " (%d) ", get<0>(*(chatRoomIter->second))->size() - get<1>(*(chatRoomIter->second)));
                                if(get<2>(*(chatRoomIter->second)) > 0) {
                                    string timeStr = string(ctime(&get<2>(*(chatRoomIter->second))));
                                    mvwprintw(listWin, y, LISTWIN_WIDTH - 7, timeStr.substr(11,5).c_str());
                                }
                                wattroff(listWin, A_REVERSE);
                            } else {
                                wattron(listWin, A_REVERSE);
                                //mvwprintw(listWin, y, x - 1, " ");
                                if(githubID.length() > LISTWIN_WIDTH - 2 - 5 - 6 - 3) {
                                    mvwprintw(listWin, y, x, "%s..", githubID.substr(0,LISTWIN_WIDTH - 2 - 5 - 6 - 3 - 2).c_str());
                                } else {
                                    mvwprintw(listWin, y, x, "%s", githubID.c_str());
                                }
                                wprintw(listWin, " (%d) ", get<0>(*(chatRoomIter->second))->size() - get<1>(*(chatRoomIter->second)));
                                if(get<2>(*(chatRoomIter->second)) > 0) {
                                    string timeStr = string(ctime(&get<2>(*(chatRoomIter->second))));
                                    mvwprintw(listWin, y, LISTWIN_WIDTH - 7, timeStr.substr(11,5).c_str());
                                }
                                wattroff(listWin, A_REVERSE);
                            }
                        }
                        else {
                            string tmpGithubID = string(chatRoomIter->first);
                            if(myGroup.find(tmpGithubID) != myGroup.end()) {
                                mvwprintw(listWin, y, x - 1, "+");
                                wattron(listWin,A_UNDERLINE);
                                if(tmpGithubID.length() > LISTWIN_WIDTH - 2 - 5 - 6 - 3) {
                                    mvwprintw(listWin, y, x, "%s..", tmpGithubID.substr(0,LISTWIN_WIDTH - 2 - 5 - 6 - 3 - 2).c_str());
                                } else {
                                    mvwprintw(listWin, y, x, "%s", tmpGithubID.c_str());
                                }
                                wprintw(listWin, " (%d) ", get<0>(*(chatRoomIter->second))->size() - get<1>(*(chatRoomIter->second)));
                                if(get<2>(*(chatRoomIter->second)) > 0) {
                                    string timeStr = string(ctime(&get<2>(*(chatRoomIter->second))));
                                    mvwprintw(listWin, y, LISTWIN_WIDTH - 7, timeStr.substr(11,5).c_str());
                                }
                                wattroff(listWin, A_UNDERLINE);
                            } else if(tmpGithubID.at(0) == '#') {
                                mvwprintw(listWin, y, x, "[G]");
                                unsigned int width = 3;
                                set<string> tmpSet;
                                unsigned long start = 1, end = 0;
                                while((unsigned long)(end = tmpGithubID.find('#', start)) != string::npos) {
                                    string tmpGithubID2 = tmpGithubID.substr(start, end - start);
                                    if(tmpGithubID2.compare(g_km->ReturnGithubID()) == 0) {
                                        start = end + 1;
                                        continue;
                                    }
                                    tmpSet.insert(tmpGithubID2);
                                    start = end + 1;
                                }
                                set<string>::iterator it = tmpSet.begin();
                                wprintw(listWin, it->c_str());
                                width += it->length();
                                it++;
                                for(;it != tmpSet.end();it++) {
                                    if(width + it->length() + 2< LISTWIN_WIDTH - 2 - 5 - 6 - 3) {
                                        wprintw(listWin, ", ");
                                        wprintw(listWin, it->c_str());
                                        width += it->length() + 2;
                                    } else {
                                        wprintw(listWin, "..");
                                        break;
                                    }
                                }
                                wprintw(listWin, " (%d) ", get<0>(*(chatRoomIter->second))->size() - get<1>(*(chatRoomIter->second)));
                                if(get<2>(*(chatRoomIter->second)) > 0) {
                                    string timeStr = string(ctime(&get<2>(*(chatRoomIter->second))));
                                    mvwprintw(listWin, y, LISTWIN_WIDTH - 7, timeStr.substr(11,5).c_str());
                                }
                            }else {
                                if(tmpGithubID.length() > LISTWIN_WIDTH - 2 - 5 - 6 - 3) {
                                    mvwprintw(listWin, y, x, "%s..", tmpGithubID.substr(0,LISTWIN_WIDTH - 2 - 5 - 6 - 3 - 2).c_str());
                                } else {
                                    mvwprintw(listWin, y, x, "%s", tmpGithubID.c_str());
                                }
                                wprintw(listWin, " (%d) ", get<0>(*(chatRoomIter->second))->size() - get<1>(*(chatRoomIter->second)));
                                if(get<2>(*(chatRoomIter->second)) > 0) {
                                    string timeStr = string(ctime(&get<2>(*(chatRoomIter->second))));
                                    mvwprintw(listWin, y, LISTWIN_WIDTH - 7, timeStr.substr(11,5).c_str());
                                }
                            }

                        }
                        i++;
                        ++y;
                        chatRoomIter++;
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
                    case '+':
                    {
                        if(githubID.at(0) == '#') continue;
                        if(myGroup.find(githubID) != myGroup.end()) {
                            myGroup.erase(githubID);
                        } else {
                            myGroup.insert(githubID);
                        }
                        continue;
                    }
                    case 330:
                    {
                        chatRoomIter = chatRoomMap->find(githubID);
                        if(chatRoomIter != chatRoomMap->end()) {
                            delete(get<0>(*(chatRoomIter->second)));
                            delete(chatRoomIter->second);
                            chatRoomMap->erase(githubID);
                        }
                        continue;
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
                    if(myGroup.size() >0) {
                        string groupChatID = "#";
                        myGroup.insert(g_km->ReturnGithubID());
                        std::set<string, bool(*)(const string &lhs, const string &rhs)>::iterator it = myGroup.begin();
                        for(;it != myGroup.end(); it++) {
                            groupChatID.append(*it);
                            groupChatID.append("#");
                        }
                        groupChatID.append("--");
                        std::thread t1(OnionUI::UIRecvThread, chatWin, groupChatID, maxY, maxX);
                        std::thread t2(OnionUI::UISendThread, chatWin, groupChatID, maxY, maxX);
                        t1.join();
                        if(t2.joinable()) t2.join();
                    } else {
                        std::thread t1(OnionUI::UIRecvThread, chatWin, githubID, maxY, maxX);
                        std::thread t2(OnionUI::UISendThread, chatWin, githubID, maxY, maxX);
                        t1.join();
                        if(t2.joinable()) t2.join();
                    }
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

    void OnionUI::PrintChat(WINDOW* win, string githubID, unsigned int maxY, unsigned int maxX) {
        k_mutex.lock();
        chatRoomIter = chatRoomMap->find(githubID);
        /*if(chatRoomIter == chatRoomMap->end()) {
            k_mutex.unlock();
            return;
        }*/
        vector<string>* msgList = get<0>(*(chatRoomIter->second));

        if(get<1>(*(chatRoomIter->second)) < msgList->size() || curIndexUp == -2) { // recv thread recved or enter key(send) -> update screen
            chat = "";
            int index = msgList->size() - 1;
            while(chat.length() < maxX * (maxY - curInputLine - LOGO_HEIGHT) && index > -1) {
                unsigned int length = msgList->at(index).length();
                string line = "";
                if( length <= maxX) {
                    line = msgList->at(index);
                    for(unsigned int i = 0 ; i < maxX - length; i++) {
                        line.append(" ");
                    }
                    chat = line + chat;
                    curLineIndexUp = 0;
                } else {
                    curLineIndexUp = length / maxX;
                    if( length % maxX == 0) curLineIndexUp--;
                    line = msgList->at(index).substr(curLineIndexUp*maxX);
                    for(unsigned int i = 0 ; i < maxX - (length % maxX); i++) {
                        line.append(" ");
                    }
                    chat = line + chat;
                    //if(chat.length() == maxX * (maxY - curInputLine - LOGO_HEIGHT)) break; // redundant
                    while(curLineIndexUp > 0 && chat.length() < maxX * (maxY - curInputLine - LOGO_HEIGHT)) {
                        curLineIndexUp--;
                        chat = msgList->at(index).substr(curLineIndexUp*maxX,maxX) + chat;
                    }
                }
                index--;

            }
            curIndexUp = index;
            curIndexDown = msgList->size();
            curLineIndexDown = 0;
            get<1>(*(chatRoomIter->second)) = msgList->size();
        } else {
            if(chat.length() > maxX * (maxY - curInputLine - LOGO_HEIGHT)) {
                mvwprintw(win, 0, 50, "[DEBUG]chat length exceed by : %d", chat.length() - maxX * (maxY - curInputLine - LOGO_HEIGHT));
            }
        }

        wclear(win);
        if(githubID.at(0) == '#') {
            mvwprintw(win, 0, 0, "GroupChat with ");
            set<string> tmpSet;
            unsigned int width = 0;
            unsigned long start = 1, end = 0;
            while((unsigned long)(end = githubID.find('#', start)) != string::npos) {
                string tmpGithubID = githubID.substr(start, end - start);
                if(tmpGithubID.compare(g_km->ReturnGithubID()) == 0) {
                    start = end + 1;
                    continue;
                }
                tmpSet.insert(tmpGithubID);
                start = end + 1;
            }
            set<string>::iterator it = tmpSet.begin();
            wprintw(win, it->c_str());
            width += it->length();
            it++;
            for(;it != tmpSet.end();it++) {
                if(width + it->length() + 2< maxX) {
                    wprintw(win, ", ");
                    wprintw(win, it->c_str());
                    width += it->length() + 2;
                } else {
                    wprintw(win, "..");
                    break;
                }
            }
        } else {
            mvwprintw(win, 0, 0, "ChatRoom with ");
            wprintw(win, githubID.c_str());
        }
        mvwprintw(win, LOGO_HEIGHT, 0, chat.c_str());
        mvwprintw(win, maxY - curInputLine, 0, ">");
        wprintw(win, typing.c_str());
        wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX + CHAT_INPUTCHAR< maxX )? curX + CHAT_INPUTCHAR : (curX + CHAT_INPUTCHAR) % maxX);
        wrefresh(win);
        k_mutex.unlock();
    }

    void OnionUI::UIRecvThread(WINDOW* win, string githubID, unsigned int maxY, unsigned int maxX) {
        while(end_flag){
            chatRoomIter = chatRoomMap->find(githubID);
            if(chatRoomIter == chatRoomMap->end()) {
                r_mutex.lock();
                vector<string>* newChatRoom = new std::vector<string>();
                chatRoomMap->insert(chatRoomMap->end(), pair<string, tuple<vector<string>*,unsigned int,time_t>*>(githubID, new tuple<vector<string>*,unsigned int,time_t>(newChatRoom, 0, 0)));
                r_mutex.unlock();
            }
            chatRoomIter = chatRoomMap->find(githubID);
            // I'm not good at threads....
            //std::unique_lock<std::mutex> lck(r_mutex);
            //r_cv.wait(lck);
            while(end_flag) {
                r_mutex.lock();
                if(RecvAvailable()) {
                    r_mutex.unlock();
                    break;
                }
                r_mutex.unlock();
            }
            if(!end_flag) {
                break;
            }
            //curY = get<0>(chatRoomIter->second)->size() - 1;
            PrintChat(win, githubID, maxY, maxX);
        }
        wclear(win);
    }


    void OnionUI::UISendThread(WINDOW *win, string githubID,unsigned int maxY, unsigned int maxX) {
        Message *msg = new Message();
        chatRoomIter = chatRoomMap->find(githubID);
        if(chatRoomIter == chatRoomMap->end()) {
            r_mutex.lock();
            vector<string>* newChatRoom = new std::vector<string>();
            chatRoomMap->insert(chatRoomMap->end(),pair<string, tuple<vector<string>*,unsigned int,time_t>*>(githubID, new tuple<vector<string>*,unsigned int,time_t>(newChatRoom, 0, 0)));
            r_mutex.unlock();
        }
        chatRoomIter = chatRoomMap->find(githubID);
        vector<string>* msgList = get<0>(*(chatRoomIter->second));
        //string str = "";
        k_mutex.lock();
        curY = 1;
        curX = 0;
        curInputLine = 1;
        curIndexUp = -2;
        //wclear(win);
        //wrefresh(win);
        k_mutex.unlock();
        PrintChat(win, githubID, maxY, maxX);
        while(1) {
            int input = wgetch(win);
            if(input >= 0x20 && input <= 0x7e) {
                if(curX == maxX - 1 && maxY == curInputLine + LOGO_HEIGHT) {
                    continue;
                }
                typing.insert(curX, 1, input);
                k_mutex.lock();
                curInputLine = 1 + (typing.length() + CHAT_INPUTCHAR) / maxX;
                curX++;
                k_mutex.unlock();
                PrintChat(win, githubID, maxY, maxX);
            } else if(input == KEY_UP) {
                k_mutex.lock();
                if(curLineIndexUp > 0) {
                    curLineIndexUp--;
                    chat = msgList->at(curIndexUp + 1).substr(curLineIndexUp*maxX,maxX) + chat.substr(0,maxX*(maxY - LOGO_HEIGHT - curInputLine - 1));
                    if(curLineIndexDown == msgList->at(curIndexDown - 1).length() / maxX - (msgList->at(curIndexDown - 1).length() % maxX == 0?1:0)) {
                        curLineIndexDown = 0;
                        curIndexDown--;
                    } else {
                        curLineIndexDown++;
                    }
                } else if(curIndexUp > -1) {
                    unsigned int length = msgList->at(curIndexUp).length();
                    if(length > maxX) {
                        if(length % maxX == 0) {
                            curLineIndexUp = length / maxX - 1;
                            chat = msgList->at(curIndexUp).substr(curLineIndexUp*maxX, maxX) + chat.substr(0, maxX*(maxY - LOGO_HEIGHT - curInputLine - 1));
                        } else {
                            curLineIndexUp = length / maxX;
                            string line = msgList->at(curIndexUp).substr(curLineIndexUp*maxX, maxX);
                            for(unsigned int i = 0 ; i < maxX - (length % maxX) ; i++) {
                                line.append(" ");
                            }
                            chat = line + chat.substr(0, maxX*(maxY - LOGO_HEIGHT - curInputLine - 1));;
                        }
                        curIndexUp--;
                    } else {
                        string line = msgList->at(curIndexUp);
                        for(unsigned int i = 0 ; i < maxX - length ; i++) {
                            line.append(" ");
                        }
                        chat = line + chat.substr(0, maxX*(maxY - LOGO_HEIGHT - curInputLine - 1));
                        curLineIndexUp = 0;
                        curIndexUp--;
                    }
                    if(msgList->at(curIndexDown - 1).length() > maxX) {
                        if(curLineIndexDown == msgList->at(curIndexDown - 1).length() / maxX - (msgList->at(curIndexDown - 1).length() % maxX == 0?1:0)) {
                            curLineIndexDown = 0;
                            curIndexDown--;
                        } else {
                            curLineIndexDown++;
                        }
                    } else {
                        curLineIndexDown = 0;
                        curIndexDown--;
                    }
                }
                k_mutex.unlock();
                PrintChat(win, githubID, maxY, maxX);
            } else if(input == KEY_DOWN) {
                k_mutex.lock();
                if(curLineIndexDown > 0) {
                    curLineIndexDown--;
                    chat = chat.substr(maxX) + msgList->at(curIndexDown - 1).substr((msgList->at(curIndexDown - 1).length() / maxX - curLineIndexDown)*maxX);
                    if(curLineIndexDown == 0) {
                        for(unsigned int i = 0 ; i < maxX - (msgList->at(curIndexDown - 1).length() % maxX ) ; i++ ) {
                            chat.append(" ");
                        }
                    }
                    if(msgList->at(curIndexUp + 1).length() > maxX) {
                        if(curLineIndexUp == msgList->at(curIndexUp + 1).length() / maxX - (msgList->at(curIndexUp + 1).length() % maxX == 0?1:0)) {
                            curLineIndexUp = 0;
                            curIndexUp++;
                        } else {
                            curLineIndexUp++;
                        }
                    } else {
                        curLineIndexUp = 0;
                        curIndexUp++;
                    }
                } else if(curIndexDown < msgList->size()) {
                    unsigned int length = msgList->at(curIndexDown).length();
                    if(length > maxX) {
                        curLineIndexDown = length / maxX - (length % maxX == 0?1:0);
                        chat = chat.substr(maxX) + msgList->at(curIndexDown).substr(0, maxX);
                        curIndexDown++;
                    } else {
                        curLineIndexDown = 0;
                        string line = msgList->at(curIndexDown);
                        for(unsigned int i = 0 ; i < maxX - length ; i++) {
                            line.append(" ");
                        }
                        chat = chat.substr(maxX) + line;
                        curIndexDown++;
                    }
                    if(msgList->at(curIndexUp + 1).length() > maxX) {
                        if(curLineIndexUp == msgList->at(curIndexUp + 1).length() / maxX - (msgList->at(curIndexUp + 1).length() % maxX == 0?1:0)) {
                            curLineIndexUp = 0;
                            curIndexUp++;
                        } else {
                            curLineIndexUp++;
                        }
                    } else {
                        curLineIndexUp = 0;
                        curIndexUp++;
                    }
                }
                k_mutex.unlock();
                PrintChat(win, githubID, maxY, maxX);
            } else if(input == 27) {    // ESC
                k_mutex.lock();
                end_flag = false;
                chat = "";
                wclear(win);
                wrefresh(win);
                k_mutex.unlock();
                break; //return;
            } else if(input == 10) {    // ENTER
                if(nodeMap->find(githubID) == nodeMap->end() && githubID.at(0) != '#') { // opponent disconnected
                    k_mutex.lock();
                    mvwprintw(win, maxY - curInputLine -1, 0, "!A SUMMONER HAS DISCONNECTED");
                    for(unsigned int i = 0 ; i < maxX*curInputLine + maxX - string("!A SUMMONER HAS DISCONNECTED").length(); i++) {
                        waddch(win, ' ');
                    }
                    mvwprintw(win, maxY - curInputLine, 0, "PRESS ESC TO FF THIS GAME");
                    k_mutex.unlock();
                    while(wgetch(win) != 27) {
                        //do_nothing
                    }
                    k_mutex.lock();
                    end_flag = false;
                    chat = "";
                    wclear(win);
                    wrefresh(win);
                    k_mutex.unlock();
                    break;
                }
                r_mutex.lock();
                unordered_map<string,tuple<vector<string>*,unsigned int,time_t>*>::iterator it = chatRoomMap->find(githubID);
                msgList->push_back("Me: " + typing);
                get<2>(*(it->second)) = time(NULL) + 32400; // korean time zone
                pair<string, tuple<vector<string>*,unsigned int,time_t>*> newEntry(*it);
                chatRoomMap->erase(it);
                chatRoomMap->insert(chatRoomMap->begin(), newEntry);
                r_mutex.unlock();
                //curY = msgList->size() + 1;
                //curY += ( string("Me").length() + typing.length() + CHAT_DELIMETER ) / maxX;
                k_mutex.lock();
                curX = 0;
                curInputLine = 1;

                k_mutex.unlock();

                if(githubID.at(0) == '#') {
                    int start = 1, end = 0;
                    while(githubID.find('#', start) != string::npos) {
                        end = githubID.find('#', start);
                        string tmpGithubID = githubID.substr(start, end - start);
                        if(nodeMap->find(tmpGithubID) == nodeMap->end()) {
                            start = end + 1;
                            continue;
                        }
                        string tmp_ip = g_km->Findip(tmpGithubID);
                        msg->SetMessage(githubID, tmp_ip, typing);
                        msg->EncMessage(tmpGithubID);
                        msg->SendMessage();
                        start = end + 1;
                    }
                } else {
                    string tmp_ip = g_km->Findip(githubID);
                    msg->SetMessage(g_km->ReturnGithubID(), tmp_ip, typing);
                    msg->EncMessage(githubID);
                    msg->SendMessage();
                }
                typing = "";
                PrintChat(win, githubID, maxY, maxX);
            } else if(input == '\t') { // tab
                if(curX >= maxX - 4 && maxY == curInputLine + LOGO_HEIGHT) {
                    continue;
                }
                typing.insert(curX, 4, ' ');
                k_mutex.lock();
                curInputLine = 1 + (typing.length() + CHAT_INPUTCHAR) / maxX;
                curX += 4;
                k_mutex.unlock();
                PrintChat(win, githubID, maxY, maxX);
            } else if(input == KEY_BACKSPACE) {
                if(typing.length() > 0 && curX > 0) {
                    typing.erase(curX - 1, 1);
                    k_mutex.lock();
                    curInputLine = 1 + (typing.length() + CHAT_INPUTCHAR) / maxX;
                    curX--;
                    mvwprintw(win, maxY - curInputLine, CHAT_INPUTCHAR, typing.c_str());
                    for(unsigned int i = 0 ; i <= typing.length() % maxX ; i++) {
                        wprintw(win," ");
                    }
                    wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX + CHAT_INPUTCHAR< maxX )? curX + CHAT_INPUTCHAR : (curX + CHAT_INPUTCHAR) % maxX);
                    wrefresh(win);
                    k_mutex.unlock();
                }
            } else if(input == KEY_LEFT) {
                if(curX > 0) {
                    k_mutex.lock();
                    curX--;
                    mvwprintw(win, maxY - curInputLine, CHAT_INPUTCHAR, typing.c_str());
                    wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX + CHAT_INPUTCHAR< maxX )? curX + CHAT_INPUTCHAR : (curX + CHAT_INPUTCHAR) % maxX);
                    wrefresh(win);
                    k_mutex.unlock();
                }
            } else if(input == KEY_RIGHT) {
                if(curX < typing.length()) {
                    k_mutex.lock();
                    curX++;
                    mvwprintw(win, maxY - curInputLine, CHAT_INPUTCHAR, typing.c_str());
                    wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX + CHAT_INPUTCHAR< maxX )? curX + CHAT_INPUTCHAR : (curX + CHAT_INPUTCHAR) % maxX);
                    wrefresh(win);
                    k_mutex.unlock();
                }
            } else if(input == 330) {
                if(curX < typing.length()) {
                    k_mutex.lock();
                    typing.erase(curX, 1);
                    curInputLine = 1 + (typing.length() + CHAT_INPUTCHAR) / maxX;
                    mvwprintw(win, maxY - curInputLine, CHAT_INPUTCHAR, typing.c_str());
                    for(unsigned int i = 0 ; i <= typing.length() % maxX ; i++) {
                        wprintw(win," ");
                    }
                    wmove(win, maxY - curInputLine + ((curX + CHAT_INPUTCHAR) / maxX), (curX < maxX )? curX + CHAT_INPUTCHAR : curX % maxX + 1);
                    wrefresh(win);
                    k_mutex.unlock();
                }
            }
        }
        k_mutex.lock();
        wclear(win);
        wrefresh(win);
        k_mutex.unlock();
        r_cv.notify_one();
    }

    void OnionUI::Init(){
        std::system("clear");
        cout << onionlogo;
    }

    void OnionUI::ShowLogo(){
        cout << onionlogo;
    }

    void OnionUI::ShowMenu(){
        cout << onionmemu;
    }

}
