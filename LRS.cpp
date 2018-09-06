/* LRS.cpp
By Linus Xu (linus-xuzixuan on GitHub)
Note: "power" here means “神” in the Chinese game.
This version currently works on Unix-based systems only as "clear" is a Shell command.
*/
#define W 0 //Witch
#define P 1 //Predictor
#define H 2 //Hunter
#define G 3 //Guard

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "Player.h"
#include "Logger.h"
using namespace std;

ifstream fin;
string spam; //For all kinds of pauses
bool over=0;
bool hunterfired=0;
bool saveused=0;
bool poisonused=0;
int hunter=-1;
int numPlayers;
char choice;
int numWolverines;
int numVillagers;
int numPowers;
bool guardpresent=0;
int killed1=0,killed2=0;
int day=0;

void setIdentity(int Wolverines,int Villagers,int Power,Player Players[],bool Present[]);
void startNight(Player Players[],int numPlayers);
void startDay(Player Players[],int result1,int result2,int hunter,int numPlayers);
bool checkover(Player Players[],int numWolverines,int numVillagers,int numPowers);
int findplayer(Player Players[],string identity,int numPlayers);

int main(void){
    int numPlayers;
    
    //Read Distrib.lrs file for player information
    openfile();
    cout<<"Please enter the number of players:";
    cin>>numPlayers;

    if (numPlayers<6 || numPlayers>12){
        cout<<"Not a valid player number."<<endl;
        return 1;
    }
    Player Players[numPlayers];
    
    fin.open("./Distrib.lrs");
    if(fin.fail()){
        cout<<"Error: Distribution file not found."<<endl;
        return 2;
    }

    for(int i=1;i<numPlayers;i++)
        fin.ignore(10000,'\n');
    int comp;
    fin>>comp;
    if(comp!=numPlayers){
        cout<<"Error: Distribution file corrupt."<<endl;
        return 3;
    }

    //Read the required line for number of each identity
    system("clear");
    fin>>numWolverines;
    cout<<"Wolverine count:"<<numWolverines<<endl;
    fin>>numVillagers;
    cout<<"Villager count:"<<numVillagers<<endl;
    numPowers=numPlayers-numWolverines-numVillagers;

    bool Present[4];
    int PowerCount;
    switch(numPlayers){
        case 6:
        case 7:
        case 8:
        PowerCount=2;
        break;

        case 9:
        case 10:
        case 11:
        PowerCount=3;
        break;

        case 12:
        PowerCount=4;
        break;
    }
    char Power[PowerCount];
    for(int i=0;i<PowerCount;i++){
        for(fin>>Power[i];Power[0]!='\n';fin.get(Power[0])){
            if(Power[i]==' '){
                continue;
            }else if(Power[i]=='W'){
                Present[W]=true;
                cout<<"1 witch"<<endl;
            }else if(Power[i]=='G'){
                Present[G]=true;
                guardpresent=true;
                cout<<"1 guard"<<endl;
            }else if(Power[i]=='P'){
                Present[P]=true;
                cout<<"1 predictor"<<endl;
            }else if(Power[i]=='H'){
                Present[H]=true;
                cout<<"1 hunter"<<endl;
            }else{
                cout<<"Error: unrecognized identity in distribution file (not W, G, P or H)"<<endl;
                return 4;
            }
        }
    }

    fin.close();
    system("sleep 2");
    setIdentity(numWolverines,numVillagers,PowerCount,Players,Present);
    system("clear");

    cout<<"Type anything to start identity confirmation:";
    cin>>spam;
    for(int j=0;j<numPlayers;j++){
        cout<<"Player "<<(j+1)<<", type something:";
        cin>>spam;
        cout<<"Player "<<(j+1)<<", you are "<<Players[j].get_identity()<<endl;
        cout<<"Type anything for confirmation:";
        cin>>spam;
        system("clear");
    }
    cout<<"End of identity confirmation."<<endl;
    cin.ignore(10000,'\n');
    system("sleep 3");

    //Iteration goes on until game over
    while(!over){
        day+=1;
        startNight(Players,numPlayers);
        startDay(Players,killed1,killed2,hunter,numPlayers);
    }
    
    endfile();
    return 0;
}

