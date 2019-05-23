#include "gui.h"
#include "tests.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    test_repo();
    test_ctrl();

    QApplication app(argc, argv);

    FileRepository  database{ "database.csv" };
    FileRepository* watchlist = nullptr;
    Controller      ctrl{ database, watchlist };
    GUI             gui{ ctrl };

    gui.show();

    return app.exec();
}
