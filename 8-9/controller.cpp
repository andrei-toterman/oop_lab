#include "controller.h"
#include <assert.h>
#include <stdexcept>

Controller::Controller(MovieRepo& _repo, MovieRepo& _watchlist) :
    database{ _repo },
    watchlist{ _watchlist } {}

void Controller::database_add(const std::string& title, const std::string& genre, int year,
                              int likes, const std::string& trailer) {
    Movie new_movie{ title, genre, year, likes, trailer };
    validate_movie(new_movie);
    this->database.add(new_movie);
}

void Controller::database_remove(const std::string& id) {
    this->database.remove(id);
    try { this->watchlist.remove(id); }
    catch (...) {}
}

void Controller::database_update(const std::string& id, const std::string& title,
                                 const std::string& genre, int year, int likes,
                                 const std::string& trailer) {
    Movie new_movie{ title, genre, year, likes, trailer };
    validate_movie(new_movie);
    this->database.update(id, new_movie);
    try { this->watchlist.update(id, new_movie); }
    catch (...) {}
}

void Controller::watchlist_add(const Movie& movie) { this->watchlist.add(movie); }

void Controller::watchlist_remove(const std::string& id, bool liked) {
    this->watchlist.remove(id);
    if (liked) this->database.add_like(id);
}

void Controller::validate_movie(const Movie& movie) {
    std::string error = "invalid material:\n";
    if (movie.get_title().empty()) error += "title must not be empty\n";
    if (movie.get_genre().empty()) error += "genre must not be empty\n";
    if (movie.get_year() < 0) error += "year must not be a positive number\n";
    if (movie.get_likes() < 0) error += "number of likes must be a positive number\n";
    if (movie.get_trailer().empty()) error += "link to trailer must not be empty\n";
    if (movie.get_trailer().find("www.") != 0 && movie.get_trailer().find("https://") != 0 &&
        movie.get_trailer().find("http://") != 0)
        error += "url must start with www., http:// or https://\n";
    if (error != "invalid material:\n") throw std::invalid_argument(error);
}
