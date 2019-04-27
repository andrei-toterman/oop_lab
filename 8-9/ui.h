#pragma once
#include "controller.h"

class UI {
    private:
    Controller& ctrl;
    // prints the menu designated for the admin
    void print_admin_menu();
    // prints the menu designated for the user
    void print_user_menu();

    // asks admin for input for adding a new Movie to the database
    void db_add();
    // asks the admin for input for removing a Movie from the database
    void db_remove();
    // asks the admin for input for updating a Movie in the database
    void db_update();
    // prints the admin's Movie database
    void print_db();

    // displays the movies from the database filtered by a genre
    // given by the user, plays them one at a time and asks the user
    // if they want the movie to be added to the watchlist
    // this is repeated until the user quits
    void browse_by_genre();
    // asks the user for input for removing a Movie from the watchlist
    void watchlist_remove();
    // prints the user's Movie watchlist
    void print_watchlist();
    /*
     * prints a given message then asks for user input
     * input: message to print
     * output: true if the user input is "yes", false if it is "no"
     *         prints an error if the user input is neither "yes" or "no"
     */
    bool yes_no(const std::string& msg);

    public:
    /*
     * parameterized constructor
     * input: reference to a Controller
     */
    explicit UI(Controller& _ctrl);
    ~UI() = default;

    // main loop of the program
    void start();
};
