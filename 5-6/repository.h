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

        template <typename Func>
        MovieRepo filter_by(Func filter) {
            MovieRepo filtered_repo;
            for (Movie movie : this->get_movies())
                if (filter(movie))
                    filtered_repo.add(movie);
            return filtered_repo;
        }
        void populate();

        MovieRepo& operator=(const MovieRepo& other);
        Movie& operator[](int index);
};

void test_repo();
