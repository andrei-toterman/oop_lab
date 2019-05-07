#include "filerepository.h"
#include "exceptions.h"
#include <fstream>

void FileRepository::read_from_file() {
    std::ifstream f(this->file);
    Movie         movie;
    if (!f.is_open()) throw RepoException("can't open file");
    while (!f.eof()) {
        f >> movie;
        Repository::add(movie);
    }
    f.close();
}

void FileRepository::write_to_file() {
    std::ofstream f(this->file);
    if (!f.is_open()) throw RepoException("can't open file");
    for (Movie& movie : this->movies) {
        f << movie;
        if (!(movie == this->movies.back())) f << '\n';
    }
    f.close();
}

FileRepository::FileRepository(const std::string& _file) : file{_file} {
    this->read_from_file();
}

void FileRepository::add(const Movie& movie) {
    Repository::add(movie);
    this->write_to_file();
}

void FileRepository::remove(const std::string& id) {
    Repository::remove(id);
    this->write_to_file();
}

void FileRepository::update(const std::string& id, const Movie& new_movie) {
    Repository::update(id, new_movie);
    this->write_to_file();
}

void FileRepository::add_like(const std::string& id) {
    Repository::add_like(id);
    this->write_to_file();
}

void FileRepository::open() {
    this->write_to_file();
    system(("start " + this->file).c_str());
}
