# include "functions.h"

// Global variable declarations
Customer customers[20];     // arrays containing all the customer and admin information extracted from the external files
Admin admins[20];
int userRecords, adminRecords;

int readUserData(char *filename) {
    FILE *file;

    file = fopen(filename, "r");

    int read = 0;
    int records = 0;

    do {
        read = fscanf(file, "%19[^,], %29[^,], %lf, %d, %lf, %lf, %lf",
                        customers[records].username,
                        customers[records].password,
                        &customers[records].balance,
                        &customers[records].approval,
                        &customers[records].transactions[0],
                        &customers[records].transactions[1],
                        &customers[records].transactions[2]);

        trim(customers[records].username);
        trim(customers[records].password);

        if(read == 7) records++;
        if(read != 7 && !feof(file)) {
            // printf("Error in file formatting.\n");
            return -1;
            }
    } while(!feof(file));

    fclose(file);

    return records;
}

int readAdminData(char *filename) {
    FILE *file;
    int dummyVar;

    file = fopen(filename, "r");

    int read = 0;
    int records = 0;

    do {
        read = fscanf(file, "%19[^,], %29[^,], %d",
                        admins[records].username,
                        admins[records].password,
                        &dummyVar);

        trim(admins[records].username);
        trim(admins[records].password);

        if(read == 3) records++;
        if(read != 3 && !feof(file)) {
            // printf("Error in file formatting.\n");
            return -1;
            }
    } while(!feof(file));

    fclose(file);

    return records;
}

void trim(char *string) {
    int length = strlen(string);
    int j = 0;

    for(int i = 0; i < length; i++) {
        if(string[i] >= '!' && string[i] <= '~') {
            string[j] = string[i];
            j++;
        }
    }
    string[j] = '\0';
}

int usernameExists(char username[20], int records) {
    for(int i; i < records; i++) {
        if(strcmp(customers[i].username, username) == 0) {
            return i+1;
        }
    }
    return 0;
}

int adminExists(char username[20], int records) {
    for(int i; i < records; i++) {
        if(strcmp(admins[i].username, username) == 0) {
            return i+1;
        }
    }
    return 0;
}

int checkPassword(int recordIndex, char password[]) {
    if(strcmp(customers[recordIndex].password, password) == 0) return 1;
    else return 0;
}

int checkAdminPassword(int recordIndex, char password[]) {
    if(strcmp(admins[recordIndex].password, password) == 0) return 1;
    else return 0;
}

void createAccount(char *username, char *password) {
    
    strcpy(customers[userRecords].username, username);
    strcpy(customers[userRecords].password, password);
    customers[userRecords].balance = 0;
    customers[userRecords].approval = 0;
    customers[userRecords].transactions[0] = 0;
    customers[userRecords].transactions[1] = 0;
    customers[userRecords].transactions[2] = 0;
    userRecords++;

    updateUserFile(userFile);
    printf("Please wait for your account to be approved by an admin.");
}

void userMenu(int recordIndex) {
    if(customers[recordIndex].approval) {
        int choice = 0;
        printf("\n\nWelcome! How would you like to use our services today?");
        while(choice != 6) {
            printf("\n1: Balance Check\n2: Cash Deposit\n3: Cash Withdrawal\n4: Transfer\n");
            printf("5: View recent transactions\n6: Logout\nPlease enter your choice: ");
            
            if(scanf("%d", &choice) != 1) {  // Check if input is an integer
                while (getchar() != '\n');  // Clear the input buffer
                printf("Invalid choice.\n");
                continue;
            }

            switch (choice) {
            case 1:
                printf("Your current balance is: %.2f\n", customers[recordIndex].balance);
                break;
            
            case 2:
                depositCash(recordIndex);
                break;

            case 3:
                float withdrawalAmount = 0;
                printf("Input amount to withdraw: ");
                if((scanf("%f", &withdrawalAmount) == 1) && (withdrawalAmount > 0) && (withdrawalAmount <= customers[recordIndex].balance)) {
                    customers[recordIndex].balance -= withdrawalAmount;
                    shiftAdd(customers[recordIndex].transactions, -withdrawalAmount, 3);
                    printf("New balance: %.2f\n", customers[recordIndex].balance);
                    updateUserFile(userFile);
                } else if(withdrawalAmount > customers[recordIndex].balance) {
                    printf("Insufficient balance.\n");
                } else {
                    printf("Invalid amount.\n");
                }
                break;

            case 4:
                float transferAmount;
                char transferUsername[20];
                int transferUserIndex;

                printf("\nPlease input recipient account username: ");
                scanf("%s", transferUsername);
                if(transferUserIndex = usernameExists(transferUsername, userRecords)) {
                    transferUserIndex--;
                    printf("Input transfer amount: ");
                    if(scanf("%f", &transferAmount) != 1) {
                        while (getchar() != '\n');
                        printf("Invalid transfer amount.\n");
                        break;
                    }

                    if(transferAmount > 0 && transferAmount <= customers[recordIndex].balance) {
                        customers[transferUserIndex].balance += transferAmount;
                        shiftAdd(customers[transferUserIndex].transactions, transferAmount, 3);

                        customers[recordIndex].balance -= transferAmount;
                        shiftAdd(customers[recordIndex].transactions, -transferAmount, 3);
                        printf("Transfer success.\n");
                        updateUserFile(userFile);
                    } else if(transferAmount > customers[recordIndex].balance) {
                        printf("Insufficient balance.\n");
                    } else printf("Invalid transfer amount.\n");
                } else {
                    printf("User does not exist.\n");
                }
                break;
            
            case 5:
                viewRecentTransactions(recordIndex);
                break;
            
            case 6:
                break;

            default:
                printf("Invalid choice.\n");
                break;
            }
        }

    } else {
        printf("Your account has not yet been approved. Please wait while an admin approves your account.\n");
    }
}