void setIdentity(int Wolverines,int Villagers,int Power,Player Players[],bool Present[]){
    int Left[Wolverines+Villagers+Power];
    srand(time(0));
    for(int j=0;j<Wolverines+Villagers+Power;j++){
        Players[j].set_life(1);
        defid:
        int a=rand()%(Wolverines+Villagers+Power)+1;
        if(Left[a-1]==1){goto defid;}
        if(a<=Wolverines){
            Players[j].set_id(1);
        }else if(a<=Villagers+Wolverines){
            Players[j].set_id(2);
        }else{
            if(Present[W]){
                Players[j].set_id(3);
                Present[W]=false;
            }else if(Present[P]){
                Players[j].set_id(4);
                Present[P]=false;
            }else if(Present[H]){
                Players[j].set_id(5);
                hunter=j;
                Present[H]=false;
            }else{
                Players[j].set_id(6);
                Present[G]=false;
            }
        }
        Left[a-1]=1;
    }
    logid(Wolverines,Power,Villagers,Players);
}

void startNight(Player Players[],int numPlayers){
    clear_guard(Players,numPlayers);
    int playerchosen;
    cout<<"Close your eyes please..."<<endl;
    system("sleep 3");
    system("clear");

    //Guard's turn
    int guard=-1;
    if(guardpresent){
        if(Players[findplayer(Players,"Guard",numPlayers)].get_state()!=0){
            cout<<"Guard!"<<endl;
            decide0:
            cout<<"Who do you want to protect tonight (0 for abandon)?";
            cin>>playerchosen;
            if(playerchosen==0){
                cout<<"Abandoned."<<endl;
            }else{
                if(playerchosen<0 || playerchosen>numPlayers){
                    cout<<"Not a valid player!"<<endl;
                    goto decide0;
                }
                if(Players[playerchosen-1].get_state()==0){
                    cout<<"Already dead!"<<endl;
                    goto decide0;
                }
                if(Players[playerchosen-1].unguard==true){
                    cout<<"You have protected this player yesterday night."<<endl;
                    goto decide0;
                }
                cout<<"Player "<<playerchosen<<", is that right?";
                cin>>choice;
                if(choice!='Y' && choice!='y')
                    goto decide0;
                Players[playerchosen-1].guard();
                guard=playerchosen;
                cout<<"Ok, he (she?) is likely to be safe tonight."<<endl;
            }
        }else{
            cout<<"Guard!"<<endl;
            cout<<"Who do you want to protect tonight (0 for abandon)?"<<endl;
            system("sleep 2");
            cout<<"You don't seem to have a choice, as you are dead."<<endl;
        }
        cout<<"Close your eyes..."<<endl;
        cin.ignore(10000,'\n');
        system("sleep 3");
        system("clear");
    }

    //Wolverine's turn
    cout<<"Wolverines!"<<endl;
    decide:
    cout<<"Who do you want to kill?"<<endl;
    cin>>playerchosen;

    if(playerchosen<1 || playerchosen>numPlayers){
        cout<<"Not a valid player!"<<endl;
        goto decide;
    }
    if(Players[playerchosen-1].get_state()==0){
        cout<<"Already dead!"<<endl;
        goto decide;
    }

    cout<<"Player "<<playerchosen<<", is that right?";
    cin>>choice;
    if(choice!='Y' && choice!='y')
        goto decide;

    if(Players[playerchosen-1].get_shield()==0){
        Players[playerchosen-1].set_life(2);
    }
    killed1=playerchosen;
    cout<<"Ok, Player "<<playerchosen<<" dead, at least for now..."<<endl;
    cout<<"Close your eyes..."<<endl;
    cin.ignore(10000,'\n');
    system("sleep 3");
    system("clear");

    //Witch's turn
    bool rescue=0;
    cout<<"Witch!"<<endl;
    if(Players[findplayer(Players,"Witch",numPlayers)].get_state()!=0){
        decide2:
        cout<<"Player "<<playerchosen<<" dead, rescue?";
        cin>>choice;
        if(choice=='Y' || choice=='y'){
            if(saveused){
                cout<<"Not valid. You have used your rescue."<<endl;
            }else{
                Players[playerchosen-1].set_life(1);
                rescue=true;killed1*=-1;saveused=1;
                cout<<"Ok, rescued."<<endl;
                if(Players[playerchosen-1].get_shield()==true){
                    Players[playerchosen-1].set_life(0);
                    killed1=-playerchosen;
                }
            }
        }else if(choice=='N' || choice=='n'){
                cout<<"Ok, he (she?) is probably dead."<<endl;
        }else{
            cout<<"Not a valid decision!"<<endl;
            goto decide2;
        }
        system("sleep 2");
        decide3:
        cout<<"Do you want to use poison?";
        cin>>choice;
        if(choice=='Y' || choice=='y'){
            if(rescue){
                cout<<"Not valid. You have rescued someone in this turn."<<endl;
            }else if(poisonused){
                cout<<"Not valid. You have used your poison."<<endl;
            }else{
                poisonused=true;
                decide31:
                cout<<"Who?";
                cin>>playerchosen;
                if(playerchosen<1 || playerchosen>numPlayers){
                    cout<<"Not a valid player!"<<endl;
                    goto decide31;
                }
                if(Players[playerchosen-1].get_state()==0){
                    cout<<"Already dead!"<<endl;
                    goto decide31;
                }
                cout<<"Player "<<playerchosen<<", is that right?";
                cin>>choice;
                if(choice!='Y' && choice!='y'){
                    goto decide31;
                }
                Players[playerchosen-1].set_life(0);
                killed2=playerchosen;

                cout<<"Ok, Player "<<playerchosen<<" poisoned."<<endl;
            }
        }else if(choice=='N' || choice=='n'){
            cout<<"Ok, nobody poisoned."<<endl;
        }else{
            cout<<"Not a valid decision!"<<endl;
            goto decide3;
        }
    }else{
        cout<<"Player "<<playerchosen<<" dead, rescue?"<<endl;
        system("sleep 2");
        cout<<"You don't seem to have a choice, as you are dead."<<endl;
        system("sleep 1");
        cout<<"Do you want to use poison?"<<endl;
        system("sleep 2");
        cout<<"Still no choice."<<endl;
    }
    cout<<"Close your eyes..."<<endl;
    cin.ignore(10000,'\n');
    system("sleep 3");
    system("clear");

    //Predictor's turn
    int verify;
    cout<<"Predictor!"<<endl;
    if(Players[findplayer(Players,"Predictor",numPlayers)].get_state()!=0){
        decide4:
        cout<<"Who do you want to verify?";
        cin>>verify;
        if(verify<1 || verify>numPlayers){
            cout<<"Not a valid player!"<<endl;
            goto decide4;
        }
        string id=Players[verify-1].get_identity();
        if(id=="Wolverine"){
            cout<<"Player "<<verify<<" is bad."<<endl;
        }else{
            cout<<"Player "<<verify<<" is good."<<endl;
        }   
    }else{
        verify=-1;
        cout<<"Who do you want to verify?"<<endl;
        system("sleep 2");
        cout<<"You don't seem to have a choice, as you are dead."<<endl;
    }
    cout<<"Close your eyes..."<<endl;
    cin.ignore(10000,'\n');
      
    lognight(Players,killed1,killed2,guard,verify,day);
    system("sleep 3");
    system("clear");
    for(int i=0;i<numPlayers;i++){
        if(Players[i].get_state()==2){
            Players[i].set_life(0);
        }
    }
}

