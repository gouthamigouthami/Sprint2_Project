#include<iostream>
#include<thread>
using namespace std;
void foo(int z)
{
	for(int i=0;i<z;i++){
		cout<<"threading finction pointer as callable\n";}
}
class thread_obj{
	public:
		void operator()(int x){
			for(int i=0;i<x;i++){
			cout<<"thread using function object as called\n";
			}
		}
		};
int main(){
cout<<"thread 1 and 2 and 3 operating independently"<<endl;
thread th1(foo,3);
thread th2(thread_obj(),3);
auto f=[](int x) 
{
	for(int i=0;i<x;i++){
		cout<<"threading using lambda function callable\n";
	}
};
thread th3(f,3);
th1.join();
th2.join();
th3.join();
return 0;
}
		
