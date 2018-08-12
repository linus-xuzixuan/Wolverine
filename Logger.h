/* Logger.h
By Linus Xu (linus-xuzixuan on GitHub)
*/

#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <iostream>
#include "Player.h"

using namespace std;
ofstream fout;

void openfile(){
    fout.open("./LRS.log");
    if(fout.fail()){
        cout<<"Log opening failed.";
    }
}

void logid(int numWolverines,int numPowers,int numVillagers,Player Players[]){
    fout<<"Total: "<<numWolverines+numPowers+numVillagers<<" players."<<endl;
    fout<<numWolverines<<" wolverines, "<<numPowers<<" powers, and "<<numVillagers<<" villagers."<<endl;
    fout<<endl<<"Identities:"<<endl;
    for(int i=0;i<numWolverines+numPowers+numVillagers;i++){
        
    }
}

#endif //LOGGER_H