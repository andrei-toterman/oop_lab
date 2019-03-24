#include "controller.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Controller* create_controller(MaterialRepo* repo) {
    Controller* controller = (Controller*) malloc(sizeof (Controller));
    controller->repo = repo;
    controller->oper_len = 0;
    controller->oper_cap = 5;
    controller->operation_stack = (Operation**) calloc((unsigned long long)controller->oper_cap, sizeof(Operation*));
    return controller;
}

int ctrl_find(char id[], Controller* ctrl) {
    return repo_find(id, ctrl->repo);
}

int validate_date(char date_string[]) {
    if (strlen(date_string) > 10) return 0;
    char temp_string[11];
    strcpy(temp_string, date_string);
    char* temp = strtok(temp_string, ".");
    if (atoi(temp) < 1 || atoi(temp) > 31) return 0;
    temp = strtok(NULL, ".");
    if (atoi(temp) < 1 || atoi(temp) > 12) return 0;
    temp = strtok(NULL, ".");
    if (atoi(temp) < 0 || atoi(temp) > 9999) return 0;
    return 1;
}

int validate_material(Material* material) {
    return strlen(material->name) > 0 &&
           strlen(material->supplier) > 0 &&
           validate_date(material->exp_date_string) &&
           material->quantity >= 0;
}

int ctrl_add(char name[], char supplier[], char exp_date_str[], int quantity, Controller* ctrl) {
    Material* material = create_material(name, supplier, exp_date_str, quantity);
    if (!validate_material(material)) {
        destroy_material(material);
        return 0;
    }
    add_operation(create_operation(material, NULL, "add"), ctrl);
    repo_add(material, ctrl->repo);
    return 1;
}

int ctrl_update(char id[], char name[], char supplier[], char exp_date_str[], int quantity, Controller* ctrl) {
    Material* new_material = create_material(name, supplier, exp_date_str, quantity);
    if (!validate_material(new_material)) {
        destroy_material(new_material);
        return 0;
    }
    int index = repo_find(id, ctrl->repo);
    add_operation(create_operation(ctrl->repo->materials[index], new_material, "update"), ctrl);
    return repo_update(id, new_material, ctrl->repo);
}

int ctrl_remove(char id[], Controller* ctrl) {
    int index = repo_find(id, ctrl->repo);
    if (index >= 0) add_operation(create_operation(ctrl->repo->materials[index], NULL, "remove"), ctrl);
    return repo_remove(id, ctrl->repo);
}

Controller* ctrl_filter_materials(int (*filter)(Material*, char []), char args[], Controller* ctrl) {
    MaterialRepo* new_material_repo = repo_filter_materials(filter, args, ctrl->repo);
    return create_controller(new_material_repo);
}

void ctrl_sort_by_quantity(Controller* ctrl, int reverse) {
    repo_sort_by(ctrl->repo, compare_quantity, reverse);
}

void ctrl_sort_by_supplier(Controller* ctrl, int reverse) {
    repo_sort_by(ctrl->repo, compare_supplier, reverse);
}

int compare_supplier(Material* first_material, Material* second_material) {
    return strcmp(first_material->supplier, second_material->supplier) == -1 ? 1 : 0;
}

int compare_quantity(Material* first_material, Material* second_material) {
    return first_material->quantity < second_material->quantity;
}

int substring_in_material_name(Material* material, char substr[]) {
    return strstr(material->name, substr) != NULL;
}

int from_supplier(Material* material, char supplier[]) {
    return strcmp(material->supplier, supplier) == 0;
}

int quantity_less_than(Material* material, char quantity[]) {
    return material->quantity <= atoi(quantity);
}

int expired_material(Material* material, char date[]) {
    int date_int = 0;
    char temp_string[11];
    strcpy(temp_string, date);
    char* temp = strtok(temp_string, ".");
    date_int += atoi(temp);
    temp = strtok(NULL, ".");
    date_int += atoi(temp) * 100;
    temp = strtok(NULL, ".");
    date_int += atoi(temp) * 10000;
    return material->exp_date < date_int;
}

