
#include "../utility.c"


// test strtok
void test_strtok()
{
    char a[] = "abc def ghi j klmno pq rstuv    jeif woeif      ";
    
    char* b = strtok(a, ' ');
    if (strEq(b, "abc") == 0)
    {
        printf("strtok failure!");
    }

    printf("%s", a);
}


void test_streq()
{
    char a[] = "hello";
    char b[] = "hello";
    char c[] = "testing";

    int r = 0;

    // test positive case
    r = strEq(a, b);

    if (r != 1)
    {
        printf("streq failure!");
    }

    // test negative case
    r = strEq(a, c);

    if (r != 0)
    {
        printf("streq failure!");
    }
}


// test strcpy
void test_strcpy()
{
    char a[] = "testing hello";
    char b[50];

    strcpy(b, a);
    
    if (strEq(a, b) != 1)
    {
        printf("strcpy failure!");
    }
}


int main()
{
    test_streq();
    test_strcpy();
    test_strtok();
}



