#pragma once
#include "material_repository.h"
#include "operation.h"

/*
defines an object that can perform various operations on a MaterialRepo and can store CRUD operations in a stack
repo: pointer to a MaterialRepo to act on
oper_len: the current amount of undo-able operations
oper_cap: the maximum amount of undu-able operations
operation_stack: pointer to a list of pointers to Operations
*/
typedef struct {
    MaterialRepo* repo;
    int oper_len;
    int oper_cap;
    Operation** operation_stack;
} Controller;

/*
creates a Controller object with the given MaterialRepo; the operation stack is made internally
input: pointer to the MaterialRepo to act on
output: pointer to the newly created Controller
*/
Controller* create_controller(MaterialRepo* repo);
/*
destroys a given Controller by freeing the memory it occupies
input: pointer to the Controller
output: -
*/
void destroy_controller(Controller* ctrl);

/*
finds the Material with a given id from a Controller's repo
input: id: Material id after which to look
       ctrl: pointer to the Controller in to look
output: the position at which the Material is found, or -1 if it is not found
*/
int ctrl_find(char id[], Controller* ctrl);
/*
creates a new Material from the given arguments and adds it to a Controller's repo if it is valid
input: name: name of the new material
       supplier: supplier of the new Material
       exp_date_str: string of the expiration date of the new Material "dd.mm.yyyy"
       quantity: quantity of the new Material
       ctrl: pointer to Controller in which to add
output: 1 if the Material has been added, 0 if not
*/
int ctrl_add(char name[], char supplier[], char exp_date_str[], int quantity, Controller* ctrl);
/*
creates a new Material from the given arguments and updates the Material with the given id from a Controller's repo if it is valid
input: id: id of Material which must be updated
       name: name of the new material
       supplier: supplier of the new Material
       exp_date_str: string of the expiration date of the new Material "dd.mm.yyyy"
       quantity: quantity of the new Material
       ctrl: pointer to Controller in which to update
output: 1 if the Material has been found and updated, 0 if not
*/
int ctrl_update(char id[], char name[], char supplier[], char exp_date_str[], int quantity, Controller* ctrl);
/*
removes the Material with the given id from a Controller's repo
input: id: id of Material which must be removed
       ctrl: Controller from which to remove
output: 1 if the Material has been found and removed, 0 if not
*/
int ctrl_remove(char id[], Controller* ctrl);

/*
creates a new Controller from the Materials from a given Controller's repo that have a given string in their name
input: substr: substring that must be in the name
       ctrl: pointer to Controller from which to take Materials
output: the newly created Controller with the Materials which satisfied the filter
*/
Controller* filter_by_name_substring(char substr[], Controller* ctrl);
/*
creates a new Controller from the Materials from a given Controller's repo that surpassed a given date
input: date: date with which to compare
       ctrl: pointer to Controller from which to take Materials
output: the newly created Controller with the Materials which satisfied the filter
*/
Controller* filter_by_expired(char date[], Controller* ctrl);
/*
creates a new Controller from the Materials from a given Controller's repo that have not surpassed a given date
input: date: date with which to compare
       ctrl: pointer to Controller from which to take Materials
output: the newly created Controller with the Materials which satisfied the filter
*/
Controller* filter_by_not_expired(char date[], Controller* ctrl);
/*
creates a new Controller from the Materials from a given Controller's repo that have the same supplier
input: supplier: supplier to search for
       ctrl: pointer to Controller from which to take Materials
output: the newly created Controller with the Materials which satisfied the filter
*/
Controller* filter_by_supplier(char supplier[], Controller* ctrl);
/*
creates a new Controller from the Materials from a given Controller's repo that have the quantity less than a given one
input: quantity: quantity with which to compare
       ctrl: pointer to Controller from which to take Materials
output: the newly created Controller with the Materials which satisfied the filter
*/
Controller* filter_by_quantity(char quantity[], Controller* ctrl);

/*
sorts the Materials from a given repository based on their quantity
input: repo: pointer to the MaterialRepo which must be sorted
       reverse: 0 for ascending order, 1 for descending order
output: -
*/
void sort_by_quantity(Controller* ctrl, int reverse);
/*
sorts the Materials from a given repository based on their supplier
input: repo: pointer to the MaterialRepo which must be sorted
       reverse: 0 for ascending order, 1 for descending order
output: -
*/
void sort_by_supplier(Controller* ctrl, int reverse);

/*
adds a given Operation to a given Controller's operation stack
input: operation: pointer to Operation which to add
       ctrl: pointer to Controller to which to add
*/
void add_operation(Operation* operation, Controller* ctrl);
/*
checks if a Controller's undo operation stack is not empty
input: pointer to Controller
output: 0 if it is empty, 1 if not
*/
int can_undo(Controller* ctrl);
/*
checks if a Controller's redo operation stack is not empty
input: pointer to Controller
output: 0 if it is empty, 1 if not
*/
int can_redo(Controller* ctrl);
/*
applies the last operation in a Controler's stack in reverse
input: pointer to the Controller
output: -
*/
void undo(Controller* ctrl);
/*
reapplies the last operation in a Controler's stack
input: pointer to the Controller
output: -
*/
void redo(Controller* ctrl);

/*
checks if a given string date is valid "dd.mm.yyyy"
input: string to check
output: 1 if it is valid, 0 if not
*/
int validate_date(char date_string[]);
/*
executes a series of tests for the methods of the Controller
input: -
output: -
*/
void test_controller(void);
