#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
int main()
{
    char w1[200] = {};
    char w2[] = "\\----Say Hello to s1083343!----\\\n";
    int ifs = open("blake.txt", O_RDWR, 0700);
    if (ifs < 0)
        cout << "Failed to open file.\n";
    else
    {
        read(ifs, w1, 200);
        close(ifs);
    }
    int fout = open("happy.tmp", O_RDWR | O_CREAT, 0700);
    int fout2 = open("happy2.tmp", O_RDWR | O_CREAT, 0700);
    if (!fout || !fout2)
        cout << "Open Error";
    else
    {
        write(fout, w2, 34);
        write(fout2, w2, 34);
        write(fout, w1, 200);
        write(fout2, w1, 200);
        close(fout);
        close(fout2);
    }
}