void startDay(Player Players[],int result1,int result2,int hunter,int numPlayers){
    //Result announcement
    if(killed1<0){
        killed1=killed2;
        killed2=0;
    }
    if(killed1==0){
        cout<<"No one killed last night."<<endl;
    }else{
        if(killed2==0){
            cout<<"Player "<<killed1<<" dead!"<<endl;
        }else{
            if(killed1>killed2){
                int temp;
                temp=killed2;
                killed2=killed1;
                killed1=temp;
            }
            cout<<"Player "<<killed1<<" and "<<killed2<<" dead!"<<endl;
        }
    }

    //Check if the game should end (all wolverines/villagers/powers dead)
    if(checkover(Players,numWolverines,numVillagers,numPowers)==true)
        return;
    
    if(hunter==-1){
        goto nohunter;
    }
    //Open fire if hunter dies
    if(Players[hunter].get_state()==0 && !hunterfired){
        hunterfired=true;
        int target;
        cout<<"Hunter, open fire!"<<endl;
        hunterfire:
        cout<<"Target (0 for abandon):";
        cin>>target;
        if(target<0 || target>numPlayers){
            cout<<"Not a valid player!"<<endl;
            goto hunterfire;
        }
        if(Players[target-1].get_state()==0){
            cout<<"Already dead!"<<endl;
            goto hunterfire;
        }
        if(target==0){
            cout<<"Abandoned."<<endl;
        }else{
            cout<<"Player "<<target<<" killed."<<endl;
            Players[target-1].set_life(0);
            loghunter(Players,target);
        }
        if(checkover(Players,numWolverines,numVillagers,numPowers)==true)
            return;
    }

    nohunter:
    //Check who should speak first
    int j=0;
    for(j=0;j<numPlayers;j++){
        if(Players[j].get_state()==1)
            break;
    }
    cout<<"Please speak, starting from Player "<<j+1<<endl;

    //End of the day
    int playerChosen;
    decide4:
    cout<<"When finished speaking and voting, input the number of the Player out (0 for peace):";
    cin>>playerChosen;
    if(playerChosen<0 || playerChosen>numPlayers){
        cout<<"Not a valid player!"<<endl;
        goto decide4;
    }
    if(Players[playerChosen-1].get_state()==0){
        cout<<"Already dead!"<<endl;
        goto decide4;
    }
    if(playerChosen==0){
        cout<<"Peace, are you sure?";
        cin>>choice;
        if(choice!='Y' && choice!='y')
            goto decide4;
        cout<<"Ok, peace today."<<endl;
    }else{
        cout<<"Player "<<playerChosen<<", is that right?";
        cin>>choice;
        if(choice!='Y' && choice!='y')
            goto decide4;
        Players[playerChosen-1].set_life(0);
        cout<<"Ok, Player "<<playerChosen<<" voted out."<<endl;
    }

    if(Players[playerChosen-1].get_identity()=="Hunter" && !hunterfired){
        hunterfired=true;
        int target;
        cout<<"Hunter, open fire!"<<endl;
        hunterfire2:
        cout<<"Target (0 for abandon):";
        cin>>target;
        if(target<0 || target>numPlayers){
            cout<<"Not a valid player!"<<endl;
            goto hunterfire2;
        }
        if(Players[target-1].get_state()==0){
            cout<<"Already dead!"<<endl;
            goto hunterfire2;
        }
        if(target==0){
            cout<<"Abandoned."<<endl;
        }else{
            cout<<"Player "<<target<<" killed."<<endl;
            Players[target-1].set_life(0);
            loghunter(Players,target);
        }
    }
     
    logday(Players,playerChosen,day);
    if(checkover(Players,numWolverines,numVillagers,numPowers)==true)
        return;
    killed1=0;killed2=0;
}

