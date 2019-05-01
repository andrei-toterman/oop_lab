#include "controller.h"
#include <stdexcept>

Controller::Controller(MovieRepo& _repo, MovieRepo& _watchlist) :
    database{ _repo },
    watchlist{ _watchlist } {}

void Controller::database_add(const std::string& title, const std::string& genre, int year,
                              int likes, const std::string& trailer) {
    Movie new_movie{ title, genre, year, likes, trailer };
    this->validator.validate_movie(new_movie);
    this->database.add(new_movie);
}

void Controller::database_remove(const std::string& id) {
    this->database.remove(id);
    try {
        this->watchlist.remove(id);
    } catch (...) {}
}

void Controller::database_update(const std::string& id, const std::string& title,
                                 const std::string& genre, int year, int likes,
                                 const std::string& trailer) {
    Movie new_movie{ title, genre, year, likes, trailer };
    this->validator.validate_movie(new_movie);
    this->database.update(id, new_movie);
    try { this->watchlist.update(id, new_movie); }
    catch (...) {}
}

void Controller::watchlist_add(const Movie& movie) { this->watchlist.add(movie); }

void Controller::watchlist_remove(const std::string& id, bool liked) {
    this->watchlist.remove(id);
    if (liked) this->database.add_like(id);
}
