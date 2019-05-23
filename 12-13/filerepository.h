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
    virtual ~FileRepository() override = default;

    void add(const Movie& movie) override;
    void remove(const string& id) override;
    void update(const string& id, const Movie& new_movie) override;
    void add_like(const string& id);
    void open();
};
