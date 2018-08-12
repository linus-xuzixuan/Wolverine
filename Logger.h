/* Logger.h
By Linus Xu (linus-xuzixuan on GitHub)
*/

#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include "Player.h"

using namespace std;
ofstream fout;

void openfile(){
    fout.open("./LRS.log");
    if(fout.fail()){
        cout<<"Log opening failed.";
        exit(1);
    }
}

void logid(int numWolverines,int numPowers,int numVillagers,Player Players[]){
    fout<<"Total: "<<numWolverines+numPowers+numVillagers<<" players."<<endl;
    fout<<numWolverines<<" wolverines, "<<numPowers<<" powers, and "<<numVillagers<<" villagers."<<endl;
    fout<<endl<<"Identities:"<<endl;
    for(int i=0;i<numWolverines+numPowers+numVillagers;i++){
        fout<<"Player "<<i+1<<" is a "<<Players[i].get_identity()<<"."<<endl;
    }
}

void lognight(Player Players[],int killed1,int killed2){
    fout<<"Wolverine(s) killed Player "<<abs(killed1);
    if(killed1<0){
        fout<<", but was rescued by the Witch."<<endl;
    }else{
        fout<<"."<<endl;
    }
}

#endif //LOGGER_H