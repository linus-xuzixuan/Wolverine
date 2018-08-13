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
    fout<<endl;
}

void lognight(Player Players[],int killed1,int killed2,int verify){
    //Logging Wolverine's kill
    fout<<"Wolverine(s) killed Player "<<abs(killed1);
    if(killed1<0){
        fout<<", but was rescued by the Witch."<<endl;
    }else{
        fout<<"."<<endl;
    }

    //Logging Witch's poison (if any)
    if(killed2!=0){
        fout<<"Witch poisoned Player "<<killed2<<endl;
    }

    //Logging Predictor's verification
    fout<<"Predictor checked Player "<<verify;
    string id=Players[verify-1].get_identity();
    if(id=="Wolverine"){
        fout<<", who is bad."<<endl;
    }else{
        fout<<", who is good."<<endl;
    }
    fout<<endl;
}

void logday(Player Players[],int votekill){
    if(votekill!=0){
        fout<<"Player "<<votekill<<" was voted out."<<endl;
    }else{
        fout<<"Peace today."<<endl;
    }
    fout<<endl;
}

void loghunter(Player Players[],int hunterkill){
    fout<<"Hunter shot Player "<<hunterkill<<"."<<endl;
    fout<<endl;
}

#endif //LOGGER_H