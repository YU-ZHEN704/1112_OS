#include <iostream>
#include <queue>
#include <cstdlib> // 亂數相關函數
#include <ctime>   //時間相關函數
#include <time.h>
#include <vector>
#include<unistd.h> 
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")
using namespace std;
//clock_t start = clock(0);

vector<bool> dbc;
struct fish
{
	int num;//number
	int clock;//冷凍time
	clock_t limit;//保存期限
	int shape;//0:鮮魚 1:切割 2:罐頭
	clock_t start;
};

pthread_mutex_t lock;
int m = 15;//鮮魚數量
int n = 5;//備料區大小
fish* buf = new fish[m + 1];
queue<fish> box;//備料區
queue<fish> air;//冷凍庫
void wait(int millisecond)
{
usleep(millisecond*1000); //<unistd.h> convert to millisecond
}

int gt(clock_t start)
{
	clock_t end = (clock());
	int timeused;
	timeused = ((int)(end - start));
	return timeused;
}


void* doSomeThing(void* data)
{
	fish* temp = (fish*)data;
	pthread_mutex_init(&lock,NULL);
	
		pthread_mutex_lock(&lock);
		
		cout << (clock() ) << "ms--Fish#" << temp->num << ":enters the CUTTER" << endl;
		int w = rand() % 21 + 10; //10~30ms
		cout << (clock()) << "ms--CUTTER: cutting... cutting..." << "Fish#" << temp->num << "--" << w*10 << endl;
		wait(w);
		cout << (clock()) << "ms--" << "Fish#" << temp->num << ": leaves CUTTER (complete 1st stage)" << endl;
		return data;
		pthread_exit(0);
		pthread_mutex_unlock(&lock);
		
		//return data;
	//}
	//return NULL;
}

void* finish(void* data)
{
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_lock(&lock);
	

	fish* temp = (fish*)data;
	//temp->shape = 2;
	cout << (clock()) << "ms--Fish#" << temp->num << ":enters to the factory (CANNER)" << endl;
	temp->shape = 2;
	int w = (rand() % 6 + 5) * 10 ; //50~100;
	wait(w);
	cout << (clock()) << "ms--Fish#" << temp->num << ": leaves CANNER(Complete)" << endl;
	pthread_exit(data);
	//pthread_exit(temp);
	pthread_mutex_unlock(&lock);
	return data;
	//return NULL;
}

int main(int argc, char* argv[])
{
	m =atoi(argv[1]);
	n =atoi(argv[2]);
	srand(10);
	//int m = 20;//鮮魚數量
	//int n = 5;//備料區大小


	pthread_t op;
	pthread_t tid;
	pthread_t fin;
	cout << (clock()) << "ms--CUTTER: under maintenance." << endl;
	cout << (clock()) << "ms-- CANNER: under maintenance." << endl;
	cout << (clock()) << "ms-- CANNER: under reviewing together..." << endl;
	cout << (clock()) << "ms-- CUTTER: under reviewing together..." << endl;
	//pthread_create(&op, NULL, doSomeThing, (void*)&temp);
	for (int i = 1; i <= m; i++) //鮮魚倉儲分類作業
	{
		wait(rand() % 6 + 5);//5~10
		if (box.size() < n)
		{
			fish temp;
			temp.num = i;

			temp.shape = 0;
			temp.limit = 0;
			temp.clock = 0;
			temp.start = 0;
			cout << (clock()) << "ms--Fish#" << temp.num << ":waiting in the slot" << endl;
			box.push(temp);
		}
		else
		{
			fish temp;
			temp.num = i;
			temp.shape = 0;
			temp.limit = (clock());
			temp.clock = (rand() % 21 + 30);//30~50排隊
			temp.start = (clock());
			air.push(temp);
		}
	}
	
	while(true)
	{
		for (int i = 1; i <= air.size(); i++)
		{
			fish temp = air.front();
			if (gt(temp.limit) >= 1950)//魚壞掉
			{
				air.pop();
				cout << (clock()) << "ms--Fish#" << temp.num << ":fish is rotten(Abandon)" << endl;
			}
			else if (gt(temp.start) >= temp.clock)
			{
				if (box.size() < n)
				{
					/*pthread_create(&tid, NULL, doSomeThing, (void*)&temp);*/
					temp.limit = 0;
					temp.clock = 0;
					temp.start = 0;
					box.push(temp);
					cout << (clock()) << "ms--Fish#" << temp.num << ":waiting in the slot" << endl;
					air.pop();
				}
				else
				{
					temp.start = (clock());
					air.push(temp);
					air.pop();
				}
			}
			else
			{
				//temp.start = clock();
				air.push(temp);
				air.pop();
			}
		}
		if (dbc.size() == m)
			break;

		fish temp = box.front();
		if (temp.shape == 0)
		{
			//void* retval;
			int ret;
			int before = temp.num;
			
			pthread_create(&tid, NULL, doSomeThing, (void*)&temp);
			ret = pthread_join(tid, (void**)&temp);
			
			if (ret == 0)
			{
				temp.shape = 1;
				temp.num = before;
			}
			
			box.push(temp);
			cout << (clock()+10) << "ms --Fish#" << temp.num << ":waiting in the slot(cutted)" << endl;

			box.pop();
		}
		else if (temp.shape == 1)
		{

			int ret;
			int before = temp.num;
			pthread_create(&fin, NULL, finish, (void*)&temp);
			ret = pthread_join(fin, (void**)&temp);

			if (ret == 0 )
			{
				temp.shape = 2;
				temp.num = before;
				box.pop();
				dbc.push_back(1);
			}
			else
			{
				box.push(temp);
				box.pop();
			}
			
		}
		

	}


}


