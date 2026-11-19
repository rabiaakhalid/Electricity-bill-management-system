#include <stdio.h>
#include "structs.h"

/* ---------- Helper functions ---------- */

float calculateEnergyCharge(float units) {
    float charge;

    if (units <= SLAB1_LIMIT) {
        charge = units * RATE1;
    } else if (units <= SLAB2_LIMIT) {
        charge = SLAB1_LIMIT * RATE1 + (units - SLAB1_LIMIT) * RATE2;
    } else {
        charge = SLAB1_LIMIT * RATE1
               + (SLAB2_LIMIT - SLAB1_LIMIT) * RATE2
               + (units - SLAB2_LIMIT) * RATE3;
    }
    return charge;
}

void printBill(Bill *b) {
    printf("Customer ID   : %d\n", b->customerId);
    printf("Billing month : %d/%d\n", b->month, b->year);
    printf("Units used    : %.2f\n", b->units);
    printf("Energy charge : Rs %.2f\n", b->energyCharge);
    printf("Tax           : Rs %.2f\n", b->tax);
    printf("Late fee      : Rs %.2f\n", b->lateFee);
    printf("TOTAL         : Rs %.2f\n", b->total);
    if (b->paid == 1) {
        printf("Status        : PAID\n");
    } else {
        printf("Status        : UNPAID\n");
    }
}

/* ---------- Billing ---------- */

void generateBill(void) {
    Customer c;
    Bill b;
    FILE *fp;

    printf("Enter customer ID: ");
    b.customerId = readInt();

    if (findCustomer(b.customerId, &c) == 0) {
        printf("Customer not found. Add the customer first.\n");
        return;
    }

    printf("Customer: %s\n", c.name);

    printf("Enter billing month (1-12): ");
    b.month = readInt();
    while (b.month < 1 || b.month > 12) {
        printf("Invalid month. Enter a number from 1 to 12: ");
        b.month = readInt();
    }

    printf("Enter billing year: ");
    b.year = readInt();
    while (b.year < 2000 || b.year > 2100) {
        printf("Invalid year. Enter a year from 2000 to 2100: ");
        b.year = readInt();
    }

    printf("Enter units consumed: ");
    b.units = readFloat();
    while (b.units < 0) {
        printf("Units cannot be negative. Enter again: ");
        b.units = readFloat();
    }

    b.energyCharge = calculateEnergyCharge(b.units);
    b.tax = b.energyCharge * TAX_RATE;
    b.total = b.energyCharge + b.tax;
    b.paid = 0;      /* every new bill starts as unpaid */
    b.lateFee = 0;   /* no late fee on a new bill */

    fp = fopen(BILL_FILE, "ab");
    if (fp == NULL) {
        printf("Error: could not open the bill file.\n");
        return;
    }
    fwrite(&b, sizeof(Bill), 1, fp);
    fclose(fp);

    printf("\n===== ELECTRICITY BILL =====\n");
    printf("Name          : %s\n", c.name);
    printf("Address       : %s\n", c.address);
    printf("Meter number  : %s\n", c.meterNumber);
    printBill(&b);
    printf("============================\n");
}

void viewBillsById(int id) {
    FILE *fp = fopen(BILL_FILE, "rb");
    Bill b;
    int count = 0;

    if (fp == NULL) {
        printf("No bills found.\n");
        return;
    }

    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        if (b.customerId == id) {
            printf("\n---------------------------\n");
            printBill(&b);
            count++;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No bills found for this customer.\n");
    }
}

void viewBills(void) {
    int id;

    printf("Enter customer ID: ");
    id = readInt();
    viewBillsById(id);
}

void payBillById(int id) {
    FILE *fp = fopen(BILL_FILE, "r+b");
    Bill b;
    int month, year;

    if (fp == NULL) {
        printf("No bills found.\n");
        return;
    }

    printf("Enter billing month (1-12): ");
    month = readInt();
    printf("Enter billing year: ");
    year = readInt();

    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        if (b.customerId == id && b.month == month && b.year == year) {
            if (b.paid == 1) {
                printf("This bill is already paid.\n");
            } else {
                b.paid = 1;
                fseek(fp, -(long)sizeof(Bill), SEEK_CUR);   /* go back one record */
                fwrite(&b, sizeof(Bill), 1, fp);
                printf("Bill marked as PAID. Amount: Rs %.2f\n", b.total);
            }
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Bill not found.\n");
}

void payBill(void) {
    int id;

    printf("Enter customer ID: ");
    id = readInt();
    payBillById(id);
}
/* Adds a late fee to every unpaid bill from a month before the current month. */
void applyLateFees(void) {
    FILE *fp = fopen(BILL_FILE, "r+b");
    Bill b;
    int currentMonth, currentYear;
    int count = 0;

    if (fp == NULL) {
        printf("No bills found.\n");
        return;
    }

    printf("Enter current month (1-12): ");
    currentMonth = readInt();
    while (currentMonth < 1 || currentMonth > 12) {
        printf("Invalid month. Enter a number from 1 to 12: ");
        currentMonth = readInt();
    }

    printf("Enter current year: ");
    currentYear = readInt();
    while (currentYear < 2000 || currentYear > 2100) {
        printf("Invalid year. Enter a year from 2000 to 2100: ");
        currentYear = readInt();
    }

    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        /* overdue = unpaid, no late fee yet, and billed for an earlier month */
        if (b.paid == 0 && b.lateFee == 0 &&
            (b.year * 12 + b.month) < (currentYear * 12 + currentMonth)) {

            b.lateFee = b.total * LATE_FEE_RATE;
            b.total = b.total + b.lateFee;

            fseek(fp, -(long)sizeof(Bill), SEEK_CUR);   /* go back one record */
            fwrite(&b, sizeof(Bill), 1, fp);
            fseek(fp, 0, SEEK_CUR);                     /* needed before reading again */
            count++;
        }
    }
    fclose(fp);

    printf("Late fee added to %d bill(s).\n", count);
}