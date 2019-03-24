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

    printf("\n1. print all the materials"
           "\n2. add a material"
           "\n3. delete a material by id"
           "\n4. update a material by id"
           "\n5. filter materials"
           "\n6. sort materials"
           "\n7. undo: %s"
           "\n8. redo: %s"
           "\n0. exit", undo, redo);
}

void destroy_ui(UI* ui) {
    destroy_controller(ui->ctrl);
    free(ui);
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
    printf("%d", (int) strlen(substr));
    substr[strlen(substr) - 1] = '\0';
    Controller* temp_ctrl1 = filter_by_name_substring(substr, ui->ctrl);
    Controller* temp_ctrl2= filter_by_expired(ui->today, temp_ctrl1);
    repo_to_string(temp_ctrl2->repo);
    destroy_repo(temp_ctrl1->repo, 0);
    destroy_repo(temp_ctrl2->repo, 0);
    destroy_controller(temp_ctrl1);
    destroy_controller(temp_ctrl2);
}

void ui_filter_by_not_expired_after_date(UI* ui) {
    char date[11];
    printf("give the date: ");
    fgets(date, 12, stdin);
    fgets(date, 12, stdin);
    date[strlen(date) - 1] = '\0';
    if (!validate_date(date)) {
        printf("\ninvalid date\n");
        return;
    }
    Controller* temp_ctrl = filter_by_not_expired(date, ui->ctrl);
    repo_to_string(temp_ctrl->repo);
    destroy_repo(temp_ctrl->repo, 0);
    destroy_controller(temp_ctrl);
}

void ui_filter_by_substring_sort_by_supplier(UI* ui, int reverse) {
    char substr[50];
    printf("give the string: ");
    fgets(substr, 50, stdin);
    fgets(substr, 50, stdin);
    substr[strlen(substr) - 1] = '\0';
    Controller* temp_ctrl = filter_by_name_substring(substr, ui->ctrl);
    sort_by_supplier(temp_ctrl, reverse);
    repo_to_string(temp_ctrl->repo);
    destroy_repo(temp_ctrl->repo, 0);
    destroy_controller(temp_ctrl);
}

void ui_filter_by_supplier_sort_by_quantity(UI* ui, int reverse) {
    char supplier[50];
    printf("give the supplier: ");
    fgets(supplier, 50, stdin);
    fgets(supplier, 50, stdin);
    supplier[strlen(supplier) - 1] = '\0';
    printf("give the desired quantity: ");
    char quantity[10];
    scanf("%s", quantity);
    Controller* temp_ctrl1 = filter_by_supplier(supplier, ui->ctrl);
    Controller* temp_ctrl2 = filter_by_quantity(quantity, temp_ctrl1);
    sort_by_quantity(temp_ctrl2, reverse);
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
                repo_to_string(ui->ctrl->repo);
                break;
            }
            case 2: {
                if (ui_add(ui)) {
                    repo_to_string(ui->ctrl->repo);
                    printf("\nmaterial added successfully\n");
                }
                else printf("\ninvalid input\n");
                break;
            }
            case 3: {
                if (ui_remove(ui)) {
                    repo_to_string(ui->ctrl->repo);
                    printf("\nmaterial deleted successfully\n");
                }
                else printf("\ninvalid input\n");
                break;
            }
            case 4: {
                if (ui_update(ui)) {
                    repo_to_string(ui->ctrl->repo);
                    printf("\nmaterial updated successfully\n");
                }
                else printf("\ninvalid input\n");
                break;
            }
            case 5: {
                printf("\n1. that have a substring in their name and are past the expiration date");
                printf("\n2. that will not be expired after a given date");
                printf("\ngive a valid command: ");
                scanf("%d", &cmd);

                if (cmd == 1) ui_filter_by_name_substring_and_expired(ui);
                else if (cmd == 2) ui_filter_by_not_expired_after_date(ui);
                else printf("\ninvalid command\n");
                break;
            }
            case 6: {
                printf("\n1. that are in short supply and same supplier, by quantity");
                printf("\n2. that contain a given string, by supplier");
                printf("\ngive a valid command: ");
                scanf("%d", &cmd);
                switch (cmd) {
                    case 1: {
                        printf("\n1. ascending");
                        printf("\n2. descending");
                        printf("\ngive a valid command: ");
                        scanf("%d", &cmd);
                        if (cmd == 1 || cmd == 2) ui_filter_by_supplier_sort_by_quantity(ui, cmd - 1);
                        else printf("\ninvalid command\n");
                        break;
                    }
                    case 2: {
                        printf("\n1. ascending");
                        printf("\n2. descending");
                        printf("\ngive a valid command: ");
                        scanf("%d", &cmd);
                        if (cmd == 1 || cmd == 2) ui_filter_by_substring_sort_by_supplier(ui, cmd - 1);
                        else printf("\ninvalid command\n");
                        break;
                    }
                    default: printf("\ninvalid command\n");
                }
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
            default: printf("\ninvalid command\n");
        }
    }
    printf("\n\nGoodbye!\n\n");
}