int not_expired_material(Material* material, char date[]) {
    return !expired_material(material, date);
}

void destroy_controller(Controller* ctrl) {
    for (int i = 0; ctrl->operation_stack[i] != NULL; ++i)
        destroy_operation(ctrl->operation_stack[i]);
    free(ctrl->operation_stack);
    free(ctrl);
    ctrl = NULL;
}

void add_operation(Operation* operation, Controller* ctrl) {
    if (ctrl->oper_len == ctrl->oper_cap - 1) {
        ctrl->oper_cap *= 2;
        ctrl->operation_stack = (Operation**) realloc(ctrl->operation_stack, (unsigned long long) ctrl->oper_cap * sizeof(Operation*));
    }
    for (int i = ctrl->oper_len; ctrl->operation_stack[i] != NULL; ++i)
        destroy_operation(ctrl->operation_stack[i]);
    memset(ctrl->operation_stack + ctrl->oper_len, 0, (unsigned long long) (ctrl->oper_cap - ctrl->oper_len) * sizeof(Operation*));
    ctrl->operation_stack[ctrl->oper_len++] = operation;
}

int can_undo(Controller* ctrl) {
    return ctrl->oper_len > 0;
}

int can_redo(Controller* ctrl) {
    return ctrl->operation_stack[ctrl->oper_len] != NULL;
}

void undo(Controller* ctrl) {
    if (ctrl->oper_len == 0) return;
    Operation* operation = ctrl->operation_stack[--ctrl->oper_len];
    switch (*operation->type) {
        case 'a': {
            int index = repo_find(operation->material->id, ctrl->repo);
            if (operation->material->quantity < ctrl->repo->materials[index]->quantity)
                ctrl->repo->materials[index]->quantity -= operation->material->quantity;
            else
                repo_remove(operation->material->id, ctrl->repo);
            break;
        }
        case 'r': {
            repo_add(create_material(operation->material->name,
                                     operation->material->supplier,
                                     operation->material->exp_date_string,
                                     operation->material->quantity),
                     ctrl->repo);
            break;
        }
        case 'u': {
            Material* temp_material = create_material(operation->material->name,
                                                      operation->material->supplier,
                                                      operation->material->exp_date_string,
                                                      operation->material->quantity);
            repo_update(operation->new_material->id,
                        temp_material,
                        ctrl->repo);
            break;
        }
    }
}

void redo(Controller* ctrl) {
    if (ctrl->operation_stack[ctrl->oper_len] == NULL) return;
    Operation* operation = ctrl->operation_stack[ctrl->oper_len++];
    switch (*operation->type) {
        case 'a': {
            repo_add(create_material(operation->material->name,
                                     operation->material->supplier,
                                     operation->material->exp_date_string,
                                     operation->material->quantity),
                     ctrl->repo);
            break;
        }
        case 'r': {
            repo_remove(operation->material->id, ctrl->repo);
            break;
        }
        case 'u': {
            Material* temp_material = create_material(operation->new_material->name,
                                                      operation->new_material->supplier,
                                                      operation->new_material->exp_date_string,
                                                      operation->new_material->quantity);
            repo_update(operation->material->id,
                        temp_material,
                        ctrl->repo);
            break;
        }
    }
}

