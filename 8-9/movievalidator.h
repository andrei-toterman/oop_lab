#pragma once
#include "exceptions.h"
#include "movie.h"

class MovieValidator {
    public:
    MovieValidator() = default;
    // checks if a given Movie has valid attributes
    void validate_movie(const Movie& movie) {
        std::string error = "invalid movie:\n";
        if (movie.get_title().empty()) error += "title must not be empty\n";
        if (movie.get_genre().empty()) error += "genre must not be empty\n";
        if (movie.get_year() < 0) error += "year must not be a positive number\n";
        if (movie.get_likes() < 0) error += "number of likes must be a positive number\n";
        if (movie.get_trailer().empty()) error += "link to trailer must not be empty\n";
        if (movie.get_trailer().find("www.") != 0 && movie.get_trailer().find("https://") != 0 &&
            movie.get_trailer().find("http://") != 0)
            error += "url must start with www., http:// or https://\n";
        if (error != "invalid movie:\n") throw MovieException(error);
    }
};
