


#include <stdio.h>
int main()
{
    int c; // input char
    int state = 1; // initial current state = S1
    FILE *fp = fopen("cprogram.c", "r"); // input is a C program


    while((c=fgetc(fp))!= EOF){
        switch(state){
            case 1:
                switch(c){
                    // if EOF: terminate
                    // switch based on current state
                    // state S1
                    // next states/outputs
                    case '/' : state = 2; break;
                    case '\"' : state = 4; printf("%c", c);break;
                    case '\n': state = 1; printf("%c", c);break;
                    default : state = 1; printf("%c", c); break;
                };break;
            case 2:
                switch(c){
                    // state S2
                    // next states/outputs
                    case '/' : state = 3;break;
                    case '\"' : state = 4; printf("/%c", c);break;
                    case '\n': state = 1; printf("/%c", c); break;
                    default: state = 1; printf("/%c", c); break;
                }; break;
            case 3:
                switch(c){
                    // state S3
                    // next states/outputs
                    case '/' : state = 3; break;
                    case '\n': state = 1; printf("%c", c); break;
                    default : state = 3; break;
                }; break;
            case 4:
                switch(c){
                    // state S4
                    // next states/outputs
                    case '\"' : state = 1; printf("%c", c); break;
                    case '\\' : state = 5; printf("%c", c); break;
                    //case '\n': state = 1; printf("%c", c); break;
                    default : state = 4; printf("%c", c); break;
                }; break;
            case 5:
                switch(c){
                    // state S5
                    // next states/outputs
                    default : state = 4; printf("%c", c); break;
                }; break;
        }
    }
}
