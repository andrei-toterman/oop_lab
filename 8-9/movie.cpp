#include "movie.h"
#include <ostream>

Movie::Movie(const string& _title, const string& _genre, int _year, int _likes,
             const string& _trailer) :
    title{ _title },
    genre{ _genre },
    year{ _year },
    likes{ _likes },
    trailer{ _trailer },
    id{ this->title + "_" + std::to_string(this->year) } {}

void Movie::add_like() { this->likes++; }

Movie& Movie::operator=(const Movie& other) {
    this->title   = other.title;
    this->genre   = other.genre;
    this->year    = other.year;
    this->likes   = other.likes;
    this->trailer = other.trailer;
    this->id      = other.id;
    return *this;
}

bool Movie::operator==(const Movie& other) { return this->id == other.id; }

std::ostream& operator<<(std::ostream& os, Movie& movie) {
    os << movie.title << " - " << movie.year << "\nGenre: " << movie.genre << ",\t" << movie.likes
       << " likes";
    return os;
}

void Movie::play() { system(("start " + this->trailer).c_str()); }
