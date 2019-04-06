#pragma once
#include "controller.h"

class UI {
    private:
        Controller& ctrl;
        void print_admin_menu();
        void print_user_menu();

        void repo_add();
        void repo_remove();
        void repo_update();
        void print_repo();

        void browse_by_genre();
        void watchlist_remove();
        void print_watchlist();
        bool yes_no(const std::string& msg);
    public:
        UI(Controller& _ctrl);
        ~UI();

        void start();
};
