#pragma once
#include "movie.h"
#include <vector>

using std::vector;

class Repository {
    protected:
    vector<Movie>  movies;
    vector<string> genres;
    /*
     * searches for a Movie genre in the genres DynamicVector
     * input: the genre after which to look
     * output: the position at which the Movie genre is found, or -1 if it is not found
     */
    vector<string>::iterator find_genre(const string& genre);

    public:
    Repository()                        = default;
    Repository(const Repository& other) = default;
    virtual ~Repository()               = default;

    // getters for all attributes
    vector<Movie>& get_movies() {
        return this->movies;
    }
    vector<string>& get_genres() {
        return this->genres;
    }

    // returns the size of the movies DynamicVector
    int size() { return static_cast<int>(this->movies.size()); }
    /*
     * searches for a Movie with the given id in the movies DynamicVector
     * input: the id after which to look
     * output: the position at which the Movie is found, or -1 if it is not found
     */
    vector<Movie>::iterator find(const string& id);
    /*
     * adds a given Movie to the movies DynamicVector and adds its genre to the genres DynamicVector if needed
     * input: reference to Movie to be added
     */
    virtual void add(const Movie& movie);
    /*
     * removes the Movie at the given index from the movies DynamicVector, along with its genre if needed
     * input: the index at which the Movie is located
     */
    virtual void remove(const string& id);
    /*
     * assigns to the Movie at the given index from the moveis DynamicVector a new given Movie
     * input: index: the index at which the Movie to be updated is located
     *        new_movie: reference to the Movie with which to update
     */
    virtual void update(const string& id, const Movie& new_movie);

    /* generic filtering function; keeps the elements that satisfy the given condition
     * input: a function with which to check the elements of the movies DynamicVector
     * output: a new MovieRepo with the filtered movies
     */
    template <typename Func>
    Repository filter_by(Func filter);

    // populates the repository with some pre-made values
    void populate();
    void add_like(const string& id);

    /*
     * assignment operator; copies the attributes from the given MovieRepo
     * input: reference to a MovieRepo from which to copy
     * output: reference to the newly copied MovieRepo
     */
    Repository& operator=(const Repository& other) = default;
    /* subscript operator
     * input: index of Movie from the movies DynamicVector
     * output: reference to the movie at that index
     */
    Movie& operator[](int index);
};

template <typename Func>
Repository Repository::filter_by(Func filter) {
    Repository filtered_repo;
    for (Movie movie : this->get_movies())
        if (filter(movie)) filtered_repo.add(movie);
    return filtered_repo;
}
