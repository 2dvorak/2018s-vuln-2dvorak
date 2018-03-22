#ifndef ONIONUI_H
#define ONIONUI_H

#include "common.h"

namespace oniui{
    class OnionUI{
    public:
        OnionUI();
        virtual ~OnionUI();
        static void MainUI();
        static void UISendThread();
        static void UIRecvThread();
        void ShowLogo();
        void ShowMenu();
    private:
    };
}

#endif // ONIONUI_H
