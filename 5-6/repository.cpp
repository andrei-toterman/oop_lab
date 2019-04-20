#include "repository.h"

int MovieRepo::find_genre(const std::string& genre) {
    for (int i = 0; i < this->genres.size(); i++)
        if (genre == this->genres[i]) return i;
    return -1;
}

int MovieRepo::find(const std::string& id) {
    for (int i = 0; i < this->size(); i++)
        if (id == this->movies[i].get_id()) return i;
    return -1;
}

void MovieRepo::add(const Movie& movie) {
    this->movies + movie;
    if (this->find_genre(movie.get_genre()) == -1) this->genres.add(movie.get_genre());
}

void MovieRepo::remove(int index) {
    int genre_index = this->find_genre(this->movies[index].get_genre());
    this->movies.remove(index);
    if (genre_index >= 0) this->genres.remove(genre_index);
}

void MovieRepo::update(int index, const Movie& new_movie) {
    this->movies[index] = new_movie;
    if (this->find_genre(new_movie.get_genre()) == -1) this->genres.add(new_movie.get_genre());
}

void MovieRepo::populate() {
    this->add(Movie{"Bird Box", "horror", 2018, 123,
                    "https://www.imdb.com/title/tt2737304/videoplayer/vi2966010393"});
    this->add(Movie{"It", "horror", 2017, 321,
                    "https://www.imdb.com/title/tt1396484/videoplayer/vi1396095257"});
    this->add(Movie{"The Shining", "horror", 1980, 999,
                    "https://www.imdb.com/title/tt0081505/videoplayer/vi1476002073"});
    this->add(Movie{"Avengers", "action", 2019, 1,
                    "https://www.imdb.com/title/tt4154796/videoplayer/vi786742553"});
    this->add(Movie{"Avengers", "action", 2018, 9,
                    "https://www.imdb.com/title/tt4154756/videoplayer/vi528070681"});
    this->add(Movie{"Batman", "action", 2008, 100,
                    "https://www.imdb.com/title/tt0468569/videoplayer/vi324468761"});
    this->add(Movie{"Deadpool", "comedy", 2016, 420,
                    "https://www.imdb.com/title/tt1431045/videoplayer/vi567457049"});
    this->add(Movie{"Aladdin", "comedy", 2019, 69,
                    "https://www.imdb.com/title/tt6139732/videoplayer/vi290241049"});
    this->add(Movie{"Interstellar", "drama", 2014, 666,
                    "https://www.imdb.com/title/tt0816692/videoplayer/vi1586278169"});
    this->add(Movie{"Pulp Fiction", "drama", 1994, 80085,
                    "https://www.imdb.com/title/tt0110912/videoplayer/vi2620371481"});
}

MovieRepo& MovieRepo::operator=(const MovieRepo& other) {
    if (this == & other) return *this;
    this->movies = other.movies;
    this->genres = other.genres;
    return *this;
}

Movie& MovieRepo::operator[](int index) { return this->movies[index]; }
