#include "ui.h"

int main() {
    test_material();
    test_repo();
    test_controller();

    MaterialRepo* repo = create_material_repo(10);
    Controller*   ctrl = create_controller(repo);
    UI*           ui   = create_ui(ctrl);

    populate_repo(repo);
    start_ui(ui);

    destroy_repo(repo, 1);
    destroy_ui(ui);
    return 0;
}
