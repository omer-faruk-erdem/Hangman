#ifndef GAME_H
#define GAME_H

#include "player.h"
#include <iostream>
#include <string>
#include <vector>

enum  status {NOTSTARTED = -1 , CONTINUE = 0  , ENDED = 1 } ;

class Game{
    public : 
    void draw() ; 
    void drawRope();
    void drawHead();
    void drawArms();
    void drawBody();
    void drawFoot();
    void printPlayerStatus();
    void addPlayer(std::string name) ; 
    bool isEnded(); 
    void takeInput();
    void takeWords();
    void takeWords(std::string);
    bool pickWord() ; 
    void playGame() ; 
    ~Game() ; 

    private : 
    std::string currentWord = "" ; 
    std::string displayedWord = "" ;
    std::vector<std::string> words ; 
    int numberOfKnownWords = 0 ; 
    char guess ; 
    Player* player = NULL;
    status current_status = NOTSTARTED ;
};

#endif