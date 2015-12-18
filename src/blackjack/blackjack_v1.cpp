#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

// End-of game probability histogram
struct endHist_t {
	double p[22];
	endHist_t() { memset(p, 0x00, 22 * sizeof(double)); }
	double &operator [] (size_t i) { return p[i]; }
};

// An action profile has an action, and a strategy
typedef enum { UNDEFINED, H, S } action_t;
typedef action_t strategySet_t[2][22]; // strategySet[hard/soft][player sum]
typedef strategySet_t playerStrategySet_t[11]; // playerStrategySet_t[dealerSum]

											   // Computes the win probability for a player following a fixed strategy
endHist_t strategicGamer(size_t i, bool ace, strategySet_t * strat) {
	endHist_t retval;

	// Stand / Bust
	if ((!ace && i >= 22) || (ace && i >= 21)) retval[0] = 1;
	else if ((*strat)[ace][i] == S) {
		if (!ace) retval[i] = 1;
		else if (i <= 10) retval[i + 11] = 1;
		else retval[i + 1] = 1;
	}

	// Hit
	else for (size_t c = 1; c <= 10; c++) {
		endHist_t recursedHist;

		// Received an ace
		if (c == 1) {
			if (ace) recursedHist = strategicGamer(i + 1, true, strat);
			else recursedHist = strategicGamer(i, true, strat);

			// Received any other card
		}
		else recursedHist = strategicGamer(i + c, ace, strat);

		// Merge in the recursed probability
		float probC = (float)(c == 10 ? 4 : 1) / 13;
		for (size_t i = 0; i < 22; i++)
			retval[i] += probC * recursedHist[i];
	}

	// Return the computed histogram
	return retval;
}

// Computes the probability distibution at the end of the game for the dealer, given his visible card
// By default, computes the distribution for all deques
endHist_t getDealerHist(size_t dealerStartingAmmount = 0, bool startingAce = false) {
	strategySet_t dealerStrat = {
		/* Hard */{ H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, S, S, S, S, S },
		/* Soft */{ H, H, H, H, H, H, S, S, S, S, S, H, H, H, H, H, S, S, S, S, S, S }
	};
	return strategicGamer(dealerStartingAmmount, startingAce, &dealerStrat);
}

// Computes the expected winnings given the players sum and the dealers distibution
double expectedWinnings(size_t playerSum, endHist_t * dealerHist) {
	if (playerSum > 21) return 0;
	else {
		double retval = (*dealerHist)[playerSum] / 2;
		for (size_t i = 0; i < playerSum; i++) retval += (*dealerHist)[i];
		return retval;
	}
}

// Computes how much a particular player strategy is expected to earn
double expectedStrategyWinnings(playerStrategySet_t * strat) {
	double winnings = 0;
	for (size_t c = 1; c <= 10; c++) {

		// Compute the dealer end-histogram for this card
		endHist_t dealerHist;
		if (c == 1) dealerHist = getDealerHist(0, true);
		else dealerHist = getDealerHist(c, false);

		// Compute the outcome of the strategy
		endHist_t playerHist = strategicGamer(0, false, &(*strat)[c]);

		// Compute the winnings
		float probC = (float)(c == 10 ? 4 : 1) / 13;
		for (size_t i = 1; i < 22; i++)
			winnings += probC * playerHist[i] * expectedWinnings(i, &dealerHist);
	}
	return winnings;
}



// Recursively computes the optimal strategy, given a dealer distribution, returning its expected payoff
typedef double expectedReward_t[2][22];
double optimalGamer(size_t i, bool ace, strategySet_t * strat, expectedReward_t * expReward, endHist_t * dealerHist) {
	endHist_t retval;

	// Bust
	if ((!ace && i >= 22) || (ace && i >= 21)) return 0;

	// If already computed
	if ((*strat)[ace][i] != UNDEFINED) return (*expReward)[ace][i];

	// Stand
	double rewardStand;
	if (!ace) rewardStand = expectedWinnings(i, dealerHist);
	else if (i <= 10) rewardStand = expectedWinnings(i + 11, dealerHist);
	else rewardStand = expectedWinnings(i + 1, dealerHist);

	// Hit
	double rewardHit = 0;
	for (size_t c = 1; c <= 10; c++) {
		double recReward;

		// Received an ace
		if (c == 1) {
			if (ace) recReward = optimalGamer(i + 1, true, strat, expReward, dealerHist);
			else recReward = optimalGamer(i, true, strat, expReward, dealerHist);
		
		// Received any other card
		} else recReward = optimalGamer(i + c, ace, strat, expReward, dealerHist);

		// Merge in the recursed probability
		float probC = (float)(c == 10 ? 4 : 1) / 13;
		rewardHit += probC * recReward;
	}

	// Make SPE choice
	action_t action;
	double reward;
	if (rewardStand > rewardHit) {
		action = S;
		reward = rewardStand;
	} else {
		action = H;
		reward = rewardHit;
	}

	// Memorize results and return
	(*strat)[ace][i] = action;
	(*expReward)[ace][i] = reward;
	return reward;
}

// Computes the optimal strategy for each dealer card
playerStrategySet_t * computeOptimalStrategy() {
	action_t * aux = (action_t*)calloc(11 * 2 * 22, sizeof(action_t));
	playerStrategySet_t * optimalStrat = (playerStrategySet_t*)aux;

	for (size_t c = 1; c <= 10; c++) {

		// Compute the dealer end-histogram for this card
		endHist_t dealerHist;
		if (c == 1) dealerHist = getDealerHist(0, true);
		else dealerHist = getDealerHist(c, false);

		// Compute the optimal strategy for this visible card
		expectedReward_t expReward; memset(expReward, 0x00, sizeof(expectedReward_t));
		optimalGamer(0, false, &((*optimalStrat)[c]), &expReward, &dealerHist);
	}

	return optimalStrat;
}
int main(void){

	// Compute the overall dealer distribution
	printf("Dealer final card histogram: \n");
	endHist_t dealerHist = getDealerHist();
	for (size_t i = 0; i < 22; i++) printf("%.3lf ", dealerHist[i]);
	printf("\n");
	
	// Compute the optimal strategy
	printf("Optimal strategy: \n");
	playerStrategySet_t * strat = computeOptimalStrategy();
	for (size_t j = 0; j < 2; j++) {
		printf("%s Hand:\n", j == 0 ? "Hard" : "Soft");
		for (size_t k = (j == 0 ? 4 : 1); k < 22; k++) {
			printf("player sum %02zu: ", k);
			for (size_t i = 1; i <= 10; i++) {
				printf("%c ", (*strat)[i][j][k] == H ? 'H' : 'S');
			}
			printf("\n");
		}
	}	
	free(strat);
	
	// Compute the expected rewards for the player
	printf("SPE strategy has expected winnings of %lf for ever 1 unit spent\n", expectedStrategyWinnings(strat));
}

			