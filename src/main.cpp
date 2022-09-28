#include <iostream>

using namespace std;

int main(int argc, char** argv){
    cout<<"Arg is:"<<endl;
    for(int i = 0;i<argc;i++){
        cout<<argv[i]<<endl;
    }
    cout<<"Exiting"<<endl;

    return 0;
}