bool checkover(Player Players[],int numWolverines,int numVillagers,int numPowers){
    int wolverineLeft=numWolverines,villagerLeft=numVillagers,powerLeft=numPowers;
    for(int i=0;i<numWolverines+numVillagers+numPowers;i++){
        if(Players[i].get_state()==0){
            if(Players[i].get_identity()=="Wolverine"){
                wolverineLeft-=1;
            }else if(Players[i].get_identity()=="Villager"){
                villagerLeft-=1;
            }else{
                powerLeft-=1;
            }
        }
    }
    if(wolverineLeft*villagerLeft*powerLeft==0){
        if(wolverineLeft==0){
            cout<<"All wolverines dead. Game over."<<endl;
            logover(1);
        }
        if(villagerLeft==0){
            cout<<"All villagers dead. Game over."<<endl;
            logover(2);
        }
        if(powerLeft==0){
            cout<<"All powers dead. Game over."<<endl;
            logover(3);
        }
        over=true;
        cout<<"Please check LRS.log for complete record of this game."<<endl;
        return true;
    }else{
        return false;
    }
}

int findplayer(Player Players[],string identity,int numPlayers){
    for(int i=0;i<numPlayers;i++){
        if(Players[i].get_identity()==identity){
            return i;
        }
    }
    return -1;
}
