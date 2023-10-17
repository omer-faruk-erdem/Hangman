#ifndef PLAYER_H
#define PLAYER_H
#include <stdio.h>
#include <string.h>
class Player{
    public :
    Player(std::string & name) ; 
    const bool isDead() const ; 
    int getHealth() const ;
    void decreaseHealth()  ;    
    std::string getName() ; 
    private :
    std::string name = "" ;
    int maxHealth {4} ; 
    int health = maxHealth ; 
    bool dead = false ;
};
#endif