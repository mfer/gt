/*
 * Game Theory - NaTuRe Team - Blackjack Strategies
 *  Ma_Na_ssés Ferreira
 *     Tú_lio Loures
 * And_Ré Harder
 */

#include <iostream>
using namespace std;
#define MAXcardsToBust 1000
#define MAXsum 21

int N=11; //types of cards 1 to 11. J=Q=K=10. A=1 or 11.
int cards[MAXcardsToBust]; //todo: dynamic allocation? //to keep track which cards already appeared
int handSum; //sum of the cards received at beginning before player acts

/*
 * input: p, s
 * output: print the sequence of cards from 0 position to p position.
 */
void print_hand(int p, int s){
  int hitSum=0;
  cout << handSum << " ";
  for (int c = 0; c < p; ++c) {
    cout << cards[c] << " ";
    hitSum = hitSum + cards[c];
  }
  if(p > 0) cout << endl; //comment this line and uncomment one from those below to print others info.
  //if(p > 0) cout << "hitSum=" << hitSum << " handSum=" << handSum << " busted-with=" << s << endl;
  //if(p > 0) cout << " busted-with=" << handSum + hitSum << " before-busted=" << handSum + hitSum - cards[p-1] << " last-card=" << cards[p-1] << endl;
}

/*
 * input: i, s, p
 * output: recursive dealer, generate all the combinations of cards
 *  for a given initial sum s and card i. stops when the sum exceeds 21
 */
void infty_dealer(int i, int s, int p)
{
  s = s + i;
  cards[p++]=i;

  if (s < MAXsum) {
    int sj = s;
    for (int j = 1; j <= N; ++j)
    {
      sj = sj + j;
      cards[p++]=j;

      if (sj < MAXsum) {
        infty_dealer(j,sj,p);
      } else {
        print_hand(p,sj);
        sj = sj - j;
        cards[p--]=0;
      }
    }
  } else {
    print_hand(p,s);
    s = s - i;
    cards[p--]=0;
  }
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
void generate_unbusted_hands(int MINhandSum, int MAXhandSum){
  int p; //auxiliary pointer to track in which node we are => which position of cards array
  for (int i = 1; i <= MAXcardsToBust; ++i) {
    cards[i]=0;
  }

  for (handSum = MINhandSum; handSum <= MAXhandSum; ++handSum) {
    //cout << "handSum " << handSum << endl;
    for (int i = 1; i <= N; ++i) {
      p=0;
      infty_dealer(i,handSum,p);
    }
  }
}

/*
 * g++ generate_unbusted_hands.cpp -o generate_unbusted_hands
 * ./generate_unbusted_hands > unbusted_hands
 */
main()
{
  generate_unbusted_hands(2,20);
  return 0;
}
