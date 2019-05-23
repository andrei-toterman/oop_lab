#include "movie.h"
#include "utils.h"
#include <iostream>
#include <vector>

Movie::Movie(const string& _title, const string& _genre, int _year, int _likes,
             const string& _trailer) :
    title{ _title },
    genre{ _genre },
    year{ _year },
    likes{ _likes },
    trailer{ _trailer },
    id{ this->title + "_" + std::to_string(this->year) } {}

void Movie::add_like() {
    this->likes++;
}

Movie& Movie::operator=(const Movie& other) {
    this->title   = other.title;
    this->genre   = other.genre;
    this->year    = other.year;
    this->likes   = other.likes;
    this->trailer = other.trailer;
    this->id      = other.id;
    return *this;
}

bool Movie::operator==(const Movie& other) {
    return this->id == other.id;
}

string Movie::to_string() {
    return this->title + " - " + std::to_string(this->year) + "\nGenre: " + this->genre + ",\t" +
           std::to_string(this->likes) + " likes";
}

string Movie::to_html_tr() {
    return "\n\t\t<tr>\n\t\t\t<td>" + this->title + "</td>\n\t\t\t<td>" + this->genre +
           "</td>\n\t\t\t<td>" + std::to_string(this->year) + "</td>\n\t\t\t<td>" +
           std::to_string(this->likes) + "</td>\n\t\t\t<td><a href =\"" + this->trailer +
           "\">Link</a></td>\n\t\t</tr>";
}

std::istream& operator>>(std::istream& is, Movie& movie) {
    std::vector<string> tokens;
    string              line;
    std::getline(is, line);
    tokens        = tokenize(line, ',');
    movie.title   = tokens[0];
    movie.genre   = tokens[1];
    movie.year    = std::stoi(tokens[2]);
    movie.likes   = std::stoi(tokens[3]);
    movie.trailer = tokens[4];
    movie.id      = tokens[0] + "_" + tokens[2];
    return is;
}

std::ostream& operator<<(std::ostream& os, Movie& movie) {
    os << movie.title << ',' << movie.genre << ',' << movie.year << ',' << movie.likes << ','
       << movie.trailer;
    return os;
}

void Movie::play() {
    system(("start " + this->trailer).c_str());
}
