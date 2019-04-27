﻿#include "ui.h"
#include "tests.h"

int main() {
    test_ctrl();
    test_repo();

    MovieRepo repo;
    repo.populate();
    MovieRepo watchlist;
    Controller ctrl{ repo, watchlist };
    UI ui{ ctrl };

    ui.start();

    return 0;
}