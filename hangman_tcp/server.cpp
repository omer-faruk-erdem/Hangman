#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>
#include <iostream>


#include <chrono>
#include <thread>
#include <time.h>


// Game headers
#include "game.h"
using namespace std;
// listen the client 
// get client name
// Keep its name
// Serve the game

int main(){
    // Create server socket
    int server_socket ;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr=INADDR_ANY;

    // Bınd the socket to our specified IP and PORT
    
    bool binded = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
   
    // !!! When binded is true it means there is failure dont know why check later...
    if(binded){
        std::cout<<"Server failed to bind the address!!!"<<std::endl;
    }
    else{
        std::cout<<"Server listening.."<<std::endl;

        listen(server_socket, 10000 ) ;

        int client_socket ;
        client_socket = accept(server_socket, NULL ,NULL );

        char client_name[256] ;
        recv(client_socket, &client_name, sizeof(client_name),0 );

        std::cout<<"Clients name is  : "<<client_name<<std::endl;
        // Got the client name then play the game

        Game* newgame = new Game() ; 
        std::string fileName = "words.txt" ;
        newgame->takeWords(fileName);
        newgame->addPlayer(client_name) ;

        //  newgame->playGame() ;
        //  INSTEAD
        //  Instead of just calling the function I have wrote down the function here explicitly to
        //  send the game info at every round to the client and receive guess from it
        newgame->pickWord() ;

        int wordSize = newgame->currentWord.size() ; 

        newgame->displayedWord = newgame->currentWord ; 
        // Make displayed word same length with currentWord but all chars replaced by '_'
        for(unsigned int i = 0 ; i < newgame->currentWord.size() ; i++){
            newgame->displayedWord[i] = '_'; 
        }

        
        newgame->current_status = CONTINUE ;
        while( newgame->isEnded() == false ){
            char word[256] = {};
            for(int i=0 ; i<newgame->displayedWord.size() ; i++){
                word[i] = newgame->displayedWord[i] ; 
            }
            
            int health = newgame->player->getHealth() ; 
            int score = newgame->numberOfKnownWords ; ;
            status cur_status = newgame->current_status;
            
            cout<<"DATA SENT FROM THE SERVER\n";
            cout<<word<<std::endl;
            cout<<health<<std::endl;
            cout<<score<<std::endl;
            cout<<cur_status<<std::endl;

            send(client_socket, &word, sizeof(word) , 0);
            send(client_socket, &health , sizeof(health) , 0);
            send(client_socket, &score, sizeof(score) , 0);
            send(client_socket, &cur_status, sizeof(cur_status) , 0);


            // Get the clients guess
            recv(client_socket, &newgame->guess, sizeof(newgame->guess), 0 );
            
            // Check if guessed char in the word
            bool charFound = false ;
            for(unsigned int i = 0 ; i < newgame->currentWord.size() ; i++){
                if( newgame->currentWord[i] == newgame->guess && newgame->displayedWord[i] =='_' ){
                    charFound = true ; 
                    wordSize -- ;
                    newgame->displayedWord[i] = newgame->guess ;
                }
            }
            
            // If guess isn't in the word then decrease the health
            if(!charFound){
                newgame->player->decreaseHealth();
            }
            
            // WIN OR LOSE DECISION

            // Case : Lose
            if( newgame->player->getHealth() == -1 ){
                newgame->current_status = ENDED ;
                send(client_socket, &newgame->current_status, sizeof(newgame->current_status) , 0);
                std::this_thread::sleep_for(std::chrono::seconds(3));
                continue ;
            }

            // Case : Win
            // if all characters found in the word then move to next word
            if( wordSize == 0 ){
                newgame->current_status = WIN ; 
                newgame->printPlayerStatus() ;
                newgame->draw() ;
                newgame->numberOfKnownWords++;

                // Case : No words left to be guessed 
                if(!newgame->pickWord()){
                    newgame->current_status = ENDED ; 
                    send(client_socket, &newgame->current_status, sizeof(newgame->current_status) , 0);
                    continue ;
                }
                // Case : There are words to be guessed
                else{
                    newgame->displayedWord = "" ;
                    std::string name = newgame->player->getName() ; 
                    delete(newgame->player) ; 
                    newgame->player = new Player(name) ; 
                    
                    if( newgame->pickWord() == false ){
                        newgame->current_status = ENDED ;
                        send(client_socket, &newgame->current_status, sizeof(newgame->current_status) , 0);
                        continue ;
                    }

                    wordSize = newgame->currentWord.size() ; 
                    newgame->displayedWord = newgame->currentWord ; 
                    
                    // Make displayed word same length with currentWord but all chars replaced by '_'
                    
                    cout<<"displayed word after making it empty -"<<newgame->displayedWord<<"-"<<endl; // BUGFIX DISPLAY--> DELETE LATER
                    cout<<"current word is "<<newgame->currentWord<<endl;

                    int n = newgame->currentWord.size() ;
                    string buffer;
                    for(int i = 0 ; i < n ; i++){
                        buffer += '_'; 
                    }
                    newgame->displayedWord = buffer ; 
                    cout<<"Buffer is : "<<buffer<<endl;

                    send(client_socket, &newgame->current_status, sizeof(newgame->current_status) , 0);
                    continue ; 
                }
            }
            newgame->current_status = CONTINUE ; 
            send(client_socket, &newgame->current_status, sizeof(newgame->current_status) , 0);
        }
        delete(newgame) ;
    }
    // close(server_socket);
    shutdown(server_socket, SHUT_RDWR);
    
    return 0 ;
}
