# ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

# ifndef userFile
# define userFile "userInfo.csv"
# endif

// Creating structures for customers and admins
typedef struct {
    char username[20];
    char password[30];
    double balance;
    int approval;           // 1 if approved
    double transactions[3];
} Customer;

typedef struct {
    char username[20];
    char password[30];
} Admin;


// Global variable declarations
extern Customer customers[20];     // arrays containing all the customer and admin information extracted from the external files
extern Admin admins[20];
extern int userRecords, adminRecords;


// Function prototypes
void trim(char *string);

int readUserData(char *filename);

int readAdminData(char *filename);

int usernameExists(char username[20], int userRecords);

int adminExists(char username[20], int records);

int checkPassword(int recordIndex, char password[]);

int checkAdminPassword(int recordIndex, char password[]);

void userMenu(int recordIndex);

void adminMenu(void);

void createAccount(char *username, char *password);

void updateUserFile(char *filename);

void shiftAdd(double *list, double newNum, int size);

void depositCash(int userAccountIndex);

void viewRecentTransactions(int userAccountIndex);

void deleteRecord(Customer *list, int *size, int recordIndex);

# endif