//written in C++
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

//function to roll, usually based on attributes
//can stack rolls for boosts maybe?
int custRoll(int amount, int upper){
    int result = 0;
    for(int i = 0; i < amount; i++){
        result += (rand() % upper) + 1;
    }
    return result;
}


class Player{
    public:
        //idk what i'm doing so i make the rules here!
        //maybe upgradable later?
        //base 20 for all, health is obvious, don't die
        //speed helps with crit chance on damage out and dodge chance(havent made dodge mechanic yet)
        //crit of 20 = 2% chance and so on
        //strength is max possible hit on roll
        int attHealth;
        int attSpeed;
        int attStrength;
        Player(){
            setAtt('h', 20);
            setAtt('p', 20);
            setAtt('s', 20);
        }
        int damOut(){
            int critMatch = custRoll(1, 100);
            int result;
            if((attSpeed / 10) >= critMatch){
                result = custRoll(1, attStrength) + attStrength;
                critOut();
            } else {
                result = custRoll(1, attStrength);
            }
            return result;
        }
        void critOut(){
            cout << "Critical hit!" << endl;
        }
        void damIn(int Amount){
            int newH = attHealth - Amount;
            setAtt('h', newH);
        }
        int heal(){
            //can only heal up to 50% of current health depending on roll
            int newH = custRoll(1, attHealth / 2);
            return newH;
        }
        void info(){
            cout << "Player -- Health: " << attHealth << "  Speed: " << attSpeed << "  Strength: " << attStrength << endl;
        }
        void setAtt(char Att, int amount){
            switch(Att){
                case 'h':
                    attHealth = amount;
                    break;
                case 'p':
                    attSpeed = amount;
                    break;
                case 's':
                    attStrength = amount;
                    break;
            }
        }
        int getAtt(char Att){
            int result;
            switch(Att){
                case 'h':
                    result = attHealth;
                    break;
                case 'p':
                    result = attSpeed;
                    break;
                case 's':
                    result = attStrength;
                    break;
            }
            return result;
        }
};

class Enemy : public Player{
    //inherits Player class  because I wanna learn!
    public:
        Enemy(){
            setAtt('h',custRoll(2,20));
            setAtt('p',custRoll(2,20));
            setAtt('s',custRoll(2,20));
            intro();
        }
        void intro(){
            cout << "An enemy approaches!" << endl;
            info();
        }
        void info(){
            //I can probably put this in the Player class and add a name so it just dumps the info that way, I should do that...
            cout << "Enemy -- Health: " << attHealth << "  Speed: " << attSpeed << "  Strength: " << attStrength << endl;
        }
};

//checks if the character is alive, this was a quick fix
bool logPlayerHealth(Player player1){
    int pH = player1.getAtt('h');
    bool result;
    if(pH <= 0){
        result = false;
    } else {
        result = true;
    }
    return result;
}
bool logEnemyHealth(Enemy enemy1){
    int eH = enemy1.getAtt('h');
    bool result;
    if(eH <= 0){
        result = false;
    } else {
        result = true;
    }
    return result;
}

void battle(Player player1, Enemy enemy1){
    //the battle was legendary!
    bool alive = true;
    string response;
    while(alive){
        //checks that everyone is not dead
        if(!logPlayerHealth(player1)){
            alive = false;
            cout << "You have been defeated!" << endl;
            break;
        } else if(!logEnemyHealth(enemy1)){
            alive = false;
            cout << "The enemy has been defeated!" << endl;
            break;
        }
    
        //you go first
        cout << endl << "What do you do? ";
        cin >> response;
        if(response == "attack"){
            int attAmt = player1.damOut();
            cout << "You attack for " << attAmt << endl;
            enemy1.damIn(attAmt);
            enemy1.info();
        } else if(response == "heal"){
            int healAmt = player1.heal();
            cout << "You heal for " << healAmt << endl;
            player1.setAtt('h', healAmt + player1.attHealth);
            player1.info();
        } else {
            cout << "Invalid";
            //for some reason this kills you
            alive = false;
        }

        Sleep(500);

        //wait and then let enemy attack or heal
        if(enemy1.getAtt('h') > 5){
            int attAmt = enemy1.damOut();
            cout << endl << "Enemy attacks for " << attAmt << endl;
            player1.damIn(attAmt);
            player1.info();
        } else if(enemy1.getAtt('h') <= 5 && enemy1.getAtt('h') > 0){
            int healAmt = enemy1.heal() * 2;
            cout << endl << "Enemy heals for " << healAmt << endl;
            enemy1.setAtt('h', healAmt + enemy1.attHealth);
            enemy1.info();
        } else{
            //is ded, dont work
        }
    }
}

//to help the rand() be more rand()
void setup(){
    srand(time(NULL));
}

//main script
int main(){
    setup();
    Player player1;
    Enemy enemy1;

    battle(player1, enemy1);

    return 0;
}
