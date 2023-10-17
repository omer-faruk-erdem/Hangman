#include <iostream>
#include <string>
#include "game.h"

int main(){
    Game* newgame = new Game() ; 
    std::string fileName = "words.txt" ;
    newgame->takeWords(fileName);
    std::string playerName ;
    std::cout<<"Enter your name : ";
    std::cin>>playerName;
    newgame->addPlayer(playerName) ;
    newgame->playGame() ;
    delete(newgame) ;
}