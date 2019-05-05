#pragma once
#include "repository.h"

class MovieRepoHtml : public MovieRepo {
    private:
    string file_html;

    public:
    explicit MovieRepoHtml(const string& _file);
    void write_to_file() override;
    void open() override;
};
