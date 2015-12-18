/*
 * Game Theory - NaTuRe Team - Blackjack Strategies
 *  Ma_Na_ssés Ferreira
 *     Tú_lio Loures
 * And_Ré Harder
 */
#include <stdlib.h> //exit
#include <cmath> //pow
#include <iostream>
using namespace std;
#define MAXcardsToBust 1000
#define MAXsum 21

int N=10; //types of cards 1 to 10. J=Q=K=10. A=1. A as 11 is considered depending on hand.
int cards[MAXcardsToBust]; //todo: dynamic allocation? //to keep track which cards already appeared
int handSum; //sum of the cards received at beginning before player acts

/*
 * input: p, s
 * output: print the sequence of cards from 0 position to p position.
 */
void print_hand(int *p, int s){
  //cout << "    print_hand(*p= " << *p << ", s=" << s << ")" << endl;
  for(int i=1; i<handSum; i++){  //for generate all pairs that sums handSum
    //if((i<=10) && (handSum-i<=10) && (i<=handSum-i)) { // sum=5 ---> (1,4), (2,3)
    if((i<=10) && (handSum-i<=10) ) {                    // sum=5 ---> (1,4), (2,3), (3,2), (4,1)
      if(cards[0]!=0){
        cout << handSum << " ";
        cout << i << " " << handSum-i << " ";
        int c10=0;
              if(i==10) c10++;
              if(handSum-i==10) c10++;
        int c=0;
        while(cards[c]!=0){
              if(cards[c]==10) c10++;
          cout << cards[c] << " ";
          c++;
        }
        cout << endl;

            if(c10>0){
              //cout << "ten " << c10 << endl;
              for(int j=0;j<pow(4,c10)-1;j++){
                cout << handSum << " ";
                cout << i << " " << handSum-i << " ";
                int c=0;
                while(cards[c]!=0){
                  cout << cards[c] << " ";
                  c++;
                }
                cout << endl;
              }
            }
      }
    }
  }
//  for(int i=1; i<handSum; i++){
    //if((i<=10) && (handSum-i<=10) && (i<=handSum-i)) { // sum=5 ---> (1,4), (2,3)
//    if((i<=10) && (handSum-i<=10) ) {                    // sum=5 ---> (1,4), (2,3), (3,2), (4,1)
//      int hitSum=0;
//      cout << handSum << " ";
//      cout << i << " " << handSum-i << " ";

//      for (int c = 0; c < *p; ++c) {
//        cout << cards[c] << " ";
//        hitSum = hitSum + cards[c];
//      }
//      if(*p > 0) cout << endl; //comment this line and uncomment one from those below to print others info.
      //if(p > 0) cout << "hitSum=" << hitSum << " handSum=" << handSum << " busted-with=" << s << endl;
      //if(p > 0) cout << " busted-with=" << handSum + hitSum << " before-busted=" << handSum + hitSum - cards[p-1] << " last-card=" << cards[p-1] << endl;
//    }
//  }
}

/*
 * input: i, s, p
 * output: recursive dealer, generate all the combinations of cards
 *  for a given initial sum s and card i. stops when the sum exceeds 21
 */
void infty_dealer(int i, int s, int *p)
{
  //cout << " infty_dealer(i= " << i << ", s=" << s << ", *p= " << *p << ")" << endl;
  s = s + i;
  cards[(*p)++]=i;

  if (s < MAXsum) {
    //cout << "  s < " << MAXsum << endl;
    int sj = s;
    for (int j = 1; j <= N; ++j)
    {
      sj = sj + j;
      cards[(*p)++]=j;

      if (sj < MAXsum) {
        //cout << "   sj < " << MAXsum << endl;
        infty_dealer(j,sj,p);
        //cout << endl;
      } else {
        //cout << "   sj > " << MAXsum << endl;
        print_hand(p,sj);
      }
      sj = sj - j;
      cards[(*p)--]=0;
    }

  } else {
    print_hand(p,s);
  }
  s = s - i;
  cards[(*p)--]=0;
}

/*
 * input: MINhandSum, MAXhandSum
 * output: all possible cards sequences until busted (i.e. reach 21 total sum) for an initial handSum.
 *    FORMAT:
 *      MINhandSum <sequence>
 *      handSum <sequence>
 *      ...
 *      ...
 *      MAXhandSum <sequence>
 */
void generate_just_busted_hands(int MINhandSum, int MAXhandSum){
  if(MINhandSum<=1) {
    cout << "Error: MINhandSum should be greater than 1" << endl;
    exit(1);
  }
  int p; //auxiliary pointer to track in which node we are => which position of cards array
  for (int i = 1; i <= MAXcardsToBust; ++i) {
    cards[i]=0;
  }

  for (handSum = MINhandSum; handSum <= MAXhandSum; ++handSum) {
    //cout << "handSum " << handSum << endl;
    for (int i = 1; i <= N; ++i) {
      p=0;
      infty_dealer(i,handSum,&p);
    }
  }
}

/*
 * g++ generate_just_busted_hands.cpp -o generate_just_busted_hands
 * ./generate_just_busted_hands > just_busted_hands
 */
main()
{
  generate_just_busted_hands(2,MAXsum-1);
  return 0;
}
