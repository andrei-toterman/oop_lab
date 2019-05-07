#include "ui.h"
#include "csvwatchlist.h"
#include "exceptions.h"
#include "htmlwatchlist.h"
#include <iostream>

using namespace std;

UI::UI(Controller& _ctrl) : ctrl{ _ctrl } {}

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
            "\n4. open the watchlist"
            "\n0. exit";
}

void UI::print_db() {
    if (this->ctrl.get_database().size() == 0) {
        cout << "\nno movies found\n";
        return;
    }
    cout << "\ncurrent movies in the database:\n";
    for (Movie movie : this->ctrl.get_database().get_movies())
        cout << endl << movie.to_string() << endl;
}

void UI::browse_by_genre() {
    string genre;
    cout << "\ncurrent genres are:\n";
    for (string g : this->ctrl.get_database().get_genres()) cout << g << endl;
    cout << "\ngive the genre you would like to browse: ";
    getline(cin, genre);

    Repository temp_repo{ this->ctrl.get_database().filter_by(
        [genre](const Movie& movie) -> bool { return movie.get_genre() == genre; }) };

    if (temp_repo.size() == 0) {
        cout << "\nno movies found\n";
        return;
    }

    auto current_movie = temp_repo.get_movies().begin();

    do {
        cout << endl << current_movie->to_string() << endl;
        current_movie->play();
        if (this->yes_no("did you like the trailer?")) {
            try {
                this->ctrl.watchlist_add(*current_movie);
                cout << "\ngreat! movie has been added to your watchlist";
            } catch (RepoException& err) { cout << "\nerror: " << err.what() << endl; }
        }
        if (++current_movie == temp_repo.get_movies().end()) {
            cout << "\n\nstarting from the beginning of the list again";
            current_movie = temp_repo.get_movies().begin();
        }
    } while (this->yes_no("\ncontinue?"));
}

void UI::watchlist_remove() {
    string id;
    cout << "give the id of the movie (title_year): ";
    getline(cin, id);

    bool liked = this->yes_no("did you like the movie?");

    try {
        this->ctrl.watchlist_remove(id, liked);
    } catch (RepoException& err) { cout << "\nerror: " << err.what() << endl; }
}

void UI::print_watchlist() {
    if (this->ctrl.get_watchlist()->size() == 0) {
        cout << "\nno movies found\n";
        return;
    }
    cout << "\ncurrent movies in your watchlist:\n";
    for (Movie movie : this->ctrl.get_watchlist()->get_movies())
        cout << endl << movie.to_string() << endl;
}

bool UI::yes_no(const string& msg) {
    string answer;

    cout << msg + " ";
    while (true) {
        cout << "yes/no: ";
        getline(cin, answer);
        if (answer == "yes")
            return true;
        else if (answer == "no")
            return false;
        else
            cout << "\ninvalid answer\n";
    }
}

void UI::db_add() {
    string title, genre, trailer;
    int    year, likes;

    cout << "give the title: ";
    getline(cin, title);
    cout << "give the genre: ";
    getline(cin, genre);
    cout << "give the year: ";
    cin >> year;
    cout << "give the number of likes: ";
    cin >> likes;
    getchar();
    cout << "give the trailer: " << flush;
    getline(cin, trailer);

    try {
        this->ctrl.database_add(title, genre, year, likes, trailer);
        this->print_db();
    } catch (MovieException& err) {
        cout << "\nerror: " << err.what() << endl;
    } catch (RepoException& err) { cout << "\nerror: " << err.what() << endl; }
}

void UI::db_remove() {
    string id;

    cout << "id (title_year): ";
    getline(cin, id);

    try {
        this->ctrl.database_remove(id);
        this->print_db();
    } catch (RepoException& err) { cout << "\nerror: " << err.what() << endl; }
}

void UI::db_update() {
    string title, genre, trailer, id;
    int    year, likes;

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

    try {
        this->ctrl.database_update(id, title, genre, year, likes, trailer);
        this->print_db();
    } catch (MovieException& err) {
        cout << "\nerror: " << err.what() << endl;
    } catch (RepoException& err) { cout << "\nerror: " << err.what() << endl; }
}

void UI::start() {
    int cmd = -1;
    cout << "Local Movie Database\n"
            "\nChoose the watchlist format"
            "\n1. csv"
            "\n2. html"
         << endl;

    while (cmd != 1 && cmd != 2) {
        cin >> cmd;
        switch (cmd) {
            case 1: {
                this->ctrl.set_watchlist(new CsvWatchlist("watchlist.csv"));
                break;
            }
            case 2: {
                this->ctrl.set_watchlist(new HtmlWatchlist("watchlist.html"));
                break;
            }
            default: cout << "\ninvalid format\n";
        }
    }

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
                            this->print_db();
                            break;
                        }
                        case 2: {
                            this->db_add();
                            break;
                        }
                        case 3: {
                            this->db_remove();
                            break;
                        }
                        case 4: {
                            this->db_update();
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
                        case 4: {
                            this->ctrl.get_watchlist()->open();
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
