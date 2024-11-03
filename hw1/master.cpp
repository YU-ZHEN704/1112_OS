#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>

using std::cout; using std::endl;

int main() {
    pid_t c_pid = fork();
    if (c_pid == 0)
        //system("./mmv.out");
        execl("./mmv.out", "ls", "-l", NULL);
    else
        cout << "Successful  (#" << getpid() << ")!" << endl;
}
