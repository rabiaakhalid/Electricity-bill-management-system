#ifndef STRUCTS_H
#define STRUCTS_H

/* ---------- Customer constants ---------- */
#define MAX_NAME      50
#define MAX_ADDRESS   100
#define MAX_PHONE     20
#define MAX_METER     20
#define CUSTOMER_FILE "customers.dat"

/* ---------- Billing constants (change these to your project's rates) ---------- */
#define BILL_FILE     "bills.dat"
#define SLAB1_LIMIT   100      /* first 100 units  */
#define SLAB2_LIMIT   200      /* next 100 units   */
#define RATE1         10.0     /* Rs per unit for slab 1 */
#define RATE2         15.0     /* Rs per unit for slab 2 */
#define RATE3         20.0     /* Rs per unit above slab 2 */
#define TAX_RATE      0.10     /* 10% tax */
#define LATE_FEE_RATE 0.05    /* 5% late fee on unpaid bills */
#define EMPLOYEE_PASSWORD "admin123"

/* ---------- Structures ---------- */
typedef struct {
    int  id;
    char name[MAX_NAME];
    char address[MAX_ADDRESS];
    char phone[MAX_PHONE];
    char meterNumber[MAX_METER];
} Customer;

typedef struct {
    int   customerId;
    int   month;
    int   year;
    float units;
    float energyCharge;
    float tax;
    float lateFee;
    float total;
    int   paid;          /* 1 = paid, 0 = unpaid */
} Bill;

/* ---------- Safe input functions (input.c) ---------- */
void  clearBuffer(void);
int   readInt(void);
float readFloat(void);
void  readString(char s[], int size);

/* ---------- Customer management (customer.c) ---------- */
void addCustomer(void);
void viewAllCustomers(void);
void searchCustomer(void);
void updateCustomer(void);
void deleteCustomer(void);
int  findCustomer(int id, Customer *result);   /* returns 1 if found, 0 if not */
void viewMyDetails(int id);

/* ---------- Billing (billing.c) ---------- */
void generateBill(void);
void viewBills(void);
void payBill(void);
void viewBillsById(int id);
void payBillById(int id);
void applyLateFees(void);

#endif
