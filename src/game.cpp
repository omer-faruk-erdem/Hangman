#include "game.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <time.h>

Game :: ~Game(){
    delete(player) ; 
}

void Game :: draw() {   
    // according to status of the game different type of man will be drawn 
    /*
        4-> only rope
        3-> only head
        2-> add arms 
        1-> add body
        0-> add foot
        -> otherwise game is ended so print ending screen 
    */
    switch (player->getHealth())
    {
    case 4 :
        drawRope();
        break;
    
    case 3 :
        drawRope();
        drawHead();
        break ;
    case 2 :
        drawRope();
        drawHead();
        drawArms();
        break ;
    case 1 : 
        drawRope();
        drawHead();
        drawArms();
        drawBody();    
        break ;
    case 0 :
        drawRope();
        drawHead();
        drawArms();
        drawBody();
        drawFoot();
        break ;
    default:
        break;
    }
}

void Game :: drawRope(){
    std::cout<<"**************************************\n";
    std::cout<<"I-------- \n";
    std::cout<<"      |   \n";
    std::cout<<"      |   \n";
    std::cout<<"    ----- \n";
}

void Game :: drawHead(){
    std::cout<<"      O   \n";
}

void Game :: drawArms(){
    std::cout<<"     /|\\  \n"; // HERE "\" might cause a trouble
}

void Game :: drawBody(){
    std::cout<<"      |   \n";
}                     

void Game :: drawFoot(){
    std::cout<<"     / \\  \n" ; // HERE "\" might cause a trouble
}


void Game :: printPlayerStatus(){
    system("clear") ;
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout<<"Your Word : "<<displayedWord<<std::endl;
    std::cout<<"Your Health : "<<player->getHealth()<<std::endl;
    std::cout<<"Your Total Score : "<<numberOfKnownWords<<std::endl;
}

void Game :: addPlayer(std::string  name) {
    if(player!=NULL){
        std::cout<<"THERE IS ALREADY A PLAYER THAT PLAYS THE GAME\n";
    }
    player = new Player(name) ; 
}

bool Game :: isEnded() {
    return current_status==ENDED ; 
}

// To take input from the player 
void Game :: takeInput(){
    std::cout<<"Enter your guess : ";
    std::cin>>guess ;
}

// Store all of the words in file to words array
// Changed the function due to error 
// Hardcoded instead of taking file name as parameter 
void Game :: takeWords(std::string file_name){
    std::string filename = file_name ;

    std::ifstream file;

    file.open(filename);

    if (file.fail())
    {
        std::cout << "Words file failed to open." << std::endl;
    }

    std::string word;
    while (!file.eof())
    {
        getline(file, word);
        words.push_back(word);
    }
    file.close();
}


/* Choses random word from words array 
 * Stores it in the currentWord variable 
 * Then deletes the curretnWord from words array
*/
bool Game :: pickWord(){
    srand(time(NULL)) ;
    int size = words.size(); 
    if(!size) return false ; 
    int randomIndex = rand()%size; 
    currentWord = words[randomIndex] ;
    words.erase(words.begin()+randomIndex) ;
    return true;
}




void Game :: playGame() {
    
    pickWord() ;

    int wordSize = currentWord.size() ; 

    displayedWord = currentWord ; 
    // Make displayed word same length with currentWord but all chars replaced by '_'
    for(unsigned int i = 0 ; i < currentWord.size() ; i++){
        displayedWord[i] = '_'; 
    }

    while( isEnded() == false ){
        printPlayerStatus();
        draw();
        takeInput();
        
        bool charFound = false ;

        // Check if guessed char in the word
        for(unsigned int i = 0 ; i < currentWord.size() ; i++){
            if( currentWord[i] == guess && displayedWord[i] =='_' ){
                charFound = true ; 
                wordSize -- ;
                displayedWord[i] = guess ;
            }
        }
        
        // If guess isn't in the word then decrease the health
        if(!charFound){
            player->decreaseHealth();
        }
        
        // Case : Lose
        if( player->getHealth() == -1 ){
            std::cout<<"You have lost the game :/ \n" ; 
            std::cout<<"As total you have guessed "<<numberOfKnownWords<<" words correctly\n";
            std::cout<<"The word to be guessed was : "<<currentWord<<std::endl ; 
            
            current_status = ENDED ;

            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue ;
        }

        // Case : Win
        // if all characters found in the word then move to next word
        if( wordSize == 0 ){
            printPlayerStatus() ;
            draw() ;
            numberOfKnownWords++;
            std::cout<<"You guessed the word correctly \n";
            // Case : No words left to be guessed 
            if(!pickWord()){
                std::cout<<"You have guessed all the words correctly\n";
                std::cout<<"As total you have guessed "<<numberOfKnownWords<<" words correctly\n";
                std::cout<<"---------------Game Ended---------------\n";
                std::cout<<"****************************************\n";
                std::this_thread::sleep_for(std::chrono::seconds(3));
                current_status = ENDED ; 
                continue ;
            }
            // Case : There are words to be guessed
            else{
                std::cout<<"Next word is loading...\n";
                displayedWord = "" ;
                std::string name = player->getName() ; 
                delete(player) ; 
                player = new Player(name) ; 
                std::this_thread::sleep_for(std::chrono::seconds(3));
                playGame() ; 
                return ;
            }
        }

        current_status = CONTINUE ; 
    }
}
