#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

UI* create_ui(Controller* controller) {
    UI* ui = (UI*) malloc(sizeof(UI));
    ui->ctrl = controller;
    time_t now = time(0);
    struct tm* local_time = localtime(&now);
    sprintf(ui->today, "%d.%d.%d", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
    return ui;
}

void print_menu(UI* ui) {
    char undo[50];
    char redo[50];

    if (!can_undo(ui->ctrl)) strcpy(undo, "no undo disponible");
    else {
        Material* undo_mat = ui->ctrl->operation_stack[ui->ctrl->oper_len - 1]->material;
        sprintf(undo, "%s %s %s %s %d",
                ui->ctrl->operation_stack[ui->ctrl->oper_len - 1]->type,
                undo_mat->name,
                undo_mat->supplier,
                undo_mat->exp_date_string,
                undo_mat->quantity);
    }
    if (!can_redo(ui->ctrl)) strcpy(redo, "no redo disponible");
    else {
        Material* redo_mat = ui->ctrl->operation_stack[ui->ctrl->oper_len]->material;
        sprintf(redo, "%s %s %s %s %d",
                ui->ctrl->operation_stack[ui->ctrl->oper_len]->type,
                redo_mat->name,
                redo_mat->supplier,
                redo_mat->exp_date_string,
                redo_mat->quantity);
    }

    printf("\n1. add a material"
           "\n2. delete a material by id"
           "\n3. update a material by id"
           "\n\n4. filter materials"
           "\n5. sort materials in short supply and same supplier"
           "\n6. print all the materials"
           "\n\n7. undo: %s"
           "\n8. redo: %s"
           "\n9. sort materials that contain a given string by supplier"
           "\n0. exit", undo, redo);
}

void destroy_ui(UI* ui) {
    destroy_controller(ui->ctrl);
    free(ui);
    ui = NULL;
}

int ui_add(UI* ui) {
    char name[50];
    char supplier[50];
    char exp_date[11];
    int quantity;
    printf("give name, supplier, expiration date and quantity, separated by space:\n");
    scanf("%s %s %s %d", name, supplier, exp_date, &quantity);
    return ctrl_add(name, supplier, exp_date, quantity, ui->ctrl);
}

int ui_remove(UI* ui) {
    char id[50];
    printf("give the id of the material you want to remove (Name_Supplier_ExpirationDate): ");
    scanf("%s", id);
    return ctrl_remove(id, ui->ctrl);
}

int ui_update(UI* ui) {
    char name[50];
    char supplier[50];
    int quantity;
    char exp_date[11];
    char id[50];
    printf("give the id of the material you want to remove (Name_Supplier_ExpirationDate): ");
    scanf("%s", id);
    printf("give name, supplier, expiration date and quantity of new material, separated by space:\n");
    scanf("%s %s %s %d", name, supplier, exp_date, &quantity);
    return ctrl_update(id, name, supplier, exp_date, quantity, ui->ctrl);
}

void ui_filter_by_name_substring_and_expired(UI* ui) {
    char substr[50];
    printf("give the substring (leave empty for all materials): ");
    fgets(substr, 50, stdin);
    fgets(substr, 50, stdin);
    Controller* temp_ctrl1 = ctrl_filter_materials(substring_in_material_name, substr, ui->ctrl);
    Controller* temp_ctrl2= ctrl_filter_materials(expired_material, ui->today, temp_ctrl1);
    repo_to_string(temp_ctrl2->repo);
    destroy_repo(temp_ctrl1->repo, 0);
    destroy_repo(temp_ctrl2->repo, 0);
    destroy_controller(temp_ctrl1);
    destroy_controller(temp_ctrl2);
}

void ui_filter_by_not_expired_after_date(UI* ui) {
    char date[11];
    printf("give the date: ");
    fgets(date, 11, stdin);
    fgets(date, 11, stdin);
    if (!validate_date(date)) {
        printf("\ninvalid date\n");
        return;
    }
    Controller* temp_ctrl = ctrl_filter_materials(not_expired_material, date, ui->ctrl);
    repo_to_string(temp_ctrl->repo);
    destroy_repo(temp_ctrl->repo, 0);
    destroy_controller(temp_ctrl);
}

void ui_sort_by_supplier(UI* ui) {
    char substr[50];
    printf("give the string: ");
    fgets(substr, 50, stdin);
    fgets(substr, 50, stdin);
    Controller* temp_ctrl = ctrl_filter_materials(substring_in_material_name, substr, ui->ctrl);
    ctrl_sort_by_supplier(temp_ctrl);
    repo_to_string(temp_ctrl->repo);
    destroy_repo(temp_ctrl->repo, 0);
    destroy_controller(temp_ctrl);
}

void ui_sort_by_quantity(UI* ui, int reverse) {
    char supplier[50];
    printf("give the supplier: ");
    fgets(supplier, 50, stdin);
    fgets(supplier, 50, stdin);
    printf("give the desired quantity: ");
    char quantity[10];
    scanf("%s", quantity);
    Controller* temp_ctrl1 = ctrl_filter_materials(from_supplier, supplier, ui->ctrl);
    Controller* temp_ctrl2 = ctrl_filter_materials(quantity_less_than, quantity, temp_ctrl1);
    ctrl_sort_by_quantity(temp_ctrl2, reverse);
    repo_to_string(temp_ctrl2->repo);
    destroy_repo(temp_ctrl1->repo, 0);
    destroy_repo(temp_ctrl2->repo, 0);
    destroy_controller(temp_ctrl1);
    destroy_controller(temp_ctrl2);
}

void start_ui(UI* ui) {
    printf("Welcome to Bread'n'Bagel!\n");
    printf("Current date: %s\n", ui->today);
    repo_to_string(ui->ctrl->repo);

    int cmd = -1;

    while (cmd != 0) {
        print_menu(ui);
        printf("\ngive a valid command: ");
        scanf("%d", &cmd);

        switch (cmd) {
            case 0: break;
            case 1: {
                if (ui_add(ui)) {
                    repo_to_string(ui->ctrl->repo);
                    printf("\nmaterial added successfully\n");
                }
                else printf("\ninvalid input\n");
                break;
            }
            case 2: {
                if (ui_remove(ui)) {
                    repo_to_string(ui->ctrl->repo);
                    printf("\nmaterial deleted successfully\n");
                }
                else printf("\ninvalid input\n");
                break;
            }
            case 3: {
                if (ui_update(ui)) {
                    repo_to_string(ui->ctrl->repo);
                    printf("\nmaterial updated successfully\n");
                }
                else printf("\ninvalid input\n");
                break;
            }
            case 4: {
                printf("\n1. that have a substring in their name and are past the expiration date");
                printf("\n2. that will not be expired after a given date");
                printf("\ngive a valid command: ");
                scanf("%d", &cmd);

                if (cmd == 1) ui_filter_by_name_substring_and_expired(ui);
                else if (cmd == 2) ui_filter_by_not_expired_after_date(ui);
                else printf("\ninvalid command\n");
                break;
            }
            case 5: {
                printf("\n1. ascending");
                printf("\n2. descending");
                printf("\ngive a valid command: ");
                scanf("%d", &cmd);
                if (cmd == 1 || cmd == 2) ui_sort_by_quantity(ui, cmd - 1);
                else printf("\ninvalid command\n");
                break;
            }
            case 6: {
                repo_to_string(ui->ctrl->repo);
                break;
            }
            case 7: {
                undo(ui->ctrl);
                repo_to_string(ui->ctrl->repo);
                break;
            }
            case 8: {
                redo(ui->ctrl);
                repo_to_string(ui->ctrl->repo);
                break;
            }
            case 9: {
                ui_sort_by_supplier(ui);
                break;
            }
            default: printf("\ninvalid command\n");
        }
    }
    printf("\n\nGoodbye!\n\n");
}
