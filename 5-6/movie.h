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
        Movie(const std::string& id);
        Movie(const std::string& title, const std::string& genre, int year, int likes, const std::string& trailer);
        Movie(const Movie& other);
        ~Movie();

        std::string get_title() const { return this->title; }
        std::string get_genre() const { return this->genre; }
        int get_year() const { return this->year; }
        int get_likes() const { return this->likes; }
        std::string get_trailer() const { return this->trailer; }

        void set_title(const std::string& _title);
        void set_genre(const std::string& _genre);
        void set_year(int _year);
        void set_likes(int _likes);
        void set_trailer(const std::string& _trailer);

        Movie& operator=(const Movie& other);
        bool operator==(const Movie& other);
        std::string to_string();

        void play();
};
