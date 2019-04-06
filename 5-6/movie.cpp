#include "movie.h"
#include <sstream>
#include <stdexcept>

Movie::Movie() {}

Movie::Movie(const std::string& _title, const std::string& _genre, int _year, int _likes, const std::string& _trailer) {
    this->title = _title;
    this->genre = _genre;
    this->year = _year;
    this->likes = _likes;
    this->trailer = _trailer;
    this->id = this->title + "_" + std::to_string(this->year);
}

Movie::Movie(const Movie& other) {
    this->title = other.title;
    this->genre = other.genre;
    this->year = other.year;
    this->likes = other.likes;
    this->trailer = other.trailer;
    this->id = other.id;
}

Movie::~Movie() {}

void Movie::add_like() {
    this->likes++;
}

Movie& Movie::operator=(const Movie& other) {
    this->title = other.title;
    this->genre = other.genre;
    this->year = other.year;
    this->likes = other.likes;
    this->trailer = other.trailer;
    this->id = other.id;
    return *this;
}

bool Movie::operator==(const Movie& other) {
    return this->id.compare(other.id) == 0;
}

std::ostream& operator<<(std::ostream& os, Movie& movie) {
    os << std::endl << movie.title << " - " << movie.year << "\nGenre: " << movie.genre << ",\t" << movie.likes << " likes" << std::endl;
    return os;
}

void Movie::play() {
    system(("start " + this->trailer).c_str());
}

