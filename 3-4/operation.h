#pragma once
#include "material.h"

/*
defines an object which stores a CRUD operation with a Material and, if necessary, a new Material
material: pointer to Material with which an operation has been made
new_material: pointer to Material with which an update operation has been made
type: string which defines the type of operation i.e add, remove, update
*/
typedef struct {
    Material* material;
    Material* new_material;
    char type[10];
} Operation;

/*
creates a new Operation object with the given attributes
input: material: pointer to the initial Material with which an operation has been made
       new_material: pointer to the new Material with which an update operation has been made
       type: type of operation
output: pointer to the newly created Operation
*/
Operation* create_operation(Material* material, Material* new_material, char type[]);
/*
destroys a given Operation by freeing the memory it occupies
input: pointer to the Operation
output: -
*/
void destroy_operation(Operation* operation);
