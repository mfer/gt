/*
 * Game Theory - NaTuRe Team - Blackjack Strategies - hAs
 *  Ma_Na_ssés Ferreira
 *     Tú_lio Loures
 * And_Ré Harder
 */
#include <stdlib.h> //exit
#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <fstream> //ifstream
#include <sstream>  //istringstream

#define SUMS 22
#define CARTAS 100
int contFinalDealer[SUMS]; //histogram, 0 position will be used to busted cases.
int cartas[CARTAS];

int getCard(int *ci){
  //cout << "  getCard: ";
  if (*ci > CARTAS){
    cout << "\nError: trying to get more cards than the size of the pile.\
             \n tip: Increase CARTAS!" << endl;
    exit(1);
  } else if ( cartas[*ci] == 0) {
    cout << "\nError: trying to get more cards than there is in the pile.\
             \n tip: Fill cartas properly!" << endl;
    exit(2);
  } else {
    //cout << cartas[(*ci)] << endl;
    return cartas[(*ci)++];
  }
}

/* i //soma das cartas do dealer
 * hAs //se ele tem pelo menos um as
 */
void dealer(int i, bool hAs, int *ci) {
  //cout<< "dealer: i " << i << " hAs " << hAs << endl;
// Burst
  if ( (i > 21 && !hAs) || (i > 20 && hAs) ) {
    //cout << " burst" << endl;
    contFinalDealer[0]++;
// Stand
  } else if (hAs && i >= 6 && i <= 10) {
    //cout << " stand_6to10" << endl;
    contFinalDealer[i + 11]++;
  } else if (hAs && i >= 16 && i <= 20) {
    //cout << " stand_16to20" << endl;
    contFinalDealer[i + 1]++;
  } else if (!hAs && i >= 17) {
    //cout << " stand_17" << endl;
    contFinalDealer[i]++;
// Hit
  } else {
    //cout << " hit " << endl;
    int card = getCard(ci);
    //cout << "  ci " << *ci << " card " << card << endl;

    if (card == 1) { //1 means an ACE
      //cout << "  an ACE card, ";
      if (hAs) {
        //cout << "not the first, adding one" << endl;
        dealer(i + 1, true, ci);
      } else {
        //cout << "the first, not adding, setting hAs true" << endl;
        dealer(i, true, ci);
      }
    } else {
      //cout << "  not ACE card, adding" << endl;
      dealer(i + card, hAs, ci);
    }
  }
}

/*
 * g++ main.cpp && ./a.out
 */
main()
{
  for(int s=0; s<SUMS; s++) {
    contFinalDealer[s]=0;
  }

  int c;

  ifstream dealerFile("just_busted_hands"); //todo: set this name file as argument of main()
  string dealerCards;
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

    for(c=0; c<CARTAS; c++) {
      cartas[c]=0;
    }

    int ci = 0;
    int i; //soma das duas primeiras cartas do dealer
    bool hAs=false;
    c=0;
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
      if(it == v.begin()) {
        //i=*it;
        i=0;
        //cout << " dealerHand " << *it << " cards ";
      }else{
        cartas[c++]=*it;
        //cout << *it << " ";
      }
      //if(it == v.end()-1) cout << endl;
    }

    c=0;
    if(cartas[c]!=0){
    //  cout << i << " ";
    //  while(cartas[c]!=0) {
    //    cout << cartas[c] << " ";
    //    c++;
    //  }
    //  cout << endl;

      dealer(i, hAs, &ci);
    }
  }

  for(int s=0; s<SUMS; s++) {
    cout << s << "\t" << contFinalDealer[s] << endl;
  }
}
