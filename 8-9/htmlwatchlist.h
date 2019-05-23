#pragma once
#include "filerepository.h"

class HtmlWatchlist : public FileRepository {
    public:
    explicit HtmlWatchlist(const string& _file);
    // writes the elements in the repository to a HTML table
    void write_to_file() override;
};
