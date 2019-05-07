#include "htmlwatchlist.h"
#include "tests.h"
#include "ui.h"

int main() {
    //    test_ctrl();
    //    test_repo();

    FileRepository  database{"database.csv"};
    FileRepository* watchlist = nullptr;
    Controller      ctrl{database, watchlist};
    UI         ui{ ctrl };

    ui.start();

    return 0;
}
