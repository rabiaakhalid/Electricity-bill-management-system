#include <stdio.h>
#include <string.h>
#include "structs.h"

void customerMenu(void) {
    int choice;

    do {
        printf("\n----- Customer Management -----\n");
        printf("1. Add customer\n");
        printf("2. View all customers\n");
        printf("3. Search customer by ID\n");
        printf("4. Update customer\n");
        printf("5. Delete customer\n");
        printf("0. Back to main menu\n");
        printf("Enter your choice: ");
        choice = readInt();

        switch (choice) {
            case 1: addCustomer();      break;
            case 2: viewAllCustomers(); break;
            case 3: searchCustomer();   break;
            case 4: updateCustomer();   break;
            case 5: deleteCustomer();   break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);
}

void billingMenu(void) {
    int choice;

    do {
        printf("\n----- Billing -----\n");
        printf("1. Generate bill\n");
        printf("2. View bills of a customer\n");
        printf("3. Pay bill\n");
        printf("4. Apply late fees\n");
        printf("0. Back to main menu\n");
        printf("Enter your choice: ");
        choice = readInt();

        switch (choice) {
            case 1: generateBill(); break;
            case 2: viewBills();    break;
            case 3: payBill();      break;
            case 4: applyLateFees(); break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);
}

void employeeMenu(void) {
    int choice;

    do {
        printf("\n===== EMPLOYEE MENU =====\n");
        printf("1. Customer management\n");
        printf("2. Billing\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        choice = readInt();

        switch (choice) {
            case 1: customerMenu(); break;
            case 2: billingMenu();  break;
            case 0: printf("Logged out.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);
}

void customerPortal(int id) {
    int choice;

    do {
        printf("\n===== CUSTOMER MENU =====\n");
        printf("1. View my details\n");
        printf("2. View my bills\n");
        printf("3. Pay a bill\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        choice = readInt();

        switch (choice) {
            case 1: viewMyDetails(id); break;
            case 2: viewBillsById(id); break;
            case 3: payBillById(id);   break;
            case 0: printf("Logged out.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);
}

int main(void) {
    int choice, id;
    char password[50];
    Customer c;

    do {
        printf("\n===== ELECTRICITY BILL SYSTEM =====\n");
        printf("1. Employee login\n");
        printf("2. Customer login\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        choice = readInt();

        switch (choice) {
            case 1:
                printf("Enter employee password: ");
                readString(password, 50);
                if (strcmp(password, EMPLOYEE_PASSWORD) == 0) {
                    employeeMenu();
                } else {
                    printf("Wrong password.\n");
                }
                break;
            case 2:
                printf("Enter your customer ID: ");
                id = readInt();
                if (findCustomer(id, &c) == 1) {
                    printf("Welcome, %s!\n", c.name);
                    customerPortal(id);
                } else {
                    printf("Customer ID not found.\n");
                }
                break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
