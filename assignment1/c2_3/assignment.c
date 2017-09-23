
int g; // uninitialized global
int main()
{
    int a, b, c, d, e, f; // local variables
    a=b=c=d=e=f=1;
    g = sum(a,b,c,d,e,f);
}


