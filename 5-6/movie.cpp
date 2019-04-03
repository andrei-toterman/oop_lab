#include "movie.h"
#include <stdexcept>
#include <sstream>

Movie::Movie() {}

Movie::Movie(const std::string& id) {
    this->id = id;
}

Movie::Movie(const std::string& title, const std::string& genre, int year, int likes, const std::string& trailer) {
    this->title = title;
    this->genre = genre;
    this->year = year;
    this->likes = likes;
    this->trailer = trailer;
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

void Movie::set_title(const std::string& _title) {
    if (_title.size() == 0)
        throw std::invalid_argument("title must not be empty");
    this->title = _title;
    this->id = this->title + "_" + std::to_string(this->year);
}

void Movie::set_genre(const std::string& _genre) {
    if (_genre.size() == 0)
        throw std::invalid_argument("genre must not be empty");
    this->genre = _genre;
}

void Movie::set_year(int _year) {
    if (_year < 0)
        throw std::invalid_argument("the year can't be negative");
    this->year = _year;
    this->id = this->title + "_" + std::to_string(this->year);
}

void Movie::set_likes(int _likes) {
    if (_likes < 0)
        throw std::invalid_argument("the number of likes can't be negative");
    this->likes = _likes;
}

void Movie::set_trailer(const std::string& _trailer) {
    if (_trailer.size() == 0)
        throw std::invalid_argument("link to trailer must not be empty");
    if (_trailer.find("www.") != 0 && _trailer.find("https://") != 0 && _trailer.find("http://") != 0)
        throw std::invalid_argument("this isn't a valid url");
    this->trailer = _trailer;
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

std::string Movie::to_string() {
    std::ostringstream str;
    str << std::endl << this->title << " - " << this->year << "\nGenre: " << this->genre << ",\t" << this->likes << " likes" << std::endl;
    return str.str();
}

void Movie::play() {
    system(("start " + this->trailer).c_str());
}
