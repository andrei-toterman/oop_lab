#pragma once
#include "filerepository.h"
class CsvWatchlist : public FileRepository {
    public:
    CsvWatchlist(const string& _file);
};