void adminMenu(void) {
    char userAccountName[20];
    int userAccountIndex;
    printf("\nWelcome!\nInput the username of the account you wish to edit: ");
    scanf("%s", userAccountName);
    if(userAccountIndex = usernameExists(userAccountName, userRecords)) {
        userAccountIndex--;
        int choice = 0;
        while(choice != 6) {
            printf("\n1: Approve\n2: Balance Check\n3: Cash Deposit\n4: View most recent transactions\n5: Delete Account\n6: Logout");
            printf("\nPlease enter your choice: ");
            if(scanf("%d", &choice) != 1) {
                while (getchar() != '\n');
                printf("Invalid choice.\n");
                continue;
            }
            switch (choice) {
            case 1:
                if(customers[userAccountIndex].approval) {
                    printf("Account already approved.\n");
                } else {
                    customers[userAccountIndex].approval = 1;
                    updateUserFile(userFile);
                    printf("Account successfully approved.\n");
                }
                break;

            case 2:
                printf("The current balance of %s is: %.2f\n", customers[userAccountIndex].username, customers[userAccountIndex].balance);
                break;
            
            case 3:
                depositCash(userAccountIndex);
                break;

            case 4:
                viewRecentTransactions(userAccountIndex);
                break;

            case 5:
                deleteRecord(customers, &userRecords, userAccountIndex);
                printf("Account deleted.\n");
                updateUserFile(userFile);
                choice = 6;
                break;

                // char decision;
                // printf("Are you sure you want to delete this account? (y/n)\n");
                // scanf("%c", decision);
                // if((decision != 'y') && (decision != 'Y') && (decision != 'n') && (decision != 'N')) {
                //     while((decision != 'y') && (decision != 'Y') && (decision != 'n') && (decision != 'N')) {
                //         printf("Are you sure you want to delete this account? (y/n)\n");
                //         scanf("%c", decision);
                //     }
                // }
                // if(decision == 'y' || decision == 'Y') {
                    // deleteRecord(customers, &userRecords, userAccountIndex);
                    // printf("Account deleted.\n");
                    // updateUserFile(userFile);
                    // choice = 6;
                // } else {
                //     break;
                // }
                
            
            case 6:
                break;

            default:
                printf("Invalid choice.\n");
                break;
            }
        }
    } else {
        printf("Username %s does not exist.\n", userAccountName);
    }
}

void updateUserFile(char *filename) {
    FILE *file;
    file = fopen(filename, "w");
    for(int i = 0; i < userRecords; i++) {
        char line[80];
        snprintf(line, sizeof(line), "%s,%s,%.2f,%d,%.2f,%.2f,%.2f\n",
                                        customers[i].username,
                                        customers[i].password,
                                        customers[i].balance,
                                        customers[i].approval,
                                        customers[i].transactions[0],
                                        customers[i].transactions[1],
                                        customers[i].transactions[2]);
        fputs(line, file);
    }
    fclose(file);
}

void shiftAdd(double *list, double newNum, int size) {
    for (int i = size - 1; i > 0; i--) {
        list[i] = list[i - 1];
    }
    list[0] = newNum;
}

void depositCash(int userAccountIndex) {
    float depositAmount = 0;
                printf("Input amount to deposit: ");
                if((scanf("%f", &depositAmount) == 1) && (depositAmount > 0)) {
                    customers[userAccountIndex].balance += depositAmount;
                    shiftAdd(customers[userAccountIndex].transactions, depositAmount, 3);
                    printf("New balance: %.2f\n", customers[userAccountIndex].balance);
                    updateUserFile(userFile);
                } else {
                    printf("Invalid amount.\n");
                }
}

void viewRecentTransactions(int userAccountIndex) {
    printf("Recent transactions: \n");
    for(int i = 0; i < 3; i++) {
        double transaction = customers[userAccountIndex].transactions[i];
        if(transaction > 0) {
            printf("In: %.2f\n", transaction);
        } else if(transaction < 0) {
            printf("Out: %.2f\n", -transaction);
        } else if(transaction == 0 && i == 0) {
            printf("No transactions have occured.\n");
            break;
        }
    }
}

void deleteRecord(Customer *list, int *size, int recordIndex) {

    // Shift items to the left to fill the gap
    for (int i = recordIndex; i < *size - 1; i++) {
        list[i] = list[i + 1];
    }

    // Decrease the size of the array
    (*size)--;

    // Optional: Clear the last item
    strcpy(customers[*size].username, "");
    strcpy(customers[*size].password, "");
    customers[*size].balance = 0;
    customers[*size].approval = 0;
    customers[*size].transactions[0] = 0;
    customers[*size].transactions[1] = 0;
    customers[*size].transactions[2] = 0;
}