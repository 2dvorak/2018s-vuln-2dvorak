
#include "common.h"
#include "onionui.h"

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

    static char *onionmemu = (char *)"\
            \n1. List\
            \n2. Talk\
            \n3. Exit\
            \n> ";

    void OnionUI::MainUI(){
        OnionUI *ui = new OnionUI();
        ui->ShowLogo();
        int value = 0;
        while(1){
            ui->ShowMenu();
            cin >> value;
            switch(value){
            case 1:
                if(qRecvMsg.empty() == 0) {
                    string str = qRecvMsg.front();
                    cout << str << "\n";
                    qRecvMsg.pop();}
            break;
            case 2:
            break;
            case 3:
                exit(1);
            break;
            default:
            continue;
            }
        }
    }

    void OnionUI::ShowLogo(){
        cout << onionlogo;
    }

    void OnionUI::ShowMenu(){
        cout << onionmemu;
    }

}

