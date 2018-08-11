/* Logger.h
By Linus Xu (linus-xuzixuan on GitHub)
*/

#include <fstream>
#include <iostream>

using namespace std;
fstream fin;

void openfile(){
    fin.open("./LRS.log");
    if(fin.fail()){
        cout<<"Log opening failed.";
    }
}

void logid(int numWolverines,int numPowers,int numVillagers){

}