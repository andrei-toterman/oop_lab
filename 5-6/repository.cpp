#include "repository.h"
#include <assert.h>

MovieRepo::MovieRepo() {}

MovieRepo::MovieRepo(const MovieRepo& other) {
    this->movies = other.movies;
}

MovieRepo::~MovieRepo() {}

int MovieRepo::find(std::string id) {
    for (int i = 0; i < this->size(); i++)
        if (id == this->movies[i].get_id())
            return i;
    return -1;
}

void MovieRepo::add(const Movie& movie) {
    this->movies + movie;
}

void MovieRepo::remove(int index) {
    this->movies.remove(index);
}

void MovieRepo::update(int index, const Movie& new_movie) {
    this->movies[index] = new_movie;
}

void MovieRepo::populate() {
    this->add(Movie{ "Bird Box", "horror", 2018, 123, "https://www.imdb.com/title/tt2737304/videoplayer/vi2966010393" });
    this->add(Movie{ "It", "horror", 2017, 321, "https://www.imdb.com/title/tt1396484/videoplayer/vi1396095257" });
    this->add(Movie{ "The Shining", "horror", 1980, 999, "https://www.imdb.com/title/tt0081505/videoplayer/vi1476002073" });
    this->add(Movie{ "Avengers", "action", 2019, 1, "https://www.imdb.com/title/tt4154796/videoplayer/vi786742553" });
    this->add(Movie{ "Avengers", "action", 2018, 9, "https://www.imdb.com/title/tt4154756/videoplayer/vi528070681" });
    this->add(Movie{ "Batman", "action", 2008, 100, "https://www.imdb.com/title/tt0468569/videoplayer/vi324468761" });
    this->add(Movie{ "Deadpool", "comedy", 2016, 420, "https://www.imdb.com/title/tt1431045/videoplayer/vi567457049" });
    this->add(Movie{ "Aladdin", "comedy", 2019, 69, "https://www.imdb.com/title/tt6139732/videoplayer/vi290241049" });
    this->add(Movie{ "Interstellar", "drama", 2014, 666, "https://www.imdb.com/title/tt0816692/videoplayer/vi1586278169" });
    this->add(Movie{ "Pulp Fiction", "drama", 1994, 80085, "https://www.imdb.com/title/tt0110912/videoplayer/vi2620371481" });
}

MovieRepo&MovieRepo::operator=(const MovieRepo& other) {
    if (this == & other) return *this;
    this->movies = other.movies;
    return *this;
}

Movie& MovieRepo::operator[](int index) {
    return this->movies[index];
}

static void test_add() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.add(Movie("d", "e", 3, 4, "f"));
    repo.add(Movie("g", "h", 5, 6, "i"));
    assert(repo.get_movies().size() == 3);
    repo.add(Movie("j", "k", 7, 8, "l"));
    assert(repo.get_movies().capacity() == 6);
}

static void test_remove() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    repo.add(Movie("d", "e", 3, 4, "f"));
    repo.remove(0);
    assert(repo.size() == 1);
    try {
        repo.remove(1);
        assert(false);
    } catch (...) {
        assert(true);
    }
}

static void test_index_operator() {
    MovieRepo repo;
    Movie m{"a", "b", 1, 2, "c"};
    repo.add(m);
    assert(repo.get_movies()[0] == m);
    try {
        repo.get_movies()[1];
        assert(false);
    } catch (...) {
        assert(true);
    }
}

static void test_find() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    assert(repo.find("a_1") >= 0);
    assert(repo.find("a_2") == -1);
}

static void test_assign_operator() {
    MovieRepo repo;
    repo.add(Movie("a", "b", 1, 2, "c"));
    MovieRepo new_repo{ repo };
    assert(new_repo.size() == 1);
    assert(new_repo.get_movies().capacity() == 3);
    assert(new_repo[0] == repo[0]);
}

void test_repo() {
    test_add();
    test_remove();
    test_index_operator();
    test_assign_operator();
    test_find();
}
