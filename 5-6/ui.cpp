#include "ui.h"
#include <iostream>

using namespace std;

UI::UI(Controller& _ctrl) : ctrl{ _ctrl } {}

UI::~UI() {}

void UI::print_admin_menu() {
    cout << "\n1. print all the movies"
            "\n2. add a new movie"
            "\n3. remove a movie"
            "\n4. update a movie"
            "\n0. exit";
}

void UI::print_movies() {
    cout << this->ctrl.get_repo().to_string();
}

void UI::add() {
    string title, genre, trailer;
    int year, likes;

    cout << "give the title: ";
    getline(cin, title);
    cout << "give the genre: ";
    getline(cin, genre);
    cout << "give the year: ";
    cin >> year;
    cout << "give the number of likes: ";
    cin >> likes;
    getchar();
    cout << "give the trailer: ";
    getline(cin, trailer);

    this->ctrl.add(title, genre, year, likes, trailer);
}

void UI::remove() {
    string id;

    cout << "id (title_year): ";
    getline(cin, id);

    this->ctrl.remove(id);
}

void UI::update() {
    string title, genre, trailer, id;
    int year, likes;

    cout << "id (title_year): ";
    cin >> id;
    getchar();
    cout << "give the title: ";
    getline(cin, title);
    cout << "give the genre: ";
    getline(cin, genre);
    cout << "give the year: ";
    cin >> year;
    cout << "give the number of likes: ";
    cin >> likes;
    getchar();
    cout << "give the trailer: ";
    getline(cin, trailer);

    this->ctrl.update(id, title, genre, year, likes, trailer);
}

void UI::start() {
    int cmd = -1;
    cout << "Local Movie Database" << endl;

    while (cmd != 0) {
        cout << "\n1. use as administrator\n"
                "2. use as user\n"
                "0. exit\n"
                "your command: ";
        cin >> cmd;
        getchar();
        switch (cmd) {
            case 0: break;
            case 1: {
                while (cmd != 0) {
                    print_admin_menu();
                    cout << "\n\nyour command: ";
                    cin >> cmd;
                    getchar();
                    switch (cmd) {
                        case 0: break;
                        case 1: {
                            cout << this->ctrl.get_repo().to_string();
                            break;
                        }
                        case 2: {
                            try {
                                this->add();
                            } catch (std::exception& err) {
                                cout << "\nerror: " << err.what() << endl;
                                break;
                            }
                            cout << this->ctrl.get_repo().to_string();
                            break;
                        }
                        case 3: {
                            try {
                                this->remove();
                            } catch (std::exception& err) {
                                cout << "\nerror: " << err.what() << endl;
                                break;
                            }
                            cout << this->ctrl.get_repo().to_string();
                            break;
                        }
                        case 4: {
                            try {
                                this->update();
                            } catch (std::exception& err) {
                                cout << "\nerror: " << err.what() << endl;
                                break;
                            }
                            cout << this->ctrl.get_repo().to_string();
                            break;
                        }
                        default: cout << "\ninvalid command\n";
                    }
                    cin.clear();
                }
                cmd = -1;
                break;
            }
            default: cout << "\ninvalid command\n";
        }
        cin.clear();
    }
}
