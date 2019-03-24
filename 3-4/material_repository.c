#include "material_repository.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

MaterialRepo* create_material_repo(int init_cap) {
    MaterialRepo* repo = (MaterialRepo*) malloc(sizeof(MaterialRepo));
    repo->cap = init_cap;
    repo->materials = (Material**) malloc((unsigned long long) init_cap * sizeof(Material*));
    repo->len = 0;
    return repo;
}

int repo_find(char id[], MaterialRepo* repo) {
    for (int i = 0; i < repo->len; ++i)
        if (strcmp(repo->materials[i]->id, id) == 0) return i;
    return -1;
}

void repo_add(Material* material, MaterialRepo* repo) {
    int index = repo_find(material->id, repo);

    if (index >= 0) {
        repo->materials[index]->quantity += material->quantity;
        destroy_material(material);
    } else {
        if (repo->len == repo->cap) {
            repo->cap *= 2;
            repo->materials = (Material**) realloc(repo->materials, (unsigned long long) repo->cap * sizeof(Material*));
        }
        repo->materials[repo->len++] = material;
    }
}

int repo_update(char id[], Material* new_material, MaterialRepo* repo) {
    int index = repo_find(id, repo);
    if (index < 0) return 0;
    overwrite_material(repo->materials[index], new_material);
    destroy_material(new_material);
    return 1;
}

int repo_remove(char id[], MaterialRepo* repo) {
    int index = repo_find(id, repo);
    if (index < 0) return 0;
    for (int i = index; i < repo->len - 1; ++i)
        overwrite_material(repo->materials[i], repo->materials[i + 1]);
    destroy_material(repo->materials[--repo->len]);
    return 1;
}

void destroy_repo(MaterialRepo* repo, int with_materials) {
    if (repo == NULL) return;
    for (int i = 0; i < repo->len && with_materials; ++i)
        destroy_material(repo->materials[i]);
    free(repo->materials);
    free(repo);
}

MaterialRepo* repo_filter_materials(int (*filter)(Material*, char[]), char args[], MaterialRepo* repo) {
    MaterialRepo* new_material_repo = create_material_repo(repo->len);
    for (int i = 0; i < repo->len; ++i)
        if (filter(repo->materials[i], args))
            repo_add(repo->materials[i], new_material_repo);
    return new_material_repo;
}

void repo_sort_by(int (*compare)(Material*, Material*), int reverse, MaterialRepo* repo) {
    for (int i = 0; i < repo->len - 1; ++i) {
        int min = i;
        for (int j = i + 1; j < repo->len; ++j)
            if (compare(repo->materials[j], repo->materials[min]) != reverse)
                min = j;
        Material* temp = repo->materials[min];
        repo->materials[min] = repo->materials[i];
        repo->materials[i] = temp;
    }
}

void repo_to_string(MaterialRepo* repo) {
    if (repo->len == 0) {
        printf("\nno materials found\n");
        return;
    }
    int max_name_length = 4;
    int max_supplier_length = 8;

    for (int i = 0; i < repo->len; ++i) {
        int name_length = (int) strlen(repo->materials[i]->name);
        int supplier_length = (int) strlen(repo->materials[i]->supplier);

        if (max_name_length < name_length) max_name_length = name_length;
        if (max_supplier_length < supplier_length) max_supplier_length = supplier_length;
    }

    printf("\nName");
    for (int i = 4; i < max_name_length; ++i) printf(" ");
    printf(" | Supplier");
    for (int i = 8; i < max_supplier_length; ++i) printf(" ");
    printf(" | Expiration date");
    printf(" | Quantity\n");
    for (int i = -35; i < max_name_length + max_supplier_length; ++i) printf("-");

    for (int i = 0; i < repo->len; ++i) {
        printf("\n%s", repo->materials[i]->name);
        for (int j = (int) strlen(repo->materials[i]->name); j < max_name_length; ++j) printf(" ");
        printf(" | %s", repo->materials[i]->supplier);
        for (int j = (int) strlen(repo->materials[i]->supplier); j < max_supplier_length; ++j) printf(" ");
        printf(" | %s", repo->materials[i]->exp_date_string);
        for (int j = (int) strlen(repo->materials[i]->exp_date_string); j < 15; ++j) printf(" ");
        printf(" | %d", repo->materials[i]->quantity);
    }
    printf("\n");
}

static void test_create() {
    MaterialRepo* repo = create_material_repo(5);
    assert(repo->cap == 5);
    assert(repo->len == 0);
    populate_repo(repo);
    assert(repo->cap == 10);
    assert(repo->len == 10);
    destroy_repo(repo, 1);
}

static void test_find() {
    MaterialRepo* repo = create_material_repo(11);
    populate_repo(repo);
    assert(repo_find("Milk_Napolact_20.2.2019", repo) == 1);
    assert(repo_find("aaa", repo) == -1);
    destroy_repo(repo, 1);
}

static void test_add() {
    MaterialRepo* repo = create_material_repo(5);
    repo_add(create_material("a", "a", "1.1.1", 1), repo);
    assert(repo->len == 1);
    destroy_repo(repo, 1);
}

static void test_add_existing() {
    MaterialRepo* repo = create_material_repo(5);
    repo_add(create_material("a", "a", "1.1.1", 1), repo);
    repo_add(create_material("a", "a", "1.1.1", 2), repo);
    assert(repo->len == 1);
    assert(repo->materials[0]->quantity == 3);
    destroy_repo(repo, 1);
}

static void test_remove() {
    MaterialRepo* repo = create_material_repo(5);
    repo_add(create_material("a", "a", "1.1.1", 1), repo);
    repo_remove("a_a_1.1.1", repo);
    assert(repo->len == 0);
    destroy_repo(repo, 1);
}

static void test_update() {
    MaterialRepo* repo = create_material_repo(5);
    Material* m = create_material("a", "a", "1.1.1", 1);
    repo_add(m, repo);
    repo_update("a_a_1.1.1", create_material("b", "c", "2.2.2", 2), repo);
    assert(strcmp(m->name, "b") == 0);
    assert(strcmp(m->supplier, "c") == 0);
    assert(strcmp(m->exp_date_string, "2.2.2") == 0);
    assert(m->quantity == 2);
    assert(m->exp_date == 20202);
    assert(strcmp(m->id, "b_c_2.2.2") == 0);
    destroy_repo(repo, 1);
}

void test_repo() {
    test_create();
    test_find();
    test_add();
    test_add_existing();
    test_remove();
    test_update();
}

void populate_repo(MaterialRepo* repo) {
    repo_add(create_material("Chocolate",   "Hershey",     "15.2.2019",  10), repo);
    repo_add(create_material("Milk",        "Napolact",    "20.2.2019",   5), repo);
    repo_add(create_material("Eggs",        "Chicken",     "3.4.2018",   13), repo);
    repo_add(create_material("Yeast",       "Bakersville", "10.1.2020",  10), repo);
    repo_add(create_material("Flour",       "Bakersville", "14.12.2018", 23), repo);
    repo_add(create_material("Sugar",       "Crystal",     "13.10.2019", 30), repo);
    repo_add(create_material("Eggs",        "Chicken",     "3.1.2019",   16), repo);
    repo_add(create_material("Baking soda", "Bakersville", "10.1.2020",  13), repo);
    repo_add(create_material("Vanilla",     "Dr.Oetker",   "20.9.2020",  25), repo);
    repo_add(create_material("Cinnamon",    "Dr.Oetker",   "3.3.2021",   40), repo);
}
