#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>

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

bool checkNormalUser(normalUser currUser)
{
    int i = currUser.userID - 1000; //Acts as 1-index
    int fd = open("NormalUser", O_RDONLY, 0744);
    bool result;
    normalUser temp;

    //Read Lock
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser); //nth record[In 1 base indexing]
    lock.l_len = sizeof(normalUser);         //sizeof(record)
    lock.l_pid = getpid();

    int fl1 = fcntl(fd, F_SETLKW, &lock); //Lock the selected record
    //getchar();

    lseek(fd, (i) * sizeof(normalUser), SEEK_SET); //Going to the file pointer to the selected record
    read(fd, &temp, sizeof(normalUser));           //Reading that selected record to check!
    if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result; //Returns true if Password matches to the ID and Account is ACTIVE
}

bool checkJointUser(jointUser currUser)
{
    int i = currUser.userID - 1000;
    int fd = open("JointUser", O_RDONLY, 0744);
    bool result;
    jointUser temp;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser);
    lock.l_len = sizeof(jointUser);
    lock.l_pid = getpid();

    int fl1 = fcntl(fd, F_SETLKW, &lock); //lock the selected record

    lseek(fd, (i) * sizeof(jointUser), SEEK_SET); //Going to the file pointer to the selected record
    read(fd, &temp, sizeof(jointUser));
    if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool checkAdmin(admin currUser)
{
    int i = currUser.userID - 1000;
    int fd = open("Admin", O_RDONLY, 0744);
    bool result;
    admin temp;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(admin);
    lock.l_len = sizeof(admin);
    lock.l_pid = getpid();

    int fl1 = fcntl(fd, F_SETLKW, &lock); //lock the selected record

    lseek(fd, (i) * sizeof(admin), SEEK_SET); //Going to the file pointer to the selected record
    read(fd, &temp, sizeof(admin));
    if (!strcmp(temp.password, currUser.password))
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool depositMoney(int accType, int ID, float amt)
{
    int i = ID - 1000;

    if (accType == 1) //Corresponding to Normal User
    {
        int fd = open("NormalUser", O_RDWR, 0744);
        bool result;

        //Write Lock
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser); //nth record[based on the index i]
        lock.l_len = sizeof(normalUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock); //Locking the selected record
        getchar();

        normalUser currUser;                           //temp struct to read the data
        lseek(fd, (i) * sizeof(normalUser), SEEK_SET); //Going to the file pointer to the selected record
        read(fd, &currUser, sizeof(normalUser));       //Reading the selected record

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amt;
            lseek(fd, sizeof(normalUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(normalUser)); //Writing the changes back in the file
            result = true;
        }
        else
        {
            result = false;
        }
        lock.l_type = F_UNLCK; //Unlocking the record
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    else if (accType == 2) //Corresponding to JointUser
    {
        int fd = open("JointUser", O_RDWR, 0744);
        bool result;

        //Write Lock
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser);
        lock.l_len = sizeof(jointUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock);

        jointUser currUser;
        lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
        read(fd, &currUser, sizeof(jointUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amt;
            lseek(fd, sizeof(jointUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(jointUser));
            result = true;
        }
        else
        {
            result = false;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    return false;
}

bool withdrawMoney(int accType, int ID, float amt)
{
    int i = ID - 1000;

    if (accType == 1) //Normal User
    {
        int fd = open("NormalUser", O_RDWR, 0744);
        bool result;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser); //nth record
        lock.l_len = sizeof(normalUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock);

        normalUser currUser;
        lseek(fd, (i) * sizeof(normalUser), SEEK_SET);
        read(fd, &currUser, sizeof(normalUser));

        if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amt)
        {
            currUser.balance -= amt;
            lseek(fd, sizeof(normalUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(normalUser)); //Writing the changes back
            result = true;
        }
        else
        {
            result = false;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    else if (accType == 2) //JointUser
    {
        int fd = open("JointUser", O_RDWR, 0744);
        bool result;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser);
        lock.l_len = sizeof(jointUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock);

        jointUser currUser;
        lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
        read(fd, &currUser, sizeof(jointUser));

        if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amt)
        {
            currUser.balance -= amt;
            lseek(fd, sizeof(jointUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(jointUser));
            result = true;
        }
        else
        {
            result = false;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return false;
}

//Balance Enquiry
float getBalance(int accType, int ID)
{
    int i = ID - 1000;
    float result;
    if (accType == 1) //NormalUser
    {
        int i = ID - 1000;
        int fd = open("NormalUser", O_RDONLY, 0744);
        normalUser temp;

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser);
        lock.l_len = sizeof(normalUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(normalUser), SEEK_SET);
        read(fd, &temp, sizeof(normalUser));
        if (!strcmp(temp.status, "ACTIVE"))
            result = temp.balance;
        else
            result = -1;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    else if (accType == 2) //JointUser
    {
        int i = ID - 1000;
        int fd = open("JointUser", O_RDONLY, 0744);
        jointUser temp;

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser);
        lock.l_len = sizeof(jointUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
        read(fd, &temp, sizeof(jointUser));
        if (!strcmp(temp.status, "ACTIVE"))
            result = temp.balance;
        else
            result = -1;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    return 0;
}

//Password Change
bool alterPassword(int accType, int ID, char newPwd[10])
{
    int i = ID - 1000;
    if (accType == 1) //NormalUser
    {
        int fd = open("NormalUser", O_RDWR, 0744);
        bool result;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser);
        lock.l_len = sizeof(normalUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock);

        normalUser currUser;
        lseek(fd, (i) * sizeof(normalUser), SEEK_SET);
        read(fd, &currUser, sizeof(normalUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.password, newPwd);
            lseek(fd, sizeof(normalUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(normalUser)); //Writing the new changes
            result = true;
        }
        else
        {
            result = false;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    else if (accType == 2) //JointUser
    {
        int fd = open("JointUser", O_RDWR, 0744);
        bool result;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser);
        lock.l_len = sizeof(jointUser);
        lock.l_pid = getpid();

        int fl1 = fcntl(fd, F_SETLKW, &lock);

        jointUser currUser;
        lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
        read(fd, &currUser, sizeof(jointUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.password, newPwd);
            lseek(fd, sizeof(jointUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(jointUser));
            result = true;
        }
        else
        {
            result = false;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    return false;
}

normalUser getNormalUser(int ID)
{
    int i = ID - 1000;
    normalUser currUser;
    int fd = open("NormalUser", O_RDONLY, 0744);

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser); //Getting the ith record
    lock.l_len = sizeof(normalUser);
    lock.l_pid = getpid();

    int fl1 = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i) * sizeof(normalUser), SEEK_SET);
    read(fd, &currUser, sizeof(normalUser)); //Getting the entire user detail based on its ID, index

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser; //Return the temp struct data is reading
}

jointUser getJointUser(int ID)
{
    int i = ID - 1000;
    jointUser currUser;
    int fd = open("JointUser", O_RDONLY, 0744);

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser);
    lock.l_len = sizeof(jointUser);
    lock.l_pid = getpid();

    int fl1 = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
    read(fd, &currUser, sizeof(jointUser));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

admin getAdmin(int ID)
{
    int i = ID - 1000;
    admin currUser;
    int fd = open("Admin", O_RDONLY, 0744);
    int fl1;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(admin);
    lock.l_len = sizeof(admin);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i) * sizeof(admin), SEEK_SET);
    read(fd, &currUser, sizeof(admin));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

bool addNormalUser(normalUser record)
{
    int fd = open("NormalUser", O_RDWR, 0744);
    bool result;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = (-1) * sizeof(normalUser); //Locking the last record
    lock.l_len = sizeof(normalUser);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    normalUser endUser; //Temp struct to read the last record
    lseek(fd, (-1) * sizeof(normalUser), SEEK_END);
    read(fd, &endUser, sizeof(normalUser)); //Reading the last record

    record.userID = endUser.userID + 1;         //Assigning user ID to new record
    record.account_no = endUser.account_no + 1; //Assigning account no to the new record
    strcpy(record.status, "ACTIVE");

    int j = write(fd, &record, sizeof(normalUser));
    if (j != 0)
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool addJointUser(jointUser record)
{
    int fd = open("JointUser", O_RDWR, 0744);
    bool result;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = (-1) * sizeof(jointUser);
    lock.l_len = sizeof(jointUser);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    jointUser endUser;
    lseek(fd, (-1) * sizeof(jointUser), SEEK_END);
    read(fd, &endUser, sizeof(jointUser));

    record.userID = endUser.userID + 1;
    record.account_no = endUser.account_no + 1;
    strcpy(record.status, "ACTIVE");

    int j = write(fd, &record, sizeof(jointUser));
    if (j != 0)
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool deleteNormalUser(int ID)
{
    int i = ID - 1000;
    int fd = open("NormalUser", O_RDWR, 0744);
    bool result = false;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser); //Accessing that particular record based on index i
    lock.l_len = sizeof(normalUser);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    normalUser currUser;
    lseek(fd, (i) * sizeof(normalUser), SEEK_SET);
    read(fd, &currUser, sizeof(normalUser));

    if (!strcmp(currUser.status, "ACTIVE"))
    {
        strcpy(currUser.status, "CLOSED"); //Because deleting would cause holes
        currUser.balance = 0;

        lseek(fd, (-1) * sizeof(normalUser), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(normalUser)); //Writing the changes back
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool deleteJointUser(int ID)
{
    int i = ID - 1000;
    int fd = open("JointUser", O_RDWR, 0744);
    bool result = false;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser);
    lock.l_len = sizeof(jointUser);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    jointUser currUser;
    lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
    read(fd, &currUser, sizeof(jointUser));

    if (!strcmp(currUser.status, "ACTIVE"))
    {
        strcpy(currUser.status, "CLOSED");
        currUser.balance = 0;

        lseek(fd, (-1) * sizeof(jointUser), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(jointUser));
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool modifyNormalUser(normalUser modUser)
{
    int i = modUser.userID - 1000;
    int fd = open("NormalUser", O_RDWR, 0744);
    bool result = false;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser);
    lock.l_len = sizeof(normalUser);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    normalUser currUser;
    lseek(fd, (i) * sizeof(normalUser), SEEK_SET);
    read(fd, &currUser, sizeof(normalUser)); //Reading the ith record into currUser

    if (!strcmp(currUser.status, "ACTIVE") && (modUser.account_no == currUser.account_no))
    {
        lseek(fd, (-1) * sizeof(normalUser), SEEK_CUR);
        int j = write(fd, &modUser, sizeof(normalUser)); //Writing the changes
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool modifyJointUser(jointUser modUser)
{
    int i = modUser.userID - 1000;
    int fd = open("JointUser", O_RDWR, 0744);
    bool result = false;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser);
    lock.l_len = sizeof(jointUser);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    jointUser currUser;
    lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
    read(fd, &currUser, sizeof(jointUser));

    if (!strcmp(currUser.status, "ACTIVE") && (modUser.account_no == currUser.account_no))
    {
        lseek(fd, (-1) * sizeof(jointUser), SEEK_CUR);
        int j = write(fd, &modUser, sizeof(jointUser));
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

void serverTask(int nsd)
{
    int msgLength, select, type, option, accType, currUserID;
    bool result;
    while (1)
    {
        read(nsd, &option, sizeof(option));
        printf("Option Chosen: %d\n", option);
        //Option value is the same as what client enters in chooseOption function

        if (option == 1) //User is a Normal User
        {
            normalUser currUser1;
            accType = 1; //Assigning the normal account type as 1
            msgLength = read(nsd, &currUser1, sizeof(normalUser));
            printf("Entered Username : %d\n", currUser1.userID);
            printf("Entered Password : %s\n", currUser1.password);
            currUserID = currUser1.userID;       //Assigning a variable for current user ID
            result = checkNormalUser(currUser1); //Checks if Username and Password are present in NormalUser
            write(nsd, &result, sizeof(result)); //Returns a bool result
        }
        else if (option == 2) //JointUser
        {
            jointUser currUser2;
            accType = 2;
            msgLength = read(nsd, &currUser2, sizeof(jointUser));
            currUserID = currUser2.userID;
            printf("Entered Username : %d\n", currUser2.userID);
            printf("Entered Password : %s\n", currUser2.password);
            result = checkJointUser(currUser2);
            write(nsd, &result, sizeof(result));
        }
        else if (option == 3) //Admin
        {
            admin currUser3;
            accType = 3;
            msgLength = read(nsd, &currUser3, sizeof(admin));
            currUserID = currUser3.userID;
            printf("Entered Username : %d\n", currUser3.userID);
            printf("Entered Password : %s\n", currUser3.password);
            result = checkAdmin(currUser3);
            write(nsd, &result, sizeof(result));
        }
        else
        {
            result = false;
            write(nsd, &result, sizeof(result));
        }
        if (result)
            break;
    }

    //Once you log in
    while (1)
    {
        read(nsd, &select, sizeof(int)); //Reading the value of select
        if (option == 1 || option == 2)  // Normal User or Joint User
        {
            if (select == 1) //select=1 corresponds to Deposit
            {
                float amt;
                read(nsd, &amt, sizeof(float)); //Reading the amount to be deposited
                result = depositMoney(accType, currUserID, amt);
                if (result)
                    printf("\nAmount of Rs.%0.2f deposited!\n", amt);
                //Account type is the same as assigned earlier
                //Using the assigned variable for current user ID
                write(nsd, &result, sizeof(result)); //Giving the bool result to client
            }
            else if (select == 2) //select=2 corresponds to Withdraw
            {
                float amt;
                read(nsd, &amt, sizeof(float));
                result = withdrawMoney(accType, currUserID, amt);
                if (result)
                    printf("\nAmount of Rs.%0.2f withdrawn!\n", amt);
                write(nsd, &result, sizeof(result));
            }
            else if (select == 3) //select=3 corresponds to balance enquiry
            {
                float amt;
                amt = getBalance(accType, currUserID);
                printf("\nCurrent Amount Displayed : Rs.%0.2f\n", amt);
                write(nsd, &amt, sizeof(float));
            }
            else if (select == 4) //select=4 corresponds to password change
            {
                char pwd[10];
                read(nsd, pwd, sizeof(pwd));                      //Reading the new password
                result = alterPassword(accType, currUserID, pwd); //Bool return value
                printf("\nUser Password Changed!\n");
                write(nsd, &result, sizeof(result));
            }
            else if (select == 5) //select=5 corresponds to getting user account details
            {
                if (option == 1) //Corresponds to normal user
                {
                    normalUser user1 = getNormalUser(currUserID);
                    write(nsd, &user1, sizeof(normalUser));
                }
                else if (option == 2) //Joint User
                {
                    jointUser user2 = getJointUser(currUserID);
                    write(nsd, &user2, sizeof(jointUser));
                }
            }
            else if (select == 6)
                break; //Corresponding to exit
        }
        else if (option == 3) //Admin
        {
            read(nsd, &type, sizeof(int));
            if (select == 1) //select=1 corresponds to Add Account
            {
                if (type == 1) //Type=1 For Normal User
                {
                    normalUser newUser1;                      //Temp struct to read data coming from client
                    read(nsd, &newUser1, sizeof(normalUser)); //Reading the entered user details
                    result = addNormalUser(newUser1);         //Bool result
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2) //Joint User
                {
                    jointUser newUser2;
                    read(nsd, &newUser2, sizeof(jointUser));
                    result = addJointUser(newUser2);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 2) //select=2 corresponds to Delete account
            {
                if (type == 1) //Normal User
                {
                    int delUserID1;
                    read(nsd, &delUserID1, sizeof(int));
                    result = deleteNormalUser(delUserID1); //bool result
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2) //JointUser
                {
                    int delUserID2;
                    read(nsd, &delUserID2, sizeof(int));
                    result = deleteJointUser(delUserID2);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 3) //select=3 corresponds to modify account
            {
                if (type == 1) //Normal User
                {
                    normalUser modUser1; //Temp struct to read the new details of account
                    read(nsd, &modUser1, sizeof(normalUser));
                    result = modifyNormalUser(modUser1);
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2) //JointUser
                {
                    jointUser modUser2;
                    read(nsd, &modUser2, sizeof(jointUser));
                    result = modifyJointUser(modUser2);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 4) //select=4 corresponds to search account
            {
                if (type == 1) //Normal User
                {
                    normalUser searchUser1;
                    int userID1;
                    read(nsd, &userID1, sizeof(int));     //Reading User ID
                    searchUser1 = getNormalUser(userID1); //Same as user asking for his details
                    write(nsd, &searchUser1, sizeof(normalUser));
                }
                else if (type == 2) //JointUser
                {
                    jointUser searchUser2;
                    int userID2;
                    read(nsd, &userID2, sizeof(int));
                    searchUser2 = getJointUser(userID2);
                    write(nsd, &searchUser2, sizeof(jointUser));
                }
            }
            else if (select == 5)
                break; //To get out of the loop and end current session
        }
    }
    close(nsd); //Server closing the client session
    write(1, "Ended client session.....\n", sizeof("Ended client session.....\n"));
    return;
}

int main()
{
    struct sockaddr_in server, client;
    int sd, nsd, clientLen;
    bool result;
    sd = socket(AF_INET, SOCK_STREAM, 0); //Socket Descriptor of the server

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5557);

    bind(sd, (struct sockaddr *)&server, sizeof(server));
    listen(sd, 2);

    write(1, "Waiting for the client.....\n", sizeof("Waiting for the client.....\n"));
    while (1)
    {
        clientLen = sizeof(client);
        nsd = accept(sd, (struct sockaddr *)&client, &clientLen); //Socket descriptor of the client

        write(1, "\nConnected to the client.....\n\n", sizeof("\nConnected to the client.....\n\n"));
        if (!fork()) //Fork to make it concurrent
        {
            close(sd); //child doesn't require sd
            serverTask(nsd);
            exit(0);
        }
        else
        {
            close(nsd); //child has nsd and parent doesn't require it
        }
    }
    return 0;
}