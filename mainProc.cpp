#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        perror("pipes");
        exit(EXIT_FAILURE);
    }

    dup2(std::atoi(argv[1]), STDIN_FILENO);
    dup2(std::atoi(argv[2]), STDOUT_FILENO);

    char buffer[256];

    while (!feof(stdin))
    {
        size_t bytes = fread(buffer, 1, 1, stdin);

        fwrite(buffer, 1, bytes, stdout);
        fflush(stdout); //флашем в stdout, чтобы не ждать EOF от закрытия пайпы,
                        //а сразу можно было считать данные через read в родительском процессе
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);

    return 0;
}
