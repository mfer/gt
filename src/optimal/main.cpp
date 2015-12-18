/*
 * Game Theory - NaTuRe Team - Blackjack Strategies - hAs
 *  Ma_Na_ssés Ferreira
 *     Tú_lio Loures
 * And_Ré Harder
 */
#include <stdlib.h> //exit
#include <iostream>
using namespace std;
#include <list>
//typedef list<int> IntegerList;
//IntegerList::const_iterator ci;

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
  //cartas.push_back(2);
  //cartas.push_back(3);
  //ci = cartas.begin();

  for(int c=0; c<CARTAS; c++) {
    cartas[c]=0;
  }
  cartas[0]=6;
  cartas[1]=1;
  cartas[2]=1;
  cartas[3]=1;
  cartas[4]=1;
  cartas[5]=1;
  cartas[6]=1;
  cartas[7]=1;
  cartas[8]=1;
  cartas[9]=1;
  cartas[10]=1;
  cartas[11]=1;
  cartas[12]=1;
  cartas[13]=1;
  cartas[14]=1;
  cartas[15]=1;
  cartas[16]=1;
  cartas[17]=1;
  cartas[18]=1;
  cartas[19]=1;
  cartas[20]=1;
  cartas[21]=1;

  int ci = 0;

  int i=16;//soma das duas primeiras cartas do dealer
  bool hAs=false;
  dealer(i, hAs, &ci);

  for(int s=0; s<SUMS; s++) {
    cout << s << "\t" << contFinalDealer[s] << endl;
  }
}
