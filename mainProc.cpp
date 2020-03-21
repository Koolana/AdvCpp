#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <exception>

int main(int argc, char *argv[])
{
    char buffer[256];

    while (!feof(stdin))
    {
        size_t bytes = fread(buffer, 1, 1, stdin);

        fwrite(buffer, 1, bytes, stdout);
        fflush(stdout); //флашем в stdout, чтобы не ждать EOF от закрытия пайпы,
                        //а сразу можно было считать данные через read в родительском процессе
    }

    return 0;
}
