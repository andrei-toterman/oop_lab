﻿#include "controller.h"
#include <assert.h>

Controller::Controller(MovieRepo& _repo, MovieRepo& _watchlist) :
    database{ _repo },
    watchlist{ _watchlist }
{}

void Controller::database_add(const std::string& title, const std::string& genre, int year, int likes, const std::string& trailer) {
    Movie new_movie{title, genre, year, likes, trailer};
    validate_movie(new_movie);
    if (this->database.find(new_movie.get_id()) >= 0)
        throw std::invalid_argument("movie already exists");
    this->database.add(new_movie);
}

void Controller::database_remove(const std::string& id) {
    int index = this->database.find(id);
    if (index < 0)
        throw std::out_of_range("movie does not exist");
    this->database.remove(index);
    index = this->watchlist.find(id);
    this->watchlist.remove(index);
}

void Controller::database_update(const std::string& id, const std::string& title, const std::string& genre, int year, int likes, const std::string& trailer) {
    Movie new_movie{title, genre, year, likes, trailer};
    validate_movie(new_movie);
    int index = this->database.find(id);
    if (index < 0)
        throw std::out_of_range("movie does not exist");
    this->database.update(index, new_movie);
    index = this->watchlist.find(id);
    this->watchlist.update(index, new_movie);
}

void Controller::watchlist_add(const Movie& movie) {
    if (this->watchlist.find(movie.get_id()) >= 0)
        throw std::invalid_argument("movie already exists");
    this->watchlist.add(movie);
}

void Controller::watchlist_remove(const std::string& id, bool liked) {
    int w_index = this->watchlist.find(id);
    int r_index = this->database.find(id);
    if (w_index < 0)
        throw std::out_of_range("movie does not exist");
    if (liked) this->database[r_index].add_like();
    this->watchlist.remove(w_index);
}

void Controller::validate_movie(const Movie& movie) {
    std::string error = "invalid material:\n";
    if (movie.get_title().empty())
        error += "title must not be empty\n";
    if (movie.get_genre().empty())
        error += "genre must not be empty\n";
    if (movie.get_year() < 0)
        error += "year must not be a positive number\n";
    if (movie.get_likes() < 0)
        error += "number of likes must be a positive number\n";
    if (movie.get_trailer().empty())
        error += "link to trailer must not be empty\n";
    if (movie.get_trailer().find("www.") != 0 &&
        movie.get_trailer().find("https://") != 0 &&
        movie.get_trailer().find("http://") != 0)
        error += "url must start with www., http:// or https://\n";
    if (error != "invalid material:\n")
        throw std::invalid_argument(error);
}

static void test_add() {
    MovieRepo database;
    MovieRepo watchlist;
    Controller ctrl{ database, watchlist };
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    assert(ctrl.get_database().size() == 1);
    try { ctrl.database_add("a", "b", 1, 2, "www.google.com");
        assert(false); } catch (...) { assert(true); }
    try { ctrl.database_add("", "a", 1, 2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_add("a", "", 1, 2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_add("a", "b", -1, 2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_add("a", "b", 1, -2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_add("a", "b", 1, 2, "");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_add("a", "b", 1, 2, "dbkjgrbg");
        assert(false); } catch(...) { assert(true); }

    ctrl.watchlist_add(database[0]);
    assert(ctrl.get_watchlist().size() == 1);
}

static void test_remove() {
    MovieRepo database;
    MovieRepo watchlist;
    Controller ctrl{ database, watchlist };
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    ctrl.database_add("b", "b", 1, 2, "www.google.com");
    ctrl.watchlist_add(database[0]);
    ctrl.watchlist_add(database[1]);
    ctrl.database_remove("a_1");
    assert(ctrl.get_database().size() == 1);
    assert(ctrl.get_watchlist().size() == 1);
    ctrl.watchlist_remove("b_1", true);
    assert(ctrl.get_watchlist().size() == 0);
    try { ctrl.database_remove("adwada");
        assert(false); } catch (...) { assert(true); }

}

static void test_update() {
    MovieRepo database;
    MovieRepo watchlist;
    Controller ctrl{ database, watchlist };
    Movie m{"b", "c", 2, 3, "www.reddit.com"};
    ctrl.database_add("a", "b", 1, 2, "www.google.com");
    ctrl.watchlist_add(database[0]);
    ctrl.database_update("a_1", "b", "c", 2, 3, "www.reddit.com");
    assert(database[0] == m);
    assert(watchlist[0] == m);
    try { ctrl.database_update("a_1", "b", "c", 2, 3, "www.reddit.com");
        assert(false); } catch (...) { assert(true); }
    try { ctrl.database_update("b_2", "", "a", 1, 2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_update("b_2", "a", "", 1, 2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_update("b_2", "a", "b", -1, 2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_update("b_2", "a", "b", 1, -2, "www.");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_update("b_2", "a", "b", 1, 2, "");
        assert(false); } catch(...) { assert(true); }
    try { ctrl.database_update("b_2", "a", "b", 1, 2, "dbkjgrbg");
        assert(false); } catch(...) { assert(true); }
}

void test_ctrl() {
    test_add();
    test_remove();
    test_update();
}
