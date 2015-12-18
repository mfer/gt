/*
 * Game Theory - NaTuRe Team - Blackjack Strategies
 *  Ma_Na_ssés Ferreira
 *     Tú_lio Loures
 * And_Ré Harder
 */
#include <stdlib.h> // rand()
#include <vector>
#include <sstream>  // for istringstream
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#define SPECIAL_WORD "handSum"
int N=10;

/*
 * g++ main.cpp -o main
 * ./main
 */
main()
{
  ifstream playerFile("unbusted_hands");
  int playerHand, dealerHand, playerTotal, dealerTotal, playerPay=0, dealerPay=0, Pays=0;
  string playerCards, dealerCards;

  srand (time(NULL));

  //initialization: one for player c1, one for dealer c2, two for player c3 and two for dealer c4.
  //for(int c1=1; c1<=N; c1++){
    //for(int c2=1; c2<=N; c2++){
      //for(int c3=c1; c3<=N; c3++){
        //for(int c4=c2; c4<=N; c4++){
          while (true) {
            if( playerFile.eof() ) break;
            getline(playerFile, playerCards);
            //cout << playerCards << endl;

            istringstream s2(playerCards);
            vector<int> v;
            int tmp;
            while (s2 >> tmp) {
              v.push_back(tmp);
            }
            //player move
            playerHand=0;
            playerTotal=0;
            for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
              if(it == v.begin()) {
//                cout << "player ";
                playerHand=*it;
              }else{
//                cout << *it << " ";

                //todo: call the player Strategies Here
                if(playerHand + playerTotal < 19 ) playerTotal = playerTotal + *it;
              }
//              if(it == v.end()-1) cout << endl;
            }
            //todo: assign the playerTotal based on the strategy
            playerTotal = playerHand + playerTotal;

            if(playerHand){
              ifstream dealerFile("unbusted_hands");
              while (true) {
                if( dealerFile.eof() ) break;
                getline(dealerFile, dealerCards);
                //cout << dealerCards << endl;

                istringstream s2(dealerCards);
                vector<int> v;
                int tmp;
                while (s2 >> tmp) {
                  v.push_back(tmp);
                }
                //dealer move
                dealerHand=0;
                dealerTotal=0;
                for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
                  if(it == v.begin()) {
//                    cout << " dealer ";
                    dealerHand=*it;
                  }else{
//                    cout << *it << " ";

                      //todo: call the dealer Strategy Here: stands at 17
                      if(dealerHand + dealerTotal < 17 ) dealerTotal = dealerTotal + *it;
                  }
//                  if(it == v.end()-1) cout << endl;
                }

                //todo: assign the dealerTotal based on the strategy
                dealerTotal = dealerHand + dealerTotal;

                if(dealerHand){
                  //todo: call the jury to verify who wins: player, dealer xor none
                  //cout << "pH=" << playerHand << " dH=" << dealerHand << " ";
                  //cout << "pT=" << playerTotal << " dT=" << dealerTotal << endl;
                  Pays++;
                  if(playerTotal>21) {
                    if(dealerTotal>21) {
                      //cout << "(-1,1)" << endl;
                      playerPay--;
                      dealerPay++;
                    } else if(dealerTotal==21) {
                      //cout << "(-1,1)" << endl;
                      playerPay--;
                      dealerPay++;
                    } else if (dealerTotal<21) {
                      //cout << "(-1,1)" << endl;
                      playerPay--;
                      dealerPay++;
                    }
                  } else if(playerTotal==21) {
                    if(dealerTotal>21) {
                      //cout << "(1,-1)" << endl;
                      playerPay++;
                      dealerPay--;
                    } else if(dealerTotal==21) {
                      //cout << "(0,0)" << endl;
                    } else if (dealerTotal<21) {
                      //cout << "(1,-1)" << endl;
                      playerPay++;
                      dealerPay--;
                    }
                  } else if (playerTotal<21) {
                    if(dealerTotal>21) {
                      //cout << "(1,-1)" << endl;
                      playerPay++;
                      dealerPay--;
                    } else if(dealerTotal==21) {
                      //cout << "(-1,1)" << endl;
                      playerPay--;
                      dealerPay++;
                    } else if (dealerTotal<21) {
                      if(playerTotal > dealerTotal) {
                        //cout << "(1,-1)" << endl;
                        playerPay++;
                        dealerPay--;
                      } else if(playerTotal == dealerTotal){
                        //cout << "(0,0)" << endl;
                      } else if(playerTotal < dealerTotal){
                        //cout << "(-1,1)" << endl;
                        playerPay--;
                        dealerPay++;
                      }
                    }
                  }
                }
              }
            }
          }
        //}
      //}
    //}
  //}
  cout << "pP=" << playerPay << " dP=" << dealerPay << " Pays=" << Pays << endl;
  return 0;
}
