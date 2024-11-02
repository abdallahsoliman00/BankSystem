# include "functions.h"

int main() {
    // Thses two variables contain the total number of users and admins in the entire system
    userRecords = readUserData(userFile);
    adminRecords = readAdminData("adminInfo.csv");
    
    if(userRecords == -1 || adminRecords == -1) {
        printf("FileError: File(s) not found.");
        while(1);
        return 1;
    }

    // These variables store the properties of the current user's record
    int choice = 0;
    char username[20];
    char password[30];
    int userRecordIndex, adminRecordIndex;

    while(choice != 4) {
        printf("\n\nWelcome to the Brightskies Banking system.");
        printf("\n1: User Login\n2: Admin Login\n3: Create new account\n4: Exit\nPlease enter your choice: ");
        
        if(scanf("%d", &choice) != 1) {  // Check if input is an integer
            while (getchar() != '\n');  // Clear the input buffer
            printf("Invalid choice.\n");
            continue;
        }

        switch (choice) {
        case 1:
            printf("\nUser Login:\n");
            printf("Username: ");
            scanf("%s", username);
            if(userRecordIndex = usernameExists(username, userRecords)) {
                userRecordIndex--;
                printf("Password: ");
                scanf("%s", password);
                if(checkPassword(userRecordIndex, password)) userMenu(userRecordIndex);
                else {
                    printf("Incorrect password.\n");
                }

            } else {
                printf("Username does not exist.\n");
            }
            break;
        
        case 2:
            printf("\nAdmin Login:\n");
            printf("Username: ");
            scanf("%s", username);
            if(adminRecordIndex = adminExists(username, adminRecords)) {
                adminRecordIndex--;
                printf("Password: ");
                scanf("%s", password);
                if(checkAdminPassword(adminRecordIndex, password)) adminMenu();
                else {
                    printf("Incorrect password.\n");
                }

            } else {
                printf("Username does not exist.\n");
            }
            break;

        
        case 3:
            printf("\nPlease enter new username: ");
            scanf("%s", username);
            if(usernameExists(username, userRecords)){
                do {
                    printf("Username already exists. Please try again: ");
                    scanf("%s", username);
                } while (usernameExists(username, userRecords));
            }
            
            printf("Please enter password: ");
            scanf("%s", password);

            createAccount(username, password);
            break;

        case 4:
            updateUserFile(userFile);
            break;

        default:
            printf("Invalid choice.\n");
            break;
        }
    }
    return 0;
}
