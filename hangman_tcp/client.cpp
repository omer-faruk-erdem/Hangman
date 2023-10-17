// Client application

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>
#include  "player.h"
#include <string>

#include <chrono>
#include <thread>
#include <time.h>
#include "game.h"
using namespace std;

void drawRope(); 
void drawHead();
void drawArms();
void drawBody();
void drawFoot();
void printPlayerStatus(string displayedWord, int health , int numberOfKnownWords);
void draw(int) ; 

int main(){

    // create a socket
    int network_socket  ; 
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // specify an address for the socket
    struct sockaddr_in server_address;   
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY ; 

    // called the "connect" function and assigned the return value in connection_status variable so we can check if we could connected succesfully.
    // connected socket to "server_address"
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address , sizeof(server_address));

    if( connection_status == -1){
        printf(" There was an error making conneciton to the remote socket \n");
    }
    else{
        // send data to server
        std::cout<<"Enter your name: "<<std::endl;
        
        char client_name[256]; 
        std::cin>>client_name ; 
        send(network_socket, &client_name, sizeof(client_name),0 );

        // Game will be played on the server but game status and other printed info will be get from server

        status current_status = NOTSTARTED; 
        while(current_status!=ENDED){             
            char displayedWord[256] ;
            int health ; 
            int total_score ;

            if(recv(network_socket, &displayedWord , sizeof(displayedWord), 0) == 0 ) std::cout<<"Receive failed\n" ; 
            if(recv(network_socket, &health , sizeof(health), 0) == 0 ) std::cout<<"Receive failed\n" ; 
            if(recv(network_socket, &total_score , sizeof(total_score), 0) == 0) std::cout<<"Receive failed\n" ; 
            if(recv(network_socket, &current_status , sizeof(current_status), 0) == 0) std::cout<<"Receive failed\n" ; 
            
            printPlayerStatus(displayedWord,health,total_score);
            draw(health);
            
            // Send client's guess to server
            char guess ; 
            cout<<"Enter your guess : \n";
            cin>>guess;
            send(network_socket, &guess, sizeof(guess) , 0 ) ;


            // game status will be recevied by the server
            recv(network_socket, &current_status, sizeof(current_status), 0) ; 

            if( current_status == CONTINUE ){
                // print the current status and continue
                cout<<"CONTINUE"<<endl;
            }
            else if( current_status == WIN ){
                system("clear") ;
                cout<<"CORRECT!"<<endl;
                cout<<"Next word is loading..."<<endl;
                cout<<"WIN"<<endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
            else if( current_status == ENDED){
                system("clear") ;
                std::cout<<"As total you have guessed "<<total_score<<" words correctly\n";
                std::cout<<"---------------Game Ended---------------\n";
                std::cout<<"****************************************\n";
                std::this_thread::sleep_for(std::chrono::seconds(3));
                cout<<"ENDED"<<endl;
                break;
            }
        }
        

    }
    // close(network_socket); 
    shutdown(network_socket, SHUT_RDWR);
    return 0 ; 
}



void draw(int health) {   
    // according to status of the game different type of man will be drawn 
    /*
        4-> only rope
        3-> only head
        2-> add arms 
        1-> add body
        0-> add foot
        -> otherwise game is ended so print ending screen 
    */
    switch (health)
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

void drawRope(){
    std::cout<<"**************************************\n";
    std::cout<<"I-------- \n";
    std::cout<<"      |   \n";
    std::cout<<"      |   \n";
    std::cout<<"    ----- \n";
}

void drawHead(){
    std::cout<<"      O   \n";
}

void drawArms(){
    std::cout<<"     /|\\  \n"; // HERE "\" might cause a trouble
}

void drawBody(){
    std::cout<<"      |   \n";
}                     

void drawFoot(){
    std::cout<<"     / \\  \n" ; // HERE "\" might cause a trouble
}


void printPlayerStatus(string displayedWord, int health , int numberOfKnownWords){
    system("clear") ;
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout<<"Your Word : "<<displayedWord<<std::endl;
    std::cout<<"Your Health : "<<health<<std::endl;
    std::cout<<"Your Total Score : "<<numberOfKnownWords<<std::endl;
}
