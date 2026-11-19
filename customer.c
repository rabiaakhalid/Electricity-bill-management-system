#include <stdio.h>
#include "structs.h"

/* ---------- Helper functions ---------- */

void printCustomer(Customer *c) {
    printf("ID: %d\n", c->id);
    printf("Name: %s\n", c->name);
    printf("Address: %s\n", c->address);
    printf("Phone: %s\n", c->phone);
    printf("Meter number: %s\n", c->meterNumber);
}

/* Returns 1 if a customer with this ID is already in the file, otherwise 0. */
int idExists(int id) {
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    Customer c;

    if (fp == NULL) {
        return 0;
    }
    while (fread(&c, sizeof(Customer), 1, fp) == 1) {
        if (c.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* Asks for name, address, phone and meter number (used by add and update). */
void readCustomerDetails(Customer *c) {
    printf("Enter name: ");
    readString(c->name, MAX_NAME);
    printf("Enter address: ");
    readString(c->address, MAX_ADDRESS);
    printf("Enter phone: ");
    readString(c->phone, MAX_PHONE);
    printf("Enter meter number: ");
    readString(c->meterNumber, MAX_METER);
}

/* Looks for a customer by ID. If found, copies the details into *result and returns 1. */
int findCustomer(int id, Customer *result) {
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    Customer c;

    if (fp == NULL) {
        return 0;
    }
    while (fread(&c, sizeof(Customer), 1, fp) == 1) {
        if (c.id == id) {
            *result = c;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ---------- Customer management ---------- */

void addCustomer(void) {
    Customer c;
    FILE *fp;

    printf("Enter customer ID: ");
    c.id = readInt();
    while (c.id <= 0) {
        printf("ID must be a positive number. Enter again: ");
        c.id = readInt();
    }

    if (idExists(c.id) == 1) {
        printf("A customer with this ID already exists.\n");
        return;
    }

    readCustomerDetails(&c);

    fp = fopen(CUSTOMER_FILE, "ab");
    if (fp == NULL) {
        printf("Error: could not open the file.\n");
        return;
    }
    fwrite(&c, sizeof(Customer), 1, fp);
    fclose(fp);

    printf("Customer added successfully.\n");
}

void viewAllCustomers(void) {
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    Customer c;
    int count = 0;

    if (fp == NULL) {
        printf("No customers found.\n");
        return;
    }

    printf("\n--- All Customers ---\n");
    while (fread(&c, sizeof(Customer), 1, fp) == 1) {
        printCustomer(&c);
        printf("-------------------\n");
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No customers found.\n");
    }
}

void searchCustomer(void) {
    Customer c;
    int id;

    printf("Enter customer ID to search: ");
    id = readInt();

    if (findCustomer(id, &c) == 1) {
        printf("\n--- Customer Found ---\n");
        printCustomer(&c);
    } else {
        printf("Customer not found.\n");
    }
}

void updateCustomer(void) {
    FILE *fp = fopen(CUSTOMER_FILE, "r+b");
    Customer c;
    int id;

    if (fp == NULL) {
        printf("No customers found.\n");
        return;
    }

    printf("Enter customer ID to update: ");
    id = readInt();

    while (fread(&c, sizeof(Customer), 1, fp) == 1) {
        if (c.id == id) {
            printf("\nCurrent details:\n");
            printCustomer(&c);

            printf("\nEnter new details (ID cannot be changed):\n");
            readCustomerDetails(&c);

            fseek(fp, -(long)sizeof(Customer), SEEK_CUR);   /* go back one record */
            fwrite(&c, sizeof(Customer), 1, fp);
            fclose(fp);

            printf("Customer updated successfully.\n");
            return;
        }
    }
    fclose(fp);
    printf("Customer not found.\n");
}

void deleteCustomer(void) {
    FILE *fp;
    FILE *temp;
    Customer c;
    int id, found = 0;

    printf("Enter customer ID to delete: ");
    id = readInt();

    fp = fopen(CUSTOMER_FILE, "rb");
    if (fp == NULL) {
        printf("No customers found.\n");
        return;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Error: could not create temporary file.\n");
        fclose(fp);
        return;
    }

    /* copy every customer except the one being deleted */
    while (fread(&c, sizeof(Customer), 1, fp) == 1) {
        if (c.id == id) {
            found = 1;
        } else {
            fwrite(&c, sizeof(Customer), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);

    if (found == 1) {
        remove(CUSTOMER_FILE);
        rename("temp.dat", CUSTOMER_FILE);
        printf("Customer deleted successfully.\n");
    } else {
        remove("temp.dat");
        printf("Customer not found.\n");
    }
}

void viewMyDetails(int id) {
    Customer c;

    if (findCustomer(id, &c) == 1) {
        printf("\n--- My Details ---\n");
        printCustomer(&c);
    } else {
        printf("Customer record not found.\n");
    }
}
