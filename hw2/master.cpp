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
	string infile = "blake.txt";
	string outfile = "happy.tmp";
	//./master.out A.txt B.tmp 
	if(argc > 1){
		infile = argv[1];
		outfile = argv[2];
	}
	
	const char* infile_name = infile.c_str();
	const char* outfile_name = outfile.c_str();
	
	int ifs = open(infile_name, O_RDWR, 0700);
	if (ifs < 0)
		cout << "Fail to open file.\n";
		
	else{
		int p[2];
		pipe(p);
		//child, write file(read from pipe)
		if(fork() == 0){
			close(0);
			dup(p[0]);
			close(p[0]);
			close(p[1]);
			execlp("./mmv.out", "./mmv.out", outfile_name, NULL);
		}
		//parent, read file(write into pipe)
		else{
			char w1[200] = {};
			read(ifs, w1, 200);
			write(p[1], w1, 200);
			close(p[0]);
			close(p[1]);
			close(ifs);
		}
	}
}
