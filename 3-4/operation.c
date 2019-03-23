#include "operation.h"
#include <stdlib.h>
#include <string.h>

Operation* create_operation(Material* material, Material* new_material, char type[]) {
    Operation* operation = (Operation*) malloc(sizeof(Operation));
    operation->material = create_material(material->name,
                                          material->supplier,
                                          material->exp_date_string,
                                          material->quantity);
    if (new_material == NULL) operation->new_material = NULL;
    else operation->new_material = create_material(new_material->name,
                                                   new_material->supplier,
                                                   new_material->exp_date_string,
                                                   new_material->quantity);
    strcpy(operation->type, type);
    return operation;
}

void destroy_operation(Operation* operation) {
    if (operation == NULL) return;
    destroy_material(operation->material);
    destroy_material(operation->new_material);
    free(operation);
    operation = NULL;
}
