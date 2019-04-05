#pragma once
#include "movie.h"
#include "dynamicarray.h"

class MovieRepo {
    private:
        DynamicVector<Movie> movies;
    public:
        MovieRepo();
        MovieRepo(const MovieRepo& other);
        ~MovieRepo();

        DynamicVector<Movie>& get_movies() { return this->movies; }
        int size() { return this->movies.size(); }

        int find(std::string id);
        void add(const Movie& movie);
        void remove(int index);
        void update(int index, const Movie& new_movie);

        MovieRepo& filter_by(bool (*filter)(const Movie&));

        void play();
        std::string to_string();
        void populate();

        MovieRepo& operator=(const MovieRepo& other);
        Movie& operator[](int index);
};

void test_repo();
