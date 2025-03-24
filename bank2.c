#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

void Create_account();
void Deposite_money();
void Withdraw_money();
void check_balance();
bool account_exists(int acc_no);

const char* ACCOUNT_FILE = "account.csv";

typedef struct {
    char name[50];
    int acc_no;
    float balance;
} Account;

int main() {
    while(1) {
        int choice;
        printf("\n*** Bank Management System ***\n");
        printf("\n1. Create account");
        printf("\n2. Deposit money");
        printf("\n3. Withdraw money");
        printf("\n4. Check balance");
        printf("\n5. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                Create_account();
                break;
            case 2:
                Deposite_money();
                break;
            case 3:
                Withdraw_money();
                break;
            case 4:
                check_balance();
                break;
            case 5:
                printf("\nClosing the bank. Thank you for visiting!\n");
                return 0;
            default:
                printf("\nInvalid choice\n");
        }
    }
}

bool account_exists(int acc_no) {
    FILE *file = fopen(ACCOUNT_FILE, "r");
    if (file == NULL) {
        return false;
    }
    
    Account acc;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%49[^,],%d,%f", acc.name, &acc.acc_no, &acc.balance);
        if (acc.acc_no == acc_no) {
            fclose(file);
            return true;
        }
    }
    
    fclose(file);
    return false;
}

void Create_account() {
    Account acc;
    FILE *file = fopen(ACCOUNT_FILE, "a+");
    if (file == NULL) {
        printf("\nUnable to open file!!\n");
        return;
    }

    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\nEnter your name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

    printf("Enter your Account number: ");
    scanf("%d", &acc.acc_no);
    
    // Check for duplicate account
    if (account_exists(acc.acc_no)) {
        printf("\nAccount number %d already exists!\n", acc.acc_no);
        fclose(file);
        return;
    }

    acc.balance = 0.0f;

    if (ftell(file) == 0) {
        fprintf(file, "name,account_no,Available_balance\n"); // Add headers if file is empty
    }
    fprintf(file, "%s,%d,%.2f\n", acc.name, acc.acc_no, acc.balance);

    fclose(file);
    printf("\nAccount created successfully\n");
    printf("Data saved to %s in the program's directory\n", ACCOUNT_FILE);
}

void Deposite_money() {
    FILE *file = fopen(ACCOUNT_FILE, "r+");
    if (file == NULL) {
        printf("\nUnable to open account file!!\n");
        return;
    }

    int acc_no;
    float money;
    Account acc_r;
    
    printf("\nEnter your Account No: ");
    scanf("%d", &acc_no);
    
    printf("Enter amount to deposit: ");
    scanf("%f", &money);

    if (money <= 0) {
        printf("\nInvalid amount!\n");
        fclose(file);
        return;
    }

    char buffer[256];
    bool found = false;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%49[^,],%d,%f", acc_r.name, &acc_r.acc_no, &acc_r.balance);
        if (acc_r.acc_no == acc_no) {
            acc_r.balance += money;
            fseek(file, -strlen(buffer), SEEK_CUR);
            fprintf(file, "%s,%d,%.2f\n", acc_r.name, acc_r.acc_no, acc_r.balance);
            found = true;
            break;
        }
    }
    
    fclose(file);
    
    if (found) {
        printf("\nSuccessfully deposited Rs.%.2f, New balance is Rs.%.2f\n", money, acc_r.balance);
    } else {
        printf("\nAccount number %d not found!\n", acc_no);
    }
}

void Withdraw_money() {
    FILE *file = fopen(ACCOUNT_FILE, "r+");
    if (file == NULL) {
        printf("\nUnable to open account file!!\n");
        return;
    }
    
    int acc_no;
    float money;
    Account acc_r;
    
    printf("\nEnter your account number: ");
    scanf("%d", &acc_no);
    
    printf("Enter the amount to withdraw: ");
    scanf("%f", &money);

    if (money <= 0) {
        printf("\nInvalid amount!\n");
        fclose(file);
        return;
    }

    char buffer[256];
    bool found = false;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%49[^,],%d,%f", acc_r.name, &acc_r.acc_no, &acc_r.balance);
        if (acc_r.acc_no == acc_no) {
            found = true;
            if (acc_r.balance >= money) {
                acc_r.balance -= money;
                fseek(file, -strlen(buffer), SEEK_CUR);
                fprintf(file, "%s,%d,%.2f\n", acc_r.name, acc_r.acc_no, acc_r.balance);
                printf("\nSuccessfully withdrew Rs.%.2f, remaining balance is Rs.%.2f\n", money, acc_r.balance);
            } else {
                printf("\nInsufficient balance!\n");
            }
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\nAccount number %d not found!\n", acc_no);
    }
}

void check_balance() {
    FILE *file = fopen(ACCOUNT_FILE, "r");
    if (file == NULL) {
        printf("\nNo accounts exist yet!\n");
        return;
    }
    
    int acc_no;
    printf("\nEnter your Account number: ");
    scanf("%d", &acc_no);

    Account acc_r;
    char buffer[256];
    bool found = false;
    
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%49[^,],%d,%f", acc_r.name, &acc_r.acc_no, &acc_r.balance);
        if (acc_r.acc_no == acc_no) {
            printf("\nAccount Holder: %s", acc_r.name);
            printf("\nAccount Balance: Rs.%.2f\n", acc_r.balance);
            found = true;
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\nAccount number %d not found!\n", acc_no);
    }
}
