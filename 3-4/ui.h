#pragma once
#include "controller.h"

/*
defines an object that deals with user IO through the console and calls functiosn from a Controller
ctrl: pointer to the Controller on which to operate
today: string representation of today's date
*/
typedef struct {
    Controller* ctrl;
    char today[11];
} UI;

/*
creates an UI object with the given Controller
input: pointer to a Controller
output: pointer to the newly created UI
*/
UI* create_ui(Controller* controller);
/*
destroys a given UI by freeing the memory it occupies, along with its Controller
input: pointer to the UI
output: -
*/
void destroy_ui(UI* ui);
/*
main function of the UI which deals with IO and operates its Controller
input: pointer to the UI
output: -
*/
void start_ui(UI* ui);
