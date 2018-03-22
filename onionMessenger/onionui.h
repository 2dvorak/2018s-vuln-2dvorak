#ifndef ONIONUI_H
#define ONIONUI_H

#include "common.h"

namespace oniui{
    class OnionUI{
    public:
        OnionUI();
        virtual ~OnionUI();
        static void MainUI();
        void ShowLogo();
        void ShowMenu();
    private:
    };
}

#endif // ONIONUI_H
