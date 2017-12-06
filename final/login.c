
#include "ucode.c"
#include "util.c"


typedef struct 
{
    int populated;
    char username[50];
    char password[50];
    int gid;
    int pid;
    char fullname[50];
    char homedir[100];
    char program[50];
} User;

void printUserInfo(User* user)
{
    if (user->populated == 0)
    {
        printf("user is unpopulated.\n");
        return;
    }

    printf("username: %s\n", user->username);
    printf("password: %s\n", user->password);
    printf("gid: %d\n", user->gid);
    printf("pid: %d\n", user->pid);
    printf("fullname: %s\n", user->fullname);
    printf("homedir: %s\n", user->homedir);
    printf("program: %s\n", user->program);
}


/*
    output: userList, array of users populated with information from /etc/passwd
*/
void parseLoginFile(User * o_userList)
{
    // open login file
    int fd = open("/etc/passwd", O_RDONLY);

    // parse a user
    int result = 1;
    User * currentUser = o_userList;

    while (result != 0)
    {
        result = parseUser(fd, currentUser);
        currentUser++;
    }

    close(fd);
}

int parseUser(int fd, User * user)
{
    char line[300];
    int result = readline(fd, line);

    if (result == 0)
    {
        user->populated = 0;
        return 0;
    }

    // get the username
    char* new_line = strtok(line, ':');
    char* username = line;
    strcpy(user->username, username);

    // get the password
    char* password = new_line;
    new_line = strtok(new_line, ':');
    strcpy(user->password, password);

    // get the gid
    char* gid = new_line;
    new_line = strtok(new_line, ':');
    int i_gid = strToInt(gid);
    user->gid = i_gid;

    // get the pid
    char* pid = new_line;
    new_line = strtok(new_line, ':');
    int i_pid = strToInt(pid);
    user->pid = i_pid;

    // get the full name
    char* full_name = new_line;
    new_line = strtok(new_line, ':');
    strcpy(user->fullname, full_name);

    // get the homedir
    char* homedir = new_line;
    new_line = strtok(new_line, ':');
    strcpy(user->homedir, homedir);

    // get the start program
    char* start_program = new_line;
    new_line = strtok(new_line, ':');
    normalize(start_program);
    strcpy(user->program, start_program);

    // mark the user structure as populated and return
    user->populated = 1;

    return 1;
}

// get user with the given username from the user list
User* getUsername(char* username, User* userlist)
{
    User* currentUser = userlist;

    while(currentUser->populated == 1)
    {
        int result = strcmp(username, currentUser->username);
        if (result == 0)
        {
            return currentUser;
        }

        currentUser++;
    }
    
    return 0;
}


int main()
{
    // setup console IO
    setupConsole();

    // TODO remove
    printf("override login process, run shell\n");
    exec("sh");

    // parse the login file
    User users[20];
    parseLoginFile(users);

    User* desiredUser = 0;

    while(1)
    {
        // ask user for their username
        char username[50];
        printf("username: ");
        getline(username);
        printf("\n");

        // remove the /r or /n from the end of username
        normalize(username);

        // check the username is valid
        desiredUser = getUsername(username, users);
        if (desiredUser == 0)
        {
            printf("invalid username!\n");
            continue;
        }

        // ask user for their password
        char password[50];
        printf("password: ");
        getline(password);
        printf("\n");
        normalize(password);
        
        // check that thier password is valid
        int cmp = strcmp(password, desiredUser->password);
        if (cmp != 0)
        {
            printf("invalid password!\n");
            continue;
        }
        else
        {
            break;
        }
    }

    // desired user has logged in successfully

    // change directory to their home directory
    chdir(desiredUser->homedir);

    // take on the users uid
    chuid(desiredUser->pid, desiredUser->gid);

    // exec their startup program
    exec(desiredUser->program);
}



