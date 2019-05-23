#pragma once
#include "filerepository.h"

class CsvWatchlist : public FileRepository {
    public:
    explicit CsvWatchlist(const string& _file);
};
