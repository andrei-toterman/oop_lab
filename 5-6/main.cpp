#include "tests.h"
#include "ui.h"

int main() {
    test_ctrl();
    test_repo();
    test_dv();

    MovieRepo  repo;
    MovieRepo  watchlist;
    Controller ctrl{repo, watchlist};
    UI         ui{ctrl};

    repo.populate();
    ui.start();

    return 0;
}
