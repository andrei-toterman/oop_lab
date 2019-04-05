#pragma once
#include <string>

class Movie {
    private:
        std::string title;
        std::string genre;
        int year;
        int likes;
        std::string trailer;
        std::string id;
    public:
        Movie();
        Movie(const std::string& _title, const std::string& _genre, int _year, int _likes, const std::string& _trailer);
        Movie(const Movie& other);
        ~Movie();

        std::string get_title() const { return this->title; }
        std::string get_genre() const { return this->genre; }
        int get_year() const { return this->year; }
        int get_likes() const { return this->likes; }
        std::string get_trailer() const { return this->trailer; }
        std::string get_id() const { return this->id; }

        Movie& operator=(const Movie& other);
        bool operator==(const Movie& other);
        std::string to_string();


        void add_like();
        void play();
};
