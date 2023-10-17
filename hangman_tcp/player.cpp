#include "player.h"
#include <stdio.h>
#include <string>


Player :: Player(std::string & playername ){
    name = playername ; 
}

const bool Player :: isDead() const {
    return dead ; 
}

int Player :: getHealth() const {
    return health ;
}


void Player :: decreaseHealth() {
    health-=1 ; 
    if( health == 0 ) dead = true ; 
}

std::string Player :: getName(){
    return name ;
}