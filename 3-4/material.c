#include "material.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int date_string_to_date_int(char date_string[]) {
    if (!strcmp(date_string, "")) return 0;
    int date_int = 0;
    char temp_string[11];
    strcpy(temp_string, date_string);
    char* temp = strtok(temp_string, ".");
    if (temp == NULL) return 0;
    date_int += atoi(temp);
    temp = strtok(NULL, ".");
    if (temp == NULL) return 0;
    date_int += atoi(temp) * 100;
    temp = strtok(NULL, ".");
    if (temp == NULL) return 0;
    date_int += atoi(temp) * 10000;
    return date_int;
}

Material* create_material(char name[], char supplier[], char exp_date_string[], int quantity) {
    Material* material = (Material*) malloc(sizeof(Material));

    material->name = (char*) malloc(strlen(name) + 1);
    material->supplier = (char*) malloc(strlen(supplier) + 1);
    material->exp_date_string = (char*) malloc(11);

    strcpy(material->name, name);
    strcpy(material->supplier, supplier);
    strcpy(material->exp_date_string, exp_date_string);

    material->exp_date = date_string_to_date_int(exp_date_string);
    material->quantity = quantity;

    material->id = (char*) malloc(strlen(material->name) + strlen(material->supplier) + strlen(material->exp_date_string) + 3);
    strcpy(material->id, material->name);
    strcat(material->id, "_");
    strcat(material->id, material->supplier);
    strcat(material->id, "_");
    strcat(material->id, material->exp_date_string);

    return material;
}

void destroy_material(Material* material) {
    if (material == NULL) return;
    free(material->name);
    free(material->supplier);
    free(material->exp_date_string);
    free(material->id);
    free(material);
    material = NULL;
}

void overwrite_material(Material* dest_material, Material* source_material) {
    dest_material->name = (char*) realloc(dest_material->name, strlen(source_material->name) + 1);
    strcpy(dest_material->name, source_material->name);
    dest_material->supplier = (char*) realloc(dest_material->supplier, strlen(source_material->supplier) + 1);
    strcpy(dest_material->supplier, source_material->supplier);
    dest_material->exp_date_string = (char*) realloc(dest_material->exp_date_string, 11);
    strcpy(dest_material->exp_date_string, source_material->exp_date_string);
    dest_material->id = (char*) realloc(dest_material->id, strlen(source_material->id) + 1);
    strcpy(dest_material->id, source_material->id);
    dest_material->quantity = source_material->quantity;
    dest_material->exp_date = source_material->exp_date;
}

void test_material() {
    Material* material = create_material("Chocolate", "Hershey", "20.3.2019", 5);
    Material* copy = create_material("", "", "", 0);
    overwrite_material(copy, material);
    assert(strcmp(material->name, "Chocolate") == 0);
    assert(strcmp(copy->name, "Chocolate") == 0);
    assert(strcmp(material->supplier, "Hershey") == 0);
    assert(strcmp(copy->supplier, "Hershey") == 0);
    assert(material->quantity == 5);
    assert(copy->quantity == 5);
    assert(strcmp(material->exp_date_string, "20.3.2019") == 0);
    assert(strcmp(copy->exp_date_string, "20.3.2019") == 0);
    assert(material->exp_date == 20190320);
    assert(copy->exp_date == 20190320);
    assert(strcmp(material->id, "Chocolate_Hershey_20.3.2019") == 0);
    assert(strcmp(copy->id, "Chocolate_Hershey_20.3.2019") == 0);

    destroy_material(material);
    destroy_material(copy);
}
