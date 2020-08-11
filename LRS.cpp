/* LRS.cpp
By Linus Xu (linus-xuzixuan on GitHub)
Note: "Power" here means “神” in the Chinese game, as the collection of Predictor (预言家), Witch (女巫), Hunter (猎人), and Guard (守卫).
The function for a cop (警长) is currently not implemented (as of time of submission of this line), but will be developed later on.
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
#include <vector>
#include <algorithm>
#include <random>
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
int killed1=0,killed2=0;
int day=0;
int playersleft[3];

void setIdentity(int Wolverines,int Villagers,int Power,Player Players[]);
void startNight(Player Players[],int numPlayers);
void startDay(Player Players[],int result1,int result2,int hunter,int numPlayers);
bool checkover(Player Players[],int numWolverines,int numVillagers,int numPowers);
int findplayer(Player Players[],string identity,int numPlayers);
void hunterfire(Player Players[]);

int main(int argc, char* argv[]){
    //Get numPlayers from CLI parameters if possible, or request an input
    if(argc < 2 || atol(argv[1])==-1){
        cout << "Please enter the number of players:";
        scanf("%d", &numPlayers);
    }else{
        numPlayers = atol(argv[1]);
        cout<<"Starting the game with "<<numPlayers<<" players."<<endl;
    }

    if (numPlayers<6 || numPlayers>12){
        cout<<"Not a valid player number."<<endl;
        return 1;
    }

    openlog();

    Player Players[numPlayers];

    //Read Distrib.lrs file for player information
    fin.open("./Distrib.lrs");
    if(fin.fail()){
        cout<<"Error: Distribution file not found."<<endl;
        return 2;
    }

    for(int i=1;i<numPlayers;i++)
        fin.ignore(100,'\n');
    int comp;
    fin>>comp;
    if(comp!=numPlayers){
        cout<<"Error: Distribution file corrupt."<<endl;
        return 3;
    }

    system("sleep 3");

    //Read the required line for number of each identity
    system("clear");
    fin>>numWolverines;
    cout<<"Wolverine count:"<<numWolverines<<endl;
    fin>>numVillagers;
    cout<<"Villager count:"<<numVillagers<<endl;
    numPowers=numPlayers-numWolverines-numVillagers;
    playersleft[0]=numWolverines; playersleft[1]=numVillagers; playersleft[2]=numPowers;

    char Power[numPowers];
    for(int i=0;i<numPowers;i++){
        for(fin>>Power[i];Power[0]!='\n';fin.get(Power[0])){
            if(Power[i]==' '){
                continue;
            }else if(Power[i]=='W'){
                cout<<"1 witch"<<endl;
            }else if(Power[i]=='G'){
                cout<<"1 guard"<<endl;
            }else if(Power[i]=='P'){
                cout<<"1 predictor"<<endl;
            }else if(Power[i]=='H'){
                cout<<"1 hunter"<<endl;
            }else{
                cout<<"Error: unrecognized identity in distrib.lrs (not W, G, P or H)"<<endl;
                return 4;
            }
        }
    }

    fin.close();
    system("sleep 2");
    setIdentity(numWolverines,numVillagers,numPowers,Players);
    system("clear");

    cout<<"To 'God': Please check the log file for identities."<<endl;
    system("sleep 5");

    //Iteration goes on until game over
    while(!over){
        day+=1;
        startNight(Players,numPlayers);
        startDay(Players,killed1,killed2,hunter,numPlayers);
    }
    
    endlog();
    return 0;
}

void setIdentity(int Wolverines,int Villagers,int Power,Player Players[]){
    vector<int> Player;
    for (int i = 0; i < Wolverines + Villagers + Power; i++){
        Player.push_back(i);
    }
    shuffle(Player.begin(), Player.end(), default_random_engine(uint(time(0))));

    for (int i = 0; i < Wolverines; i++){
        Players[Player[i]].set_id(1);
    }
    for (int j = Wolverines; j < Wolverines+Villagers; j++){
        Players[Player[j]].set_id(2);
    }
    Players[Player[Wolverines + Villagers]].set_id(3); //Witch
    Players[Player[Wolverines + Villagers + 1]].set_id(4); //Predictor
    if (Wolverines + Villagers + Power >= 9){
        hunter = Player[Wolverines + Villagers + 2];
        Players[Player[Wolverines + Villagers + 2]].set_id(5); //Hunter
    }
    if (Wolverines + Villagers + Power == 12){
        Players[Player[11]].set_id(6); //Guard
    }
    logid(Wolverines,Power,Villagers,Players);
}

void startNight(Player Players[],int numPlayers){
    fout<<"Night "<<day<<endl;
    clear_guard(Players,numPlayers);
    int playerchosen;
    cout<<"Close your eyes please..."<<endl;
    system("sleep 3");
    system("clear");

    //Guard's turn
    int guard=-1;
    if(numPlayers==12){
        if(Players[findplayer(Players,"Guard",numPlayers)].get_state()!=0){
            cout<<"Guard!"<<endl;
            while(true){
                cout<<"Who do you want to protect tonight (0 to abandon)?";
                cin>>playerchosen;
                cin.ignore(100,'\n');
                if(playerchosen==0){
                    cout<<"Abandoned."<<endl;
                    break;
                }else{
                    if(playerchosen<0 || playerchosen>numPlayers){
                        cout<<"Not a valid player!"<<endl;
                        continue;
                    }
                    if(Players[playerchosen-1].get_state()==0){
                        cout<<"Already dead!"<<endl;
                        continue;
                    }
                    if(Players[playerchosen-1].unguard==true){
                        cout<<"You have protected this player last night."<<endl;
                        continue;
                    }
                    cout<<"Player "<<playerchosen<<", is that right?";
                    scanf("%s", &choice);
                    if(choice != 'Y' && choice != 'y')continue;
                    Players[playerchosen - 1].guard();
                    guard = playerchosen;
                    cout << "Ok, he (she?) is likely to be safe tonight." << endl;
                    break;
                }
            }
        }else{
            cout<<"Guard!"<<endl;
            cout<<"Who do you want to protect tonight (0 for abandon)?"<<endl;
            system("sleep 1");
            cout<<"You don't seem to have a choice, as you are dead."<<endl;
        }
        cout<<"Close your eyes..."<<endl;
        system("sleep 3");
        system("clear");
    }

    //Wolverine's turn
    cout<<"Wolverines!"<<endl;
    while(true){
        cout<<"Who do you want to kill?"<<endl;
        scanf("%d",&playerchosen);

        if(playerchosen<1 || playerchosen>numPlayers){
            cout<<"Not a valid player!"<<endl;
            continue;
        }
        if(Players[playerchosen-1].get_state()==0){
            cout<<"Already dead!"<<endl;
            continue;
        }

        cout<<"Player "<<playerchosen<<", is that right?";
        scanf("%s",&choice);
        if(choice == 'Y' || choice == 'y')break;
    }

    if(Players[playerchosen-1].get_shield()==0){ //Not guarded
        Players[playerchosen-1].set_life(2);killed1=playerchosen;
    }else{ //Guarded
        killed1=1000+playerchosen;
    }

    cout<<"Ok, Player "<<playerchosen<<" dead, or at least it seems..."<<endl;
    cout<<"Close your eyes..."<<endl;
    system("sleep 3");
    system("clear");

    //Witch's turn
    bool rescue=0;
    cout<<"Witch!"<<endl;
    if(Players[findplayer(Players,"Witch",numPlayers)].get_state()!=0){
        while(true){
            cout<<"Player "<<playerchosen<<" dead, rescue?";
            scanf("%s", &choice);
            if(choice=='Y' || choice=='y'){
                if(saveused){
                    cout<<"Not valid. You have used your rescue."<<endl;
                }else{
                    Players[playerchosen-1].set_life(1);
                    rescue=true;saveused=1;
                    killed1 *= -1; //Implying witch's rescue.
                    cout<<"Ok, rescued."<<endl;
                    if(Players[playerchosen-1].get_shield()==true)
                        Players[playerchosen - 1].set_life(0); //Being both guarded and rescued kills the player.
                }
                break;
            }else if(choice=='N' || choice=='n'){
                cout<<"Ok, he (she?) is probably dead."<<endl;
                break;
            }else{
                cout<<"Not a valid decision!"<<endl;
                continue;
            }
        }
        system("sleep 2");
        while(true){
            cout<<"Do you want to use poison?";
            scanf("%s", &choice);
            if(choice=='Y' || choice=='y'){
                if(rescue){
                    cout<<"Not valid. You have rescued someone in this turn."<<endl;
                }else if(poisonused){
                    cout<<"Not valid. You have used your poison."<<endl;
                }else{
                    poisonused=true;
                    while(true){
                        cout<<"Who?";
                        scanf("%d", &playerchosen);
                        if(playerchosen<1 || playerchosen>numPlayers){
                            cout<<"Not a valid player!"<<endl;
                            continue;
                        }
                        if(Players[playerchosen-1].get_state()==0){
                            cout<<"Already dead!"<<endl;
                            continue;
                        }
                        cout<<"Player "<<playerchosen<<", is that right?";
                        scanf("%s", &choice);
                        if(choice!='Y' || choice!='y')break;
                    }
                    Players[playerchosen-1].set_life(0);
                    killed2=playerchosen;
                    cout<<"Ok, Player "<<playerchosen<<" poisoned."<<endl;
                }
            }else if(choice=='N' || choice=='n'){
                cout<<"Ok, nobody poisoned."<<endl;
                break;
            }else{
                cout<<"Not a valid decision!"<<endl;
                continue;
            }
            break;
        }
    }else{
        cout<<"Player ?? dead, rescue?"<<endl;
        system("sleep 1");
        cout<<"You don't seem to have a choice, as you are dead."<<endl;
        system("sleep 1");
        cout<<"Do you want to use poison?"<<endl;
        system("sleep 1");
        cout<<"Still no choice."<<endl;
    }
    cout<<"Close your eyes..."<<endl;
    system("sleep 3");
    system("clear");

    //Predictor's turn
    int verify;
    cout<<"Predictor!"<<endl;
    if(Players[findplayer(Players,"Predictor",numPlayers)].get_state()!=0){
        while(true){
            cout<<"Who do you want to verify?";
            scanf("%d",&verify);
            if(verify<1 || verify>numPlayers){
                cout<<"Not a valid player!"<<endl;
                continue;
            }
            if (Players[verify - 1].get_identity() == "Wolverine"){
                cout<<"Player "<<verify<<" is bad."<<endl;
            }else{
                cout<<"Player "<<verify<<" is good."<<endl;
            }
            break;
        }
    }else{
        verify=-1;
        cout<<"Who do you want to verify?"<<endl;
        system("sleep 1");
        cout<<"You don't seem to have a choice, as you are dead."<<endl;
    }
    cout<<"Close your eyes..."<<endl;
      
    lognight(Players,killed1,killed2,guard,verify,day);
    system("sleep 3");
    system("clear");
    for (int i = 0; i < numPlayers; i++)
        if(Players[i].get_state()==2)
            Players[i].set_life(0);
}

void startDay(Player Players[],int result1,int result2,int hunter,int numPlayers){
    fout<<"Day "<<day<<endl;

    //Result announcement
    if(killed1>1000)killed1=0;
    if(killed1<0 && Players[-killed1-1].get_state()==0)killed1*=-1; //Short circuited evaluation prevents problems for the latter evaluation if killed1 >= 0.
    if(killed1<=0 || killed1==killed2){ //Move killed2 to killed1, implying 
        killed1=killed2;
        killed2=0;
    }
    if(killed1==0){
        cout<<"No one was killed last night."<<endl;
    }else{
        if(killed2==0){
            cout<<"Player "<<killed1<<" is dead!"<<endl;
        }else{
            if(killed1>killed2){ //Switch values so that killed1<killed2, to prevent disclosure of cause of death (Wolverine or Witch) by the order.
                int temp;
                temp=killed2;killed2=killed1;killed1=temp;
            }
            cout<<"Player "<<killed1<<" and "<<killed2<<" are dead!"<<endl;
        }
    }

    //Check if the game should end (all wolverines/villagers/powers dead)
    if(checkover(Players,numWolverines,numVillagers,numPowers)==true)return;
    else{cout<<"The game continues."<<endl;}

    //Open fire if hunter dies
    if(hunter!=-1)
    {
        if (Players[hunter].get_state() == 0 && !hunterfired)
        {
            hunterfired = true;
            hunterfire(Players);
            if (checkover(Players, numWolverines, numVillagers, numPowers) == true)
                return;
            else
            {
                cout << "The game continues." << endl;
            }
        }
    }

    //Check who should speak first
    if(killed1!=0){
        //Next to the first dead person (killed1), if there is one
        cout<< "Player " << killed1 << " will decide whether speaking will start from his/her left or right." << endl;
    }else{
        //Find the first remaining player otherwise
        int speak=0;
        for(int i = 0; i < numPlayers; i++){
            if(Players[i].get_state()==1){
                speak=i; break;
            }
        }
        cout << "Player " << speak + 1 << " will speak and then decide whether to continue from his/her left or right." << endl;
    }
    system("sleep 2");

    //End of the day (no pun intended)
    int playerChosen;
    while(true){
        cout<<"When finished speaking and voting, input the number of the player voted out (0 for peace):";
        scanf("%d",&playerChosen);
        if(playerChosen < 0 || playerChosen > numPlayers){
            cout<<"Not a valid player!"<<endl;
            continue;
        }
        if(Players[playerChosen-1].get_state()==0){
            cout<<"Already dead!"<<endl;
            continue;
        }
        if(playerChosen==0){
            cout<<"Peace, are you sure?";
            scanf("%s", &choice);
            if(choice!='Y' && choice!='y')continue;
            cout<<"Ok, peace today."<<endl;
        }else{
            cout<<"Player "<<playerChosen<<", is that right?";
            scanf("%s", &choice);
            if(choice!='Y' && choice!='y')continue;
            Players[playerChosen-1].set_life(0);
            cout<<"Ok, Player "<<playerChosen<<" voted out."<<endl;
        }
        break;
    }

    if(Players[playerChosen-1].get_identity()=="Hunter" && !hunterfired){
        hunterfired=true;
        hunterfire(Players);
    }
     
    logday(Players,playerChosen,day);
    if(checkover(Players,numWolverines,numVillagers,numPowers)==true)return;
    else{cout << "The game continues." << endl;}
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
            }else{powerLeft-=1;}
        }
    }
    if(wolverineLeft * villagerLeft * powerLeft==0){
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
        return true;
    }else{return false;}
}

int findplayer(Player Players[],string identity,int numPlayers){
    for(int i = 0; i < numPlayers; i++)
        if(Players[i].get_identity()==identity)return i;
    return -1;
}

void hunterfire(Player Players[]){
    int target;
    cout << "Hunter, open fire!" << endl;
    while (true)
    {
        cout << "Target (0 for abandon):";
        cin>>target;
        if (target < 0 || target > numPlayers)
        {
            cout << "Not a valid player!" << endl;
            continue;
        }
        if (Players[target - 1].get_state() == 0)
        {
            cout << "Already dead!" << endl;
            continue;
        }
        if (target == 0)cout << "Abandoned." << endl;
        else
        {
            cout << "Player " << target << " killed." << endl;
            Players[target - 1].set_life(0);
            loghunter(Players, target);
        }
        break;
    }
}