#pragma once
#include "material.h"

/*
defines an object that stores a dynamic list of Material object pointers
materials: pointer to a list of pointers to Materials
len: the current length of the list
cap: the current capacity of the list
*/
typedef struct {
    Material** materials;
    int len;
    int cap;
} MaterialRepo;

/*
creates a MaterialRepo object with a given initial capacity
input: init_cap: initial capacity of the list
output: a pointer to the newly created MaterialRepo
*/
MaterialRepo* create_material_repo(int init_cap);
/*
destroys a given MaterialRepo by freeing the memory it occupies
input: repo: pointer to the MaterialRepo
       with_materials: whether to also free the Materials from the list or not
output: -
*/
void destroy_repo(MaterialRepo* repo, int with_materials);


/*
find the position of the Material with a given id inside a given MaterialRepo
input: id: Material id after which to look
       repo: pointer to the MaterialRepo in which to look
output: the position at which the Material is found, or -1 if it is not found
*/
int repo_find(char id[], MaterialRepo* repo);
/*
adds a given Material to a given MaterialRepo
if a Material with the same id is found, it's quantity is increased instead
input: material: pointer to the Material which must be added
       repo: pointer to MaterialRepo to which to add
output: -
*/
void repo_add(Material* material, MaterialRepo* repo);
/*
updates a Material with a given id by replacing it with a new Material inside a given MaterialRepo
input: id: the id of the Material which must be updated
       new_material: the Material with which to replace the old one
       repo: the MaterialRepo in which to update
output: 1 if the Material could be updated, 0 if not
*/
int repo_update(char id[], Material* new_material, MaterialRepo* repo);
/*
removes a Material with a given id from a given MaterialRepo
input: id: the id of the Material which must be removed
       repo: the MaterialRepo from which to remove
output: 1 if the Material could be removed, 0 if not
*/
int repo_remove(char id[], MaterialRepo* repo);

/*
creates a new MaterialRepo from the Materials from a given Controller's repo that satisfy a given function
input: filter: pointer to a bool function which takes a pointer to a Material and a string
       args: arguments to be passed to filter
       ctrl: pointer to Controller from which to take Materials
output: the newly created MaterialRepo with the Materials which satisfied the filter
*/
MaterialRepo* repo_filter_materials(int (*filter)(Material*, char[]), char args[], MaterialRepo* repo);

/*
sorts the Materials from a given repository based on comparison function
input: repo: pointer to the MaterialRepo which must be sorted
       compare: pointer to function that takes two Material pointers and compares them
       reverse: whether the comparison should be done in reverse (1) or not (0)
output: -
*/
void repo_sort_by(int (*compare)(Material*, Material*), int reverse, MaterialRepo* repo);

/*
prints a string representation of a given MaterialRepo
input: repo: the MaterialRepo for which to print the string
output: -
*/
void repo_to_string(MaterialRepo* repo);
/*
populates a given MaterialRepo with some pre-made Materials
input: repo: the MaterialRepo which must be populated
output: -
*/
void populate_repo(MaterialRepo* repo);
/*
executes a series of tests for the methods of the MaterialRepo
input: -
output: -
*/
void test_repo(void);
