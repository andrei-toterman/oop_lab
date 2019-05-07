#pragma once
#include "filerepository.h"

class HtmlWatchlist : public FileRepository {
    public:
    explicit HtmlWatchlist(const string& _file);
    void write_to_file() override;
};
