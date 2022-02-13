#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct normalUser
{
    int userID; //unique index
    char name[30];
    char password[10];
    int account_no;
    float balance;
    char status[20]; //ACTIVE or CLOSED
} normalUser;

typedef struct jointUser
{
    int userID;
    char name1[30]; //Joint Account holder 1
    char name2[30]; //Joint Account holder 2
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

int getNewUserID1(); //Normal User
int getNewUserID2(); //Joint User
int getNewUserID3(); //Admin

int getNewUserID1()
{
    int fd = open("NormalUser", O_RDONLY, 0744);
    normalUser record;
    lseek(fd, -sizeof(normalUser), SEEK_END); //Going to the end of the record
    read(fd, &record, sizeof(normalUser));    //Reading the last record
    close(fd);
    return record.userID + 1; //Last userID + 1 for the new user
}

int getNewUserID2()
{
    int fd = open("JointUser", O_RDONLY, 0744);
    jointUser record;
    lseek(fd, -sizeof(jointUser), SEEK_END);
    read(fd, &record, sizeof(jointUser));
    close(fd);
    return record.userID + 1;
}

int getNewUserID3()
{
    int fd = open("Admin", O_RDONLY, 0744);
    admin record;
    lseek(fd, -sizeof(admin), SEEK_END);
    read(fd, &record, sizeof(admin));
    close(fd);
    return record.userID + 1;
}

int main()
{
    int fd = open("Admin", O_RDWR | O_CREAT, 0744);
    int choice = 0;
    admin newAdmin;
    printf("-----ADMIN----- \n");
    printf("Please enter the name of the admin: ");
    scanf(" %[^\n]", newAdmin.username);
    printf("Please enter the password: ");
    scanf(" %[^\n]", newAdmin.password);
    newAdmin.userID = 1000;

    printf("Your Admin ID is: %d \n", newAdmin.userID);
    write(fd, &newAdmin, sizeof(admin));
    printf("Do you want to continue(No[0]/Yes[1])? ");
    scanf("%d", &choice);
    while (choice)
    {
        printf("Please enter the name of the admin: ");
        scanf(" %[^\n]", newAdmin.username);
        printf("Please enter the password: ");
        scanf(" %[^\n]", newAdmin.password);
        newAdmin.userID = getNewUserID3();

        printf("Your Admin ID is : %d\n", newAdmin.userID);
        write(fd, &newAdmin, sizeof(admin));
        printf("Do you want to continue(No[0]/Yes[1])? ");
        scanf("%d", &choice);
    }
    printf("\n");
    close(fd);

    fd = open("NormalUser", O_RDWR | O_CREAT, 0744);
    choice = 1;
    normalUser newNormalUser;
    printf("-----NORMAL USER-----\n");
    printf("Please enter the name of the normal user: ");
    scanf(" %[^\n]", newNormalUser.name);
    printf("Please enter the password: ");
    scanf(" %[^\n]", newNormalUser.password);

    newNormalUser.userID = 1000;
    newNormalUser.balance = 1000; //Initial balance of Rs.1000
    newNormalUser.account_no = (newNormalUser.userID - 1000) + 100000;
    strcpy(newNormalUser.status, "ACTIVE");

    printf("Your userID is : %d\n", newNormalUser.userID);
    write(fd, &newNormalUser, sizeof(normalUser)); //Adding the new user
    printf("Do you want to continue(No[0]/Yes[1])? ");
    scanf("%d", &choice);
    while (choice)
    {
        printf("Please enter the name of the normal user: ");
        scanf(" %[^\n]", newNormalUser.name);
        printf("Please enter the password: ");
        scanf(" %[^\n]", newNormalUser.password);

        newNormalUser.userID = getNewUserID1();
        newNormalUser.balance = 1000;
        newNormalUser.account_no = (newNormalUser.userID - 1000) + 100000;
        strcpy(newNormalUser.status, "ACTIVE");

        printf("Your userID is : %d\n", newNormalUser.userID);
        write(fd, &newNormalUser, sizeof(normalUser));
        printf("Do you want to continue(No[0]/Yes[1])? ");
        scanf("%d", &choice);
    }
    printf("\n");
    close(fd);

    fd = open("JointUser", O_RDWR | O_CREAT, 0744);
    choice = 1;
    jointUser newJointUser;
    printf("-----JOINT USER-----\n");
    printf("Please enter the name of the first joint user: ");
    scanf(" %[^\n]", newJointUser.name1);
    printf("Please enter the name of the second joint user: ");
    scanf(" %[^\n]", newJointUser.name2);
    printf("Please enter the password: ");
    scanf(" %[^\n]", newJointUser.password);

    newJointUser.userID = 1000;
    newJointUser.balance = 1000;
    newJointUser.account_no = (newJointUser.userID - 1000) + 100000;
    strcpy(newJointUser.status, "ACTIVE");

    printf("Your joint userID is : %d\n", newJointUser.userID);
    write(fd, &newJointUser, sizeof(jointUser));
    printf("Do you want to continue(No[0]/Yes[1])? ");
    scanf("%d", &choice);
    while (choice)
    {
        printf("Please enter the name of the first joint user: ");
        scanf(" %[^\n]", newJointUser.name1);
        printf("Please enter the name of the second joint user: ");
        scanf(" %[^\n]", newJointUser.name2);
        printf("Please enter the password: ");
        scanf(" %[^\n]", newJointUser.password);

        newJointUser.userID = getNewUserID2();
        newJointUser.balance = 1000;
        newJointUser.account_no = (newJointUser.userID - 1000) + 100000;
        strcpy(newJointUser.status, "ACTIVE");

        printf("Your joint userID is : %d\n", newJointUser.userID);
        write(fd, &newJointUser, sizeof(jointUser));
        printf("Do you want to continue(No[0]/Yes[1])? ");
        scanf("%d", &choice);
    }
    printf("\n");
    close(fd);
    return 0;
}