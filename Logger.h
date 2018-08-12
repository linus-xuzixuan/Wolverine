/* Logger.h
By Linus Xu (linus-xuzixuan on GitHub)
*/

#include <fstream>
#include <iostream>

using namespace std;
ofstream fout;

void openfile(){
    fout.open("./LRS.log");
    if(fout.fail()){
        cout<<"Log opening failed.";
    }
}

void logid(int numWolverines,int numPowers,int numVillagers){
    fout<<"Total: "<<numWolverines+numPowers+numVillagers<<" players."<<endl;
    fout<<numWolverines<<" wolverines, "<<numPowers<<" powers, and "<<numVillagers<<" villagers.";
    
}