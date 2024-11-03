#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
using namespace std;

int main(int argc,char *argv[])
{
    int fout = open(argv[1], O_RDWR | O_CREAT, 0700);
    if (!fout){
        cout << "fail to open file.\n";
    }
    write(fout, "\\----Say Hello to s1083343!----\\\n", 34);
    string w,tmp;
    while(getline(cin,tmp))
    {
    	w += tmp;
    	w += "\n";
    }
    const char* w2=w.c_str();
    
    write(fout, w2, 200);
    close(fout);
    cout << "Successful (#" << getpid() << ")!\n";
}
