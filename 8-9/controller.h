#pragma once
#include "filerepository.h"
#include "movievalidator.h"

class Controller {
    private:
    Repository&     database;
    FileRepository* watchlist;
    MovieValidator validator;

    public:
    /*
     * parameterized constructor
     * input: a reference to a MovieRepo for the database and one for the watchlist
     */
    Controller(Repository& _database, FileRepository* _watchlist);
    ~Controller();

    // returns a reference to the controller's database
    Repository& get_database() {
        return this->database;
    }
    // returns a reference to the controller's watchlist
    FileRepository* get_watchlist() {
        return this->watchlist;
    }

    void set_watchlist(FileRepository* _watchlist);

    /*
     * creates a new Movie with the given arguments and adds it to the database if it is valid
     * throws: invalid_argument if the arguments are invalid or if a Movie with the same id already exists in the repo
     */
    void database_add(const std::string& title, const std::string& genre, int year, int likes,
                      const std::string& trailer);
    /*
     * deletes the Movie with the given id from the database
     * input: the id of the Movie to be deleted
     * throws: out_of_range if the Movie with the given id does not exist
     */
    void database_remove(const std::string& id);
    /* creates a new Movie from the given parameters and updates the Movie with the given id with the newly created Movie
     * input: id: the id of the Movie from the database to be updated
     *        the rest of the parameters: used to create a new Movie
     * throws: invalid_argument if the Movie is invalid or out_of_range if tbe Movie with the given id does not exist
     */
    void database_update(const std::string& id, const std::string& title, const std::string& genre,
                         int year, int likes, const std::string& trailer);

    /*
     * adds a given Movie to the watchlist
     * throws: invalid_argument if a Movie with the same id already exists in the watchlist
     */
    void watchlist_add(const Movie& movie);
    /*
     * deletes the Movie with the given id from the watchlist and adds a like to it if specified
     * input: id: the id of the Movie to be deleted
     *        liked: whether to add a like to the Movie or not
     * throws: out_of_range if the Movie with the given id does not exist
     */
    void watchlist_remove(const std::string& id, bool liked);
};
