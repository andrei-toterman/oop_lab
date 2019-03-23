#pragma once

/*
defines an object that stores information about a material in a bakery
name: string name of the material
supplier: string supplier for the materil
quantity: quantity in which the material is found
exp_date: expiration date of the material
exp_date_string: string representation of the expiration date
id: identifier defined by the name, supplier and expiration date of the material
*/
typedef struct {
    char* name;
    char* supplier;
    int quantity;
    int exp_date;
    char* exp_date_string;
    char* id;
} Material;

/*
creates a Material object with the given attributes
input: name: name of the Material
       supplier: supplier of the Material
       exp_date_string: expiration date of the Material, in the form "dd.mm.yyyy"
       quantity: quantity of the Material
output: pointer to the newly created Material
*/
Material* create_material(char name[], char supplier[], char exp_date_string[10], int quantity);
/*
destroys a given Material by freeing the memory it occupies
input: pointer to the Material
output: -
*/
void destroy_material(Material* material);

/*
overwrites the attributes of a given Material with the ones from another given Material
input: dest_material: pointer to Material which must be overwritten
       source_material: pointer to Material from which to overwrite
output: -
*/
void overwrite_material(Material* dest_material, Material* source_material);
/*
executes a series of tests for the methods of the Material
input: -
output: -
*/
void test_material(void);
