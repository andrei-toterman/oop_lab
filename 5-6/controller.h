#pragma once
#include "repository.h"

class Controller {
    private:
        MovieRepo& repo;
        /*
         * checks if a given movie has valid attributes
         * throws: invalid_argument if any of the attributes is invalid
         */
        void validate_movie(const Movie& movie);
    public:
        /*
         * parameterized constructor
         * input: a reference to a MovieRepo
         */
        Controller(MovieRepo& _repo);
        /*
         * default destructor
         */
        ~Controller();

        // returns a reference to the controller's MovieRepo
        MovieRepo& get_repo() { return this->repo; }

        /*
         * creates a new Movie with the given arguments and adds it to the MovieRepo if it is valid
         * throws: invalid_arument if the arguments are invalid or if a Movie with the same id already exists in the MovieRepo
         */
        void add(const std::string& title, const std::string& genre, int year, int likes, const std::string& trailer);
        void remove(const std::string& id);
        void update(const std::string& id, const std::string& title, const std::string& genre, int year, int likes, const std::string& trailer);
};

void test_ctrl();
