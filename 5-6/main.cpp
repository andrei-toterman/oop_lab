#include <iostream>
#include "repository.h"
#include "controller.h"
#include "ui.h"

using namespace std;

int main() {
    test_ctrl();
    test_repo();

    MovieRepo repo;
    repo.populate();
    Controller ctrl{ repo };
    UI ui{ ctrl };

    ui.start();

    return 0;
}
