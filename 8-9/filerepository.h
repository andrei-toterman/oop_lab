#pragma once
#include "repository.h"

class FileRepository : public Repository {
    protected:
    string       file;
    void         read_from_file();
    virtual void write_to_file();

    public:
    FileRepository() = default;
    explicit FileRepository(const string& _file);
    virtual ~FileRepository() = default;

    void add(const Movie& movie);
    void remove(const string& id);
    void update(const string& id, const Movie& new_movie);
    void add_like(const string& id);
    void open();
};
