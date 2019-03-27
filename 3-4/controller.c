#include "controller.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Controller* create_controller(MaterialRepo* repo) {
    Controller* ctrl = (Controller*) malloc(sizeof (Controller));
    ctrl->repo = repo;
    ctrl->oper_len = 0;
    ctrl->oper_cap = 5;
    ctrl->operation_stack = (Operation**) calloc((unsigned long long)ctrl->oper_cap, sizeof(Operation*));
    return ctrl;
}

static int validate_date(char date_string[]) {
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

int ctrl_find(char id[], Controller* ctrl) {
    return repo_find(id, ctrl->repo);
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
    if (index == -1) destroy_material(new_material);
    else add_operation(create_operation(ctrl->repo->materials[index], new_material, "update"), ctrl);
    return repo_update(id, new_material, ctrl->repo);
}

int ctrl_remove(char id[], Controller* ctrl) {
    int index = repo_find(id, ctrl->repo);
    if (index >= 0) add_operation(create_operation(ctrl->repo->materials[index], NULL, "remove"), ctrl);
    return repo_remove(id, ctrl->repo);
}

static int compare_supplier(Material* first_material, Material* second_material) {
    return strcmp(first_material->supplier, second_material->supplier) == -1 ? 1 : 0;
}

static int compare_quantity(Material* first_material, Material* second_material) {
    return first_material->quantity < second_material->quantity;
}

static int substring_in_material_name(Material* material, char substr[]) {
    return strstr(material->name, substr) != NULL;
}

static int from_supplier(Material* material, char supplier[]) {
    return strcmp(material->supplier, supplier) == 0;
}

static int quantity_less_than(Material* material, char quantity[]) {
    return material->quantity <= atoi(quantity);
}

static int expired_material(Material* material, char date[]) {
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

static int not_expired_material(Material* material, char date[]) {
    return !expired_material(material, date);
}

MaterialRepo* filter_by_name_substring_and_expired(char substr[], char date[], Controller* ctrl) {
    if (!validate_date(date)) return create_material_repo(0);
    MaterialRepo* filtered_repo1 = repo_filter_materials(substring_in_material_name, substr, ctrl->repo);
    MaterialRepo* filtered_repo2 = repo_filter_materials(expired_material, date, filtered_repo1);
    destroy_repo(filtered_repo1, 0);
    return filtered_repo2;
}

MaterialRepo* filter_by_supplier_sort_by_quantity(char supplier[], char quantity[], int reverse, Controller* ctrl) {
    MaterialRepo* filtered_repo1 = repo_filter_materials(from_supplier, supplier, ctrl->repo);
    MaterialRepo* filtered_repo2 = repo_filter_materials(quantity_less_than, quantity, filtered_repo1);
    destroy_repo(filtered_repo1, 0);
    repo_sort_by(compare_quantity, reverse, filtered_repo2);
    return filtered_repo2;
}

MaterialRepo* filter_by_name_substring_sort_by_supplier(char substr[], int reverse, Controller* ctrl) {
    MaterialRepo* filtered_repo = repo_filter_materials(substring_in_material_name, substr, ctrl->repo);
    repo_sort_by(compare_supplier, reverse, filtered_repo);
    return filtered_repo;
}

MaterialRepo* filter_by_not_expired(char date[], Controller* ctrl) {
    if (!validate_date(date)) return create_material_repo(0);
    return repo_filter_materials(not_expired_material, date, ctrl->repo);
}

void destroy_controller(Controller* ctrl) {
    for (int i = 0; ctrl->operation_stack[i] != NULL; ++i)
        destroy_operation(ctrl->operation_stack[i]);
    free(ctrl->operation_stack);
    free(ctrl);
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

static void test_create() {
    Controller* ctrl = create_controller(create_material_repo(1));
    assert(ctrl->oper_cap == 5);
    assert(ctrl->oper_len == 0);
    destroy_repo(ctrl->repo, 0);
    destroy_controller(ctrl);
}

static void test_find() {
    Controller* ctrl = create_controller(create_material_repo(1));
    populate_repo(ctrl->repo);
    assert(ctrl_find("Milk_Napolact_20.2.2019", ctrl) == 1);
    assert(ctrl_find("aaaa", ctrl) == -1);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_validate_date() {
    assert(validate_date("11.11.11"));
    assert(!validate_date("111"));
    assert(!validate_date("32.12.12"));
    assert(!validate_date("1.13.12"));
    assert(!validate_date("1.1.111111"));
}

static void test_validate_material() {
    Material* m1 = create_material("a", "a", "1.1.1", 1);
    Material* m2 = create_material("", "a", "1.1.1", 1);
    Material* m3 = create_material("a", "", "1.1.1", 1);
    Material* m4 = create_material("a", "a", "111", 1);
    Material* m5 = create_material("a", "a", "111", -1);
    assert(validate_material(m1));
    assert(!validate_material(m2));
    assert(!validate_material(m3));
    assert(!validate_material(m4));
    assert(!validate_material(m5));
    destroy_material(m1);
    destroy_material(m2);
    destroy_material(m3);
    destroy_material(m4);
    destroy_material(m5);
}

static void test_add() {
    Controller* ctrl = create_controller(create_material_repo(1));
    assert(ctrl_add("a", "a", "1.1.1", 1, ctrl));
    assert(!ctrl_add("", "a", "1.1.1", 1, ctrl));
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_update() {
    Controller* ctrl = create_controller(create_material_repo(1));
    ctrl_add("a", "a", "1.1.1", 1, ctrl);
    assert(!ctrl_update("aaa", "b", "b", "2.2.2", 2, ctrl));
    assert(!ctrl_update("a_a_1.1.1", "b", "b", "13.13.13", 2, ctrl));
    assert(ctrl_update("a_a_1.1.1", "b", "b", "2.2.2", 2, ctrl));
    assert(strcmp(ctrl->repo->materials[0]->name, "b") == 0);
    assert(strcmp(ctrl->repo->materials[0]->supplier, "b") == 0);
    assert(strcmp(ctrl->repo->materials[0]->exp_date_string, "2.2.2") == 0);
    assert(strcmp(ctrl->repo->materials[0]->id, "b_b_2.2.2") == 0);
    assert(ctrl->repo->materials[0]->quantity == 2);
    assert(ctrl->repo->materials[0]->exp_date == 20202);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_remove() {
    Controller* ctrl = create_controller(create_material_repo(1));
    ctrl_add("a", "a", "1.1.1", 1, ctrl);
    ctrl_add("a", "b", "1.1.1", 1, ctrl);
    assert(!ctrl_remove("b_b_2.2.2", ctrl));
    assert(ctrl_remove("a_a_1.1.1", ctrl));
    assert(ctrl->repo->len == 1);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_filter_by_name_substring_and_expired() {
    Controller* ctrl = create_controller(create_material_repo(10));
    populate_repo(ctrl->repo);
    MaterialRepo* new_repo = filter_by_name_substring_and_expired("a", "10.10.2020", ctrl);
    assert(repo_find("Chocolate_Hershey_15.2.2019", new_repo) > -1);
    assert(repo_find("Milk_Napolact_20.2.2019", new_repo) == -1);
    assert(repo_find("Eggs_Chicken_3.4.2018", new_repo) == -1);
    assert(repo_find("Yeast_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Flour_Bakersville_14.12.2018", new_repo) == -1);
    assert(repo_find("Sugar_Crystal_13.10.2019", new_repo) > -1);
    assert(repo_find("Eggs_Chicken_3.1.2019", new_repo) == -1);
    assert(repo_find("Baking soda_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Vanilla_Dr.Oetker_20.9.2020", new_repo) > -1);
    assert(repo_find("Cinnamon_Dr.Oetker_3.3.2021", new_repo) == -1);
    destroy_repo(new_repo, 0);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_filter_by_not_expired() {
    Controller* ctrl = create_controller(create_material_repo(10));
    populate_repo(ctrl->repo);
    MaterialRepo* new_repo = filter_by_not_expired("12.12.2019", ctrl);
    assert(repo_find("Chocolate_Hershey_15.2.2019", new_repo) == -1);
    assert(repo_find("Milk_Napolact_20.2.2019", new_repo) == -1);
    assert(repo_find("Eggs_Chicken_3.4.2018", new_repo) == -1);
    assert(repo_find("Yeast_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Flour_Bakersville_14.12.2018", new_repo) == -1);
    assert(repo_find("Sugar_Crystal_13.10.2019", new_repo) == -1);
    assert(repo_find("Eggs_Chicken_3.1.2019", new_repo) == -1);
    assert(repo_find("Baking soda_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Vanilla_Dr.Oetker_20.9.2020", new_repo) > -1);
    assert(repo_find("Cinnamon_Dr.Oetker_3.3.2021", new_repo) > -1);
    destroy_repo(new_repo, 0);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_filter_by_supplier_sort_by_quantity() {
    Controller* ctrl = create_controller(create_material_repo(10));
    populate_repo(ctrl->repo);
    MaterialRepo* new_repo = filter_by_supplier_sort_by_quantity("Bakersville", "30", 0, ctrl);
    assert(repo_find("Chocolate_Hershey_15.2.2019", new_repo) == -1);
    assert(repo_find("Milk_Napolact_20.2.2019", new_repo) == -1);
    assert(repo_find("Eggs_Chicken_3.4.2018", new_repo) == -1);
    assert(repo_find("Yeast_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Flour_Bakersville_14.12.2018", new_repo) > -1);
    assert(repo_find("Sugar_Crystal_13.10.2019", new_repo) == -1);
    assert(repo_find("Eggs_Chicken_3.1.2019", new_repo) == -1);
    assert(repo_find("Baking soda_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Vanilla_Dr.Oetker_20.9.2020", new_repo) == -1);
    assert(repo_find("Cinnamon_Dr.Oetker_3.3.2021", new_repo) == -1);
    for (int i = 0; i < new_repo->len -1; ++i)
        assert(new_repo->materials[i]->quantity <= new_repo->materials[i + 1]->quantity);
    destroy_repo(new_repo, 0);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_filter_by_name_substring_sort_by_supplier() {
    Controller* ctrl = create_controller(create_material_repo(10));
    populate_repo(ctrl->repo);
    MaterialRepo* new_repo = filter_by_name_substring_sort_by_supplier("a", 1, ctrl);
    assert(repo_find("Chocolate_Hershey_15.2.2019", new_repo) > -1);
    assert(repo_find("Milk_Napolact_20.2.2019", new_repo) == -1);
    assert(repo_find("Eggs_Chicken_3.4.2018", new_repo) == -1);
    assert(repo_find("Yeast_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Flour_Bakersville_14.12.2018", new_repo) == -1);
    assert(repo_find("Sugar_Crystal_13.10.2019", new_repo) > -1);
    assert(repo_find("Eggs_Chicken_3.1.2019", new_repo) == -1);
    assert(repo_find("Baking soda_Bakersville_10.1.2020", new_repo) > -1);
    assert(repo_find("Vanilla_Dr.Oetker_20.9.2020", new_repo) > -1);
    assert(repo_find("Cinnamon_Dr.Oetker_3.3.2021", new_repo) > -1);
    for (int i = 0; i < new_repo->len -1; ++i)
        assert(strcmp(new_repo->materials[i]->supplier, new_repo->materials[i + 1]->supplier) >= 0);
    destroy_repo(new_repo, 0);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

static void test_undo_redo() {
    Controller* ctrl = create_controller(create_material_repo(5));
    assert(ctrl->oper_cap == 5);
    assert(ctrl->oper_len == 0);
    ctrl_add("a", "a", "1.1.1", 1, ctrl);
    ctrl_update("a_a_1.1.1", "b", "b", "2.2.2", 2, ctrl);
    ctrl_add("c", "c", "3.3.3", 3, ctrl);
    ctrl_remove("b_b_2.2.2", ctrl);
    ctrl_update("c_c_3.3.3", "d", "d", "4.4.4", 4, ctrl);
    ctrl_add("d", "d", "4.4.4", 4, ctrl);
    assert(ctrl->oper_cap == 10);
    assert(ctrl->oper_len == 6);
    undo(ctrl);
    assert(ctrl_find("d_d_4.4.4", ctrl) == 0);
    undo(ctrl);
    assert(ctrl_find("c_c_3.3.3", ctrl) == 0);
    undo(ctrl);
    assert(ctrl_find("b_b_2.2.2", ctrl) == 1);
    undo(ctrl);
    assert(ctrl_find("c_c_3.3.3", ctrl) == -1);
    undo(ctrl);
    assert(ctrl_find("a_a_1.1.1", ctrl) == 0);
    undo(ctrl);
    assert(ctrl->repo->len == 0);
    assert(ctrl->oper_len == 0);
    redo(ctrl);
    assert(ctrl_find("a_a_1.1.1", ctrl) == 0);
    redo(ctrl);
    assert(ctrl_find("b_b_2.2.2", ctrl) == 0);
    redo(ctrl);
    assert(ctrl_find("c_c_3.3.3", ctrl) == 1);
    redo(ctrl);
    assert(ctrl_find("b_b_2.2.2", ctrl) == -1);
    redo(ctrl);
    assert(ctrl_find("d_d_4.4.4", ctrl) == 0);
    assert(ctrl->oper_len == 5);
    undo(ctrl);
    undo(ctrl);
    undo(ctrl);
    undo(ctrl);
    ctrl_add("e", "e", "5.5.5", 5, ctrl);
    assert(ctrl->oper_len == 2);
    assert(ctrl->operation_stack[2] == NULL);
    destroy_repo(ctrl->repo, 1);
    destroy_controller(ctrl);
}

void test_controller() {
    test_create();
    test_find();
    test_validate_date();
    test_validate_material();
    test_add();
    test_update();
    test_remove();
    test_filter_by_name_substring_and_expired();
    test_filter_by_not_expired();
    test_filter_by_supplier_sort_by_quantity();
    test_filter_by_name_substring_sort_by_supplier();
    test_undo_redo();
}

