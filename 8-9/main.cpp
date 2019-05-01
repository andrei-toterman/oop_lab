#include "ui.h"
#include "tests.h"

int main() {
    test_ctrl();
    test_repo();

    MovieRepo  database{ "database.csv" };
    MovieRepo  watchlist{ "watchlist.csv" };
    Controller ctrl{ database, watchlist };
    UI         ui{ ctrl };

    ui.start();

    return 0;
}
