#include "material.h"
#include "material_repository.h"
#include "controller.h"
#include "ui.h"

int main() {
    test_material();
    test_repo();
    test_controller();

    MaterialRepo* repo = create_material_repo(10);
    populate_repo(repo);
    Controller* ctrl = create_controller(repo);
    UI* ui = create_ui(ctrl);

    start_ui(ui);

    destroy_repo(repo, 1);
    destroy_ui(ui);
    return 0;
}
