#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct normalUser
{
    int userID;
    char name[30];
    char password[10];
    int account_no;
    float balance;
    char status[20];
} normalUser;

typedef struct jointUser
{
    int userID;
    char name1[30];
    char name2[30];
    char password[10];
    int account_no;
    float balance;
    char status[20];
} jointUser;

typedef struct admin
{
    int userID;
    char username[30];
    char password[10];
} admin;

void attemptNormalUserLogin(int sd);
void attemptJointUserLogin(int sd);
void attemptAdminLogin(int sd);
void showMenu(int sd);
void deposit(int sd);
void withdraw(int sd);
void balanceEnquiry(int sd);
void changePassword(int sd);
void viewDetails(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void modifyAccount(int sd);
void searchAccount(int sd);
int option, currUserID;

void chooseOption(int sd)
{
    write(1, "1 [for Normal User Login]\n", sizeof("1 [for Normal User Login]\n"));
    write(1, "2 [for Joint Account-User Login]\n", sizeof("2 [for Joint Account-User Login]\n"));
    write(1, "3 [for Admin Login]\n", sizeof("3 [for Admin Login]\n"));

    write(1, "Choose an option : ", sizeof("Choose an option : "));
    scanf("%d", &option);
    // printf("Option : %d\n",option);

    switch (option)
    {
    case 1:
        attemptNormalUserLogin(sd); //socket descriptor of server is pass as is
        break;
    case 2:
        attemptJointUserLogin(sd);
        break;
    case 3:
        attemptAdminLogin(sd);
        break;
    default:
        write(1, "Invalid option!\n", sizeof("Invalid option!\n"));
        write(1, "Please Retry!\n", sizeof("Please Retry!\n"));
        // chooseOption(sd);
        break;
    }
    return;
}

void attemptNormalUserLogin(int sd)
{
    //Writing to the server
    write(sd, &option, sizeof(int)); //1 for NU | 2 for JU | 3 for Admin

    bool result;
    normalUser currUser;
    write(1, "\nUser ID : ", sizeof("User ID : "));
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;
    write(1, "Password : ", sizeof("Password : "));
    scanf("%s", currUser.password);

    //Option value is the same as inherited from chooseOption function
    write(sd, &currUser, sizeof(normalUser));
    //sd is used here to pass information[Login ID & Password] to the server

    //Reading result from the server - Whether valid/invalid login based on entered credentials
    read(sd, &result, sizeof(result));

    if (!result)
    {
        write(1, "Invalid login!!\n\n	", sizeof("Invalid login!!\n\n"));
        attemptNormalUserLogin(sd);
    }
    else
    {
        write(1, "Succesfully logged in!\n\n", sizeof("Succesfully logged in!\n\n"));
    }
    return;
}

void attemptJointUserLogin(int sd)
{
    write(sd, &option, sizeof(int));

    bool result;
    jointUser currUser;
    write(1, "User ID : ", sizeof("User ID : "));
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;
    write(1, "Password : ", sizeof("Password : "));
    scanf("%s", currUser.password);

    write(sd, &currUser, sizeof(jointUser));

    read(sd, &result, sizeof(result));

    if (!result)
    {
        write(1, "Invalid login!!\n\n", sizeof("Invalid login!!\n\n"));
        attemptJointUserLogin(sd);
    }
    else
    {
        write(1, "Succesfully logged in!\n\n", sizeof("Succesfully logged in!\n\n"));
    }
    return;
}

void attemptAdminLogin(int sd)
{
    write(sd, &option, sizeof(int));

    bool result;
    admin currUser;
    write(1, "User ID : ", sizeof("User ID : "));
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;
    write(1, "Password : ", sizeof("Password : "));
    scanf("%s", currUser.password);

    write(sd, &currUser, sizeof(admin));

    read(sd, &result, sizeof(result));

    if (!result)
    {
        write(1, "Invalid login!!\n\n", sizeof("Invalid login!!\n\n"));
        attemptAdminLogin(sd);
    }
    else
    {
        write(1, "Succesfully logged in!\n\n", sizeof("Succesfully logged in!\n\n"));
    }
    return;
}

void showMenu(int sd)
{
    int select;
    //Option is the same as what user entered earlier(to identify between different users)
    if (option == 1 || option == 2) //Normal Users and Joint Users
    {
        write(1, "1 : Deposit\n", sizeof("1 : Deposit\n"));
        write(1, "2 : Withdraw\n", sizeof("2 : Withdraw\n"));
        write(1, "3 : Balance Enquiry\n", sizeof("3 : Balance Enquiry\n"));
        write(1, "4 : Password Change\n", sizeof("4 : Password Change\n"));
        write(1, "5 : View Account Details\n", sizeof("5 : View Account Details\n"));
        write(1, "6 : Exit\n", sizeof("6 : Exit\n"));

        write(1, "Choose an option : ", sizeof("Choose an option : "));
        scanf("%d", &select);
        // printf("Option : %d\n",select);

        switch (select)
        {
        case 1:
            deposit(sd);
            break;
        case 2:
            withdraw(sd);
            break;
        case 3:
            balanceEnquiry(sd);
            break;
        case 4:
            changePassword(sd);
            break;
        case 5:
            viewDetails(sd);
            break;
        case 6:
            write(sd, &select, sizeof(int));
            write(1, "Thank you!\n", sizeof("Thank you!\n"));
            exit(0);
        default:
            write(1, "Invalid option!!\n\n", sizeof("Invalid option!!\n\n"));
            showMenu(sd);
            break;
        }
    }
    //Option is the same as what user entered earlier(to identify between different users)
    else if (option == 3) //Admin
    {
        write(1, "1 : Add Account\n", sizeof("1 : Add Account\n"));
        write(1, "2 : Delete Account\n", sizeof("2 : Delete Account\n"));
        write(1, "3 : Modify Account\n", sizeof("3 : Modify Account\n"));
        write(1, "4 : Search Account\n", sizeof("4 : Search Account\n"));
        write(1, "5 : Exit\n", sizeof("6 : Exit\n"));

        write(1, "Choose an option : ", sizeof("Choose an option : "));
        scanf("%d", &select);
        // printf("Option : %d\n",select);

        switch (select)
        {
        case 1:
            addAccount(sd);
            break;
        case 2:
            deleteAccount(sd);
            break;
        case 3:
            modifyAccount(sd);
            break;
        case 4:
            searchAccount(sd);
            break;
        case 5:
            write(sd, &select, sizeof(int));
            write(1, "Thank you\n", sizeof("Thank you\n"));
            exit(0);
        default:
            write(1, "Invalid option!!\n\n", sizeof("Invalid option!!\n\n"));
            showMenu(sd);
            break;
        }
    }
}

void deposit(int sd)
{
    float amt;
    int select = 1;
    bool result;

    write(1, "Enter the Amount to Deposit : Rs.", sizeof("Enter the Amount to Deposit : Rs."));
    scanf("%f", &amt);

    while (amt <= 0)
    {
        printf("Please enter a valid amount!!\n");
        write(1, "Enter the Amount to Deposit : Rs.", sizeof("Enter the Amount to Deposit : Rs."));
        scanf("%f", &amt);
    }

    //Sending to the server(identified by sd)
    write(sd, &select, sizeof(int)); //select=1
    write(sd, &amt, sizeof(float));  //Sending the amount to be deposited

    read(sd, &result, sizeof(result)); //Reads from the server

    if (!result)
        write(1, "Error depositing your money!\n\n", sizeof("Error depositing your money!\n\n"));
    else
        write(1, "Succesfully deposited!\n\n", sizeof("Succesfully deposited!\n\n"));
    //Remeber that initial amount deposited in every account was 1000

    showMenu(sd);
    return;
}

void withdraw(int sd)
{
    float amt;
    int select = 2;
    bool result;

    write(1, "Enter the Amount to Withdraw : Rs.", sizeof("Enter the Amount to Withdraw : Rs."));
    scanf("%f", &amt);

    while (amt <= 0)
    {
        printf("Enter a valid amount!!\n");
        write(1, "Enter the Amount to Withdraw : Rs.", sizeof("Enter the Amount to Withdraw : Rs."));
        scanf("%f", &amt);
    }

    //Writing to the server
    write(sd, &select, sizeof(int)); //select=2
    write(sd, &amt, sizeof(float));  //Amount to be withdrawn

    read(sd, &result, sizeof(result)); //from the server

    if (!result)
        write(1, "Error withdrawing your money, please check your balance!\n\n", sizeof("Error depositing your money, please check your balance!\n\n"));
    else
        write(1, "Succesfully withdrew!!\n\n", sizeof("Succesfully withdrew!!\n\n"));
    showMenu(sd);
    return;
}

void balanceEnquiry(int sd)
{
    float amt;
    int select = 3;
    int len;

    write(sd, &select, sizeof(int));
    len = read(sd, &amt, sizeof(float)); //Reading amount from server

    write(1, "Available Balance :: Rs.", sizeof("Available Balance :: Rs."));
    printf("%0.2f\n\n", amt);

    showMenu(sd);
    return;
}

void changePassword(int sd)
{
    int select = 4;
    char nPassword[10];
    bool result;

    write(1, "Enter the new password : ", sizeof("Enter the new password : "));
    scanf("%s", nPassword);

    write(sd, &select, sizeof(int));
    write(sd, nPassword, sizeof(nPassword));

    read(sd, &result, sizeof(result));

    if (!result)
        write(1, "Password change unsuccesful!\n\n", sizeof("Password change unsuccesful!\n\n"));
    else
        write(1, "Password change succesful!\n\n", sizeof("Password change succesful!\n\n"));

    showMenu(sd);
    return;
}

void viewDetails(int sd)
{
    int select = 5;
    write(sd, &select, sizeof(int));

    if (option == 1)
    {
        normalUser currUser1;
        read(sd, &currUser1, sizeof(normalUser));

        printf("User ID : %d\n", currUser1.userID);
        printf("Name : %s\n", currUser1.name);
        printf("Account Number : %d\n", currUser1.account_no);
        printf("Available Balance : Rs.%0.2f\n", currUser1.balance);
        printf("Status : %s\n\n", currUser1.status);
    }
    else if (option == 2)
    {
        jointUser currUser2;
        read(sd, &currUser2, sizeof(jointUser));

        printf("User ID : %d\n", currUser2.userID);
        printf("Main Account Holder's Name : %s\n", currUser2.name1);
        printf("Other Account Holder's Name : %s\n", currUser2.name2);
        printf("Account Number : %d\n", currUser2.account_no);
        printf("Available Balance : Rs.%0.2f\n", currUser2.balance);
        printf("Status : %s\n\n", currUser2.status);
    }
    showMenu(sd);
    return;
}

void addAccount(int sd)
{
    int select = 1;
    int type;
    bool result;

    write(sd, &select, sizeof(int));

    write(1, "Enter the type account(1: Normal Account 2: Joint Account) : ", sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
    scanf("%d", &type);

    write(sd, &type, sizeof(int));

    if (type == 1)
    {
        normalUser addUser1;
        write(1, "Name of the account holder : ", sizeof("Name of the account holder : "));
        scanf(" %[^\n]", addUser1.name);
        write(1, "Password(max 10 characters) : ", sizeof("Password(max 10 characters) : "));
        scanf("%s", addUser1.password);
        write(1, "Initial Deposit : Rs.", sizeof("Initial Deposit : Rs."));
        scanf("%f", &addUser1.balance);
        write(sd, &addUser1, sizeof(normalUser));
    }

    if (type == 2)
    {
        jointUser addUser2;
        write(1, "Name of the primary account holder : ", sizeof("Name of the primary account holder : "));
        scanf(" %[^\n]", addUser2.name1);
        write(1, "Name of the other account holder : ", sizeof("Name of the other account holder : "));
        scanf(" %[^\n]", addUser2.name2);
        write(1, "Password(max 10 characters) : ", sizeof("Password(max 10 characters) : "));
        scanf("%s", addUser2.password);
        write(1, "Initial Deposit : Rs.", sizeof("Initial Deposit : Rs."));
        scanf("%f", &addUser2.balance);
        write(sd, &addUser2, sizeof(jointUser));
    }

    read(sd, &result, sizeof(result)); //Read result from the server

    if (!result)
        write(1, "Error adding the account!!\n\n", sizeof("Error adding the account!!\n\n"));
    else
        write(1, "Succesfully added the account!\n\n", sizeof("Succesfully added the account!\n\n"));

    showMenu(sd);
    return;
}

void deleteAccount(int sd)
{
    int select = 2;
    int type, userID;
    bool result;

    write(sd, &select, sizeof(int));

    write(1, "Enter the type account(1: Normal Account 2: Joint Account) : ", sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
    scanf("%d", &type);

    write(sd, &type, sizeof(int));

    write(1, "User ID : ", sizeof("User ID : "));
    scanf("%d", &userID); //Deleting based on ID
    write(sd, &userID, sizeof(int));

    read(sd, &result, sizeof(result)); //from the server

    if (!result)
        write(1, "Error deleting the account ,please re-check the User ID!!\n\n", sizeof("Error deleting the account ,please re-check the User ID!!\n\n"));
    else
        write(1, "Succesfully deleted the account!\n\n", sizeof("Succesfully deleted the account!\n\n"));

    showMenu(sd);
    return;
}

void modifyAccount(int sd)
{
    int select = 3;
    int type;
    bool result;

    write(sd, &select, sizeof(int));

    write(1, "Enter the type account(1: Normal Account 2: Joint Account) : ", sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
    scanf("%d", &type);

    write(sd, &type, sizeof(int));

    if (type == 1)
    {
        normalUser modUser1;
        write(1, "User ID : ", sizeof("User ID : "));
        scanf("%d", &modUser1.userID);
        write(1, "Account Number : ", sizeof("Account Number : "));
        scanf("%d", &modUser1.account_no);
        write(1, "New Name of the account holder : ", sizeof("New Name of the account holder : "));
        scanf(" %[^\n]", modUser1.name);
        write(sd, &modUser1, sizeof(normalUser));
    }

    if (type == 2)
    {
        jointUser modUser2;
        write(1, "User ID : ", sizeof("User ID : "));
        scanf("%d", &modUser2.userID);
        write(1, "Account Number : ", sizeof("Account Number : "));
        scanf("%d", &modUser2.account_no);
        write(1, "New Name of the primary account holder : ", sizeof("New Name of the primary account holder : "));
        scanf(" %[^\n]", modUser2.name1);
        write(1, "New Name of the secondary account holder : ", sizeof("New Name of the secondary account holder : "));
        scanf(" %[^\n]", modUser2.name2);
        write(sd, &modUser2, sizeof(jointUser));
    }

    read(sd, &result, sizeof(result)); //from the server

    if (!result)
        write(1, "Error modifying the account ,please re-check the User ID and Account No!!\n\n", sizeof("Error modifying the account ,please re-check the User ID and Account No!!\n\n"));
    else
        write(1, "Succesfully modified the account!!\n\n", sizeof("Succesfully modified the account!!\n\n"));

    showMenu(sd);
    return;
}

void searchAccount(int sd)
{
    int select = 4;
    int type, len;
    bool result;

    write(sd, &select, sizeof(int));

    write(1, "Enter the type account(1: Normal Account 2: Joint Account) : ", sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
    scanf("%d", &type);

    write(sd, &type, sizeof(int));

    if (type == 1)
    {
        normalUser searchUser1;
        int userID;
        write(1, "User ID : ", sizeof("User ID : "));
        scanf("%d", &userID);
        write(sd, &userID, sizeof(int)); //Gives the user ID to the server[who has data access]

        len = read(sd, &searchUser1, sizeof(normalUser));
        //Recieving the user struc and reading in temp struc searchUser1
        if (!len)
        {
            printf("User ID : %d\n", searchUser1.userID);
            printf("Name : %s\n", searchUser1.name);
            printf("Account Number : %d\n", searchUser1.account_no);
            printf("Available Balance : Rs.%0.2f\n", searchUser1.balance);
            printf("Status : %s\n\n", searchUser1.status);
        }
        else
        {
            write(1, "Please re-check the User ID!!\n\n", sizeof("Please re-check the User ID!!\n\n"));
        }
    }

    if (type == 2)
    {
        jointUser searchUser2;
        int userID1;
        write(1, "User ID : ", sizeof("User ID : "));
        scanf("%d", &userID1);
        write(sd, &userID1, sizeof(int));

        len = read(sd, &searchUser2, sizeof(jointUser));
        if (!len)
        {
            printf("User ID : %d\n", searchUser2.userID);
            printf("Main Account Holder's Name : %s\n", searchUser2.name1);
            printf("Other Account Holder's Name : %s\n", searchUser2.name2);
            printf("Account Number : %d\n", searchUser2.account_no);
            printf("Available Balance : Rs.%0.2f\n", searchUser2.balance);
            printf("Status : %s\n\n", searchUser2.status);
        }
        else
        {
            write(1, "Please re-check the User ID!!\n\n", sizeof("Please re-check the User ID!!\n\n"));
        }
    }
    showMenu(sd);
    return;
}

int main()
{
    struct sockaddr_in server;
    int sd, msgLength;
    char buff[50];
    char result;

    //connection establishment
    sd = socket(AF_INET, SOCK_STREAM, 0); //Socket descriptor of the server
    //sd is same as fd to a file i.e. unique identifier of a socket(server)
    server.sin_family = AF_INET; //IPV4
    //server.sin_addr.s_addr=inet_addr("172.16.81.54"); //other machine
    server.sin_addr.s_addr = INADDR_ANY; //same machine
    server.sin_port = htons(5557);       //Port Number

    connect(sd, (struct sockaddr *)&server, sizeof(server));

    chooseOption(sd);
    showMenu(sd);
    close(sd);
    return 0;
}