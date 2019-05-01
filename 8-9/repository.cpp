#include "repository.h"
#include "exceptions.h"
#include <algorithm>
#include <fstream>
#include <stdexcept>

vector<std::string>::iterator MovieRepo::find_genre(const string& genre) {
    return std::find(this->genres.begin(), this->genres.end(), genre);
}

void MovieRepo::read_from_file() {
    std::ifstream f(this->file);
    Movie         movie;
    if (!f.is_open()) return;
    while (!f.eof()) {
        f >> movie;
        this->add(movie);
    }
    f.close();
}

void MovieRepo::write_to_file() {
    std::ofstream f(this->file);
    if (!f.is_open()) return;
    for (Movie movie : this->movies) {
        f << movie;
        if (!(movie == this->movies[this->movies.size() - 1])) f << '\n';
    }
    f.close();
}

MovieRepo::MovieRepo(const std::string& _file) : file{ _file } { read_from_file(); }

vector<Movie>::iterator MovieRepo::find(const std::string& id) {
    return std::find_if(this->movies.begin(), this->movies.end(), [id](const Movie& m) {
        return m.get_id() == id;
    });
}

void MovieRepo::add(const Movie& movie) {
    if (this->find(movie.get_id()) != this->movies.end())
        throw RepoException("movie already exists");
    this->movies.push_back(movie);
    if (this->find_genre(movie.get_genre()) == this->genres.end())
        this->genres.push_back(movie.get_genre());
    write_to_file();
}

void MovieRepo::remove(const string& id) {
    auto movie_iter = this->find(id);
    if (movie_iter == this->movies.end()) throw RepoException("movie does not exist");
    auto genre_iter = this->find_genre(movie_iter->get_genre());
    this->movies.erase(movie_iter);
    if (genre_iter != this->genres.end()) this->genres.erase(genre_iter);
    write_to_file();
}

void MovieRepo::update(const string& id, const Movie& new_movie) {
    auto movie_iter = this->find(id);
    if (movie_iter == this->movies.end()) throw RepoException("movie does not exist");
    auto genre_iter = this->find_genre(movie_iter->get_genre());
    if (genre_iter != this->genres.end()) this->genres.erase(genre_iter);
    *movie_iter = new_movie;
    if (this->find_genre(new_movie.get_genre()) == this->genres.end())
        this->genres.push_back(new_movie.get_genre());
    write_to_file();
}

void MovieRepo::populate() {
    this->add(Movie{ "Bird Box", "horror", 2018, 123,
                     "https://www.imdb.com/title/tt2737304/videoplayer/vi2966010393" });
    this->add(Movie{ "It", "horror", 2017, 321,
                     "https://www.imdb.com/title/tt1396484/videoplayer/vi1396095257" });
    this->add(Movie{ "The Shining", "horror", 1980, 999,
                     "https://www.imdb.com/title/tt0081505/videoplayer/vi1476002073" });
    this->add(Movie{ "Avengers", "action", 2019, 1,
                     "https://www.imdb.com/title/tt4154796/videoplayer/vi786742553" });
    this->add(Movie{ "Avengers", "action", 2018, 9,
                     "https://www.imdb.com/title/tt4154756/videoplayer/vi528070681" });
    this->add(Movie{ "Batman", "action", 2008, 100,
                     "https://www.imdb.com/title/tt0468569/videoplayer/vi324468761" });
    this->add(Movie{ "Deadpool", "comedy", 2016, 420,
                     "https://www.imdb.com/title/tt1431045/videoplayer/vi567457049" });
    this->add(Movie{ "Aladdin", "comedy", 2019, 69,
                     "https://www.imdb.com/title/tt6139732/videoplayer/vi290241049" });
    this->add(Movie{ "Interstellar", "drama", 2014, 666,
                     "https://www.imdb.com/title/tt0816692/videoplayer/vi1586278169" });
    this->add(Movie{ "Pulp Fiction", "drama", 1994, 80085,
                     "https://www.imdb.com/title/tt0110912/videoplayer/vi2620371481" });
}

MovieRepo& MovieRepo::operator=(const MovieRepo& other) {
    if (this == &other) return *this;
    this->movies = other.movies;
    this->genres = other.genres;
    return *this;
}

Movie& MovieRepo::operator[](int index) {
    if (index < 0 || static_cast<unsigned long long>(index) >= this->movies.size())
        throw RepoException("invalid index");
    return this->movies[static_cast<unsigned long long>(index)];
}

void MovieRepo::add_like(const string& id) {
    auto movie_iter = this->find(id);
    if (movie_iter != this->movies.end()) movie_iter->add_like();
    write_to_file();
}
