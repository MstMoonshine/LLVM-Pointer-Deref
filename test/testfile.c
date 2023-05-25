#include <stdio.h>

int foo(long a)
{
    long b = a + 1;
    return (int)b;
}

char bar()
{
    return (char)foo(32);
}

int main()
{
    char res = bar();
    return 0;
}
