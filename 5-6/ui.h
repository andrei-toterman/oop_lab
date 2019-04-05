#pragma once
#include "controller.h"

class UI {
    private:
        Controller& ctrl;
        void print_admin_menu();
        void print_user_menu();
        void add();
        void remove();
        void update();
        void print_movies();
    public:
        UI(Controller& _ctrl);
        ~UI();

        void start();
};
