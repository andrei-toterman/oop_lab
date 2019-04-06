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

void UI::print_user_menu() {
    cout << "\n1. see your watchlist"
            "\n2. browse movies by genre"
            "\n3. remove a movie from your watchlist"
            "\n0. exit";
}

void UI::print_repo() {
    if (this->ctrl.get_repo().size() == 0) {
        cout << "\nno movies found\n";
        return;
    }
    cout <<  "\ncurrent movies in the database:\n";
    for (Movie movie : this->ctrl.get_repo().get_movies())
        cout << movie;
}

void UI::browse_by_genre() {
    string genre;
    cout << "give the genre you would like to browse: ";
    getline(cin, genre);

    MovieRepo temp_repo { this->ctrl.get_repo().filter_by([genre](const Movie& movie) -> bool { return movie.get_genre() == genre; }) };

    if (temp_repo.size() == 0) {
        cout << "\nno movies found\n";
        return;
    }

    Movie* current_movie = temp_repo.get_movies().begin();
    bool liked;

    while (true) {
        cout << *current_movie;
        current_movie->play();
        liked = this->yes_no("did you like the trailer?");
        if (liked) {
            try {
                this->ctrl.watchlist_add(*current_movie);
                cout << "\ngreat! movie has been added to your watchlist\n";
            } catch (std::exception& err) {
                cout << "\nerror: " << err.what() << endl;
            }
        }
        if (!this->yes_no("continue?")) break;
        if (++current_movie == temp_repo.get_movies().end()) {
            cout << "\nstarting from the beginning again\n";
            current_movie = temp_repo.get_movies().begin();
        }
    }
}

void UI::watchlist_remove() {
    string id;
    cout << "give the id of the movie (title_year): ";
    getline(cin, id);

    bool liked = this->yes_no("did you like the movie?");

    try {
        this->ctrl.watchlist_remove(id, liked);
    } catch (std::exception& err) {
        cout << "\nerror: " << err.what() << endl;
    }
}

void UI::print_watchlist() {
    if (this->ctrl.get_watchlist().size() == 0) {
        cout << "\nno movies found\n";
        return;
    }
    cout <<  "\ncurrent movies in your watchlist:\n";
    for (Movie movie : this->ctrl.get_watchlist().get_movies())
        cout << movie;
}

bool UI::yes_no(const string& msg) {
    string answer;

    cout << msg + " ";
    while (true) {
        cout << "yes/no: ";
        getline(cin, answer);
    if (answer == "yes") return true;
    else if (answer == "no") return false;
    else cout << "\ninvalid answer\n";
    }
}

void UI::repo_add() {
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

    this->ctrl.repo_add(title, genre, year, likes, trailer);
}

void UI::repo_remove() {
    string id;

    cout << "id (title_year): ";
    getline(cin, id);

    this->ctrl.repo_remove(id);
}

void UI::repo_update() {
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

    this->ctrl.repo_update(id, title, genre, year, likes, trailer);
}

void UI::start() {
    int cmd = -1;
    cout << "Local Movie Database" << endl;

    while (cmd != 0) {
        cout << "\n1. use as administrator"
                "\n2. use as user"
                "\n0. exit"
                "\nyour command: ";
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
                            this->print_repo();
                            break;
                        }
                        case 2: {
                            try {
                                this->repo_add();
                            } catch (std::exception& err) {
                                cout << "\nerror: " << err.what() << endl;
                                break;
                            }
                            this->print_repo();
                            break;
                        }
                        case 3: {
                            try {
                                this->repo_remove();
                            } catch (std::exception& err) {
                                cout << "\nerror: " << err.what() << endl;
                                break;
                            }
                            this->print_repo();
                            break;
                        }
                        case 4: {
                            try {
                                this->repo_update();
                            } catch (std::exception& err) {
                                cout << "\nerror: " << err.what() << endl;
                                break;
                            }
                            this->print_repo();
                            break;
                        }
                        default: cout << "\ninvalid command\n";
                    }
                    cin.clear();
                }
                cmd = -1;
                break;
            }
            case 2: {
                while (cmd != 0) {
                    print_user_menu();
                    cout << "\nyour command: ";
                    cin >> cmd;
                    getchar();
                    switch (cmd) {
                        case 0: break;
                        case 1: {
                            this->print_watchlist();
                            break;
                        }
                        case 2: {
                            this->browse_by_genre();
                            break;
                        }
                        case 3: {
                            this->watchlist_remove();
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
