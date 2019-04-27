#pragma once
#include <string>

using std::string;

class Movie {
    private:
    string title;
    string genre;
    int    year;
    int    likes;
    string trailer;
    string id;

    public:
    Movie() = default;

    /*
     * parameterized constructor; sets the id as "title_year"
     * input: _title: title of the movie
     *        _genre: genre of the movie
     *        _year: year of the movie
     *        _likes: likes of the movie
     *        _trailer: trailer of the movie
     */
    Movie(const string& _title, const string& _genre, int _year, int _likes, const string& _trailer);
    Movie(const Movie& other) = default;
    ~Movie()                  = default;

    // getters for all attributes
    string get_title() const { return this->title; }
    string get_genre() const { return this->genre; }
    int    get_year() const { return this->year; }
    int    get_likes() const { return this->likes; }
    string get_trailer() const { return this->trailer; }
    string get_id() const { return this->id; }

    /*
     * assignment operator; copies each attributes from the given Movie
     * input: Movie reference from which to take the attributes
     * output: reference to the the newly copied Movie
     */
    Movie& operator=(const Movie& other);
    /*
     * equality compariosn operator
     * input: Movie reference with which to compare
     * output: true if the ids of the Movies are equal, false if not
     */
    bool operator==(const Movie& other);
    /*
     * extraction operator for ostream; performs a formatted output to the ostream
     * input: an ostream reference and a Movie reference
     * output: reference to the same ostream
     */
    friend std::ostream& operator<<(std::ostream& os, Movie& movie);

    // increments the number of likes by 1
    void add_like();
    // opens the trailer link from cmd
    void play();
};
