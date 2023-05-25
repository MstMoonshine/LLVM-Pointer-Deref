#include <stdio.h>

const char *ptr_types[] = {"load\t\t", "store\t\t", "getelementptr\t"};
void pointer_deref(char *ptr, char type)
{
    printf("[%s] %p\n", ptr_types[type], ptr);
}

// -----

struct Test {
    int a;
    int b;
};

int foo(struct Test *t)
{
    long b = (long)(t->b) + 1;
    return (int)b;
}

char bar()
{
    struct Test t = {
        .a = 123,
        .b = 345
    };

    return (char)foo(&t);
}

int main()
{
    char res = bar();
    return 0;
}
