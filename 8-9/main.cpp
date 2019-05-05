#include "repositoryhtml.h"
#include "tests.h"
#include "ui.h"

int main() {
    test_ctrl();
    test_repo();

    MovieRepo  database{ "database.csv" };
    MovieRepo* watchlist;
    Controller ctrl{ database, watchlist };
    UI         ui{ ctrl };

    ui.start();

    return 0;
}