void test_controller() {
    MaterialRepo* repo = create_material_repo(5);
    Controller* ctrl = create_controller(repo);

    assert(validate_date("10.10.10"));
    assert(!validate_date("10.20.10"));
    assert(!validate_date("100.10.10"));
    assert(!validate_date("1.1.100000"));

    void* m1 = create_material("", "", "123", 0);
    assert(!validate_material(m1));
    destroy_material(m1);

    void* m2 = create_material("abcba", "abbaabc", "1.1.2000", 1);
    assert(validate_material(m2));

    assert(substring_in_material_name(m2, "bcb"));
    assert(!substring_in_material_name(m2, "bdb"));
    assert(expired_material(m2, "2.1.2019"));
    assert(not_expired_material(m2, "2.1.1101"));
    assert(from_supplier(m2, "abbaabc"));
    assert(!from_supplier(m2, "awdwaa"));
    assert(quantity_less_than(m2, "2"));
    assert(!quantity_less_than(m2, "0"));

    Operation* op = create_operation(m2, NULL, "add");
    add_operation(op, ctrl);
    assert(can_undo(ctrl));
    destroy_material(m2);

    assert(!can_redo(ctrl));
    assert(ctrl_add("aaa", "bbb", "12.12.2019", 1, ctrl));
    assert(ctrl_add("aaa", "bbb", "12.12.2019", 1, ctrl));
    assert(ctrl_add("aaa", "ccc", "12.12.2019", 1, ctrl));
    assert(can_undo(ctrl));

    assert(ctrl_find("aaa_bbb_12.12.2019", ctrl) == 0);
    assert(ctrl_find("aaa_ccc_12.12.2019", ctrl) == 1);

    undo(ctrl);
    assert(ctrl_find("aaa_ccc_12.12.2019", ctrl) == -1);
    undo(ctrl);
    assert(ctrl_find("aaa_bbb_12.12.2019", ctrl) == 0);
    undo(ctrl);
    assert(ctrl_find("aaa_bbb_12.12.2019", ctrl) == -1);
    redo(ctrl);
    assert(ctrl_find("aaa_bbb_12.12.2019", ctrl) == 0);
    redo(ctrl);
    redo(ctrl);
    assert(ctrl_find("aaa_ccc_12.12.2019", ctrl) == 1);

    assert(ctrl_update("aaa_ccc_12.12.2019", "bbb", "ccc", "1.1.2010", 3, ctrl));
    assert(ctrl_find("bbb_ccc_1.1.2010", ctrl) == 1);
    undo(ctrl);
    assert(ctrl_find("aaa_ccc_12.12.2019", ctrl) == 1);
    redo(ctrl);
    assert(!ctrl_update("aaa_bbb_12.12.2019", "bbb", "ccc", "13.13.2010", 3, ctrl));

    assert(ctrl_remove("aaa_bbb_12.12.2019", ctrl));
    assert(ctrl->repo->len == 1);
    undo(ctrl);
    assert(ctrl->repo->len == 2);
    redo(ctrl);
    assert(!ctrl_remove("aaa_bbb_12.12.2019", ctrl));

    ctrl_add("a", "x", "1.1.1", 1, ctrl);
    ctrl_add("b", "y", "1.1.1", 3, ctrl);
    ctrl_add("c", "x", "1.1.1", 2, ctrl);
    ctrl_add("d", "y", "1.1.1", 5, ctrl);

    Controller* temp_ctrl = ctrl_filter_materials(from_supplier, "x", ctrl);
    assert(repo_find("a_x_1.1.1", temp_ctrl->repo) == 0);
    assert(repo_find("c_x_1.1.1", temp_ctrl->repo) == 1);
    assert(repo_find("b_y_1.1.1", temp_ctrl->repo) == -1);

    ctrl_sort_by_quantity(ctrl, 0);
    for (int i = 0; i < ctrl->repo->len - 1; ++i)
        assert(ctrl->repo->materials[i]->quantity <= ctrl->repo->materials[i + 1]->quantity);

    ctrl_sort_by_supplier(ctrl, 1);
    for (int i = 0; i < ctrl->repo->len - 1; ++i)
        assert(strcmp(ctrl->repo->materials[i]->supplier, ctrl->repo->materials[i + 1]->supplier) >= 0);

    destroy_repo(temp_ctrl->repo, 0);
    destroy_repo(repo, 1);
    destroy_controller(temp_ctrl);
    destroy_controller(ctrl);

}
