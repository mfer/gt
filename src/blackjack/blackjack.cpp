#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

// End-of game probability histogram
struct endHistogram_t {
	double p[22];
	endHistogram_t() { memset(p, 0x00, 22 * sizeof(double)); }
	double &operator [] (size_t i) { return p[i]; }
};

// An action profile has an action, and a strategy
typedef enum { UNDEFINED, H, S } action_t;
typedef action_t strategySet_t[2][22]; // strategySet[hard/soft][player sum]
typedef strategySet_t playerStrategySet_t[11]; // playerStrategySet_t[dealerSum]

// Computes the win probability for a player following a fixed strategy
endHistogram_t getPlayerHistogram(size_t i, bool ace, strategySet_t * strategy) {
	endHistogram_t retval;

	// Stand / Bust
	if ((!ace && i >= 22) || (ace && i >= 21)) retval[0] = 1;
	else if ((*strategy)[ace][i] == S) {
		if (!ace) retval[i] = 1;
		else if (i <= 10) retval[i + 11] = 1;
		else retval[i + 1] = 1;
	}

	// Hit
	else for (size_t c = 1; c <= 10; c++) {
		endHistogram_t recursedHistogram;

		if (c == 1) { // Received an ace
			if (ace) recursedHistogram = getPlayerHistogram(i + 1, true, strategy);
			else recursedHistogram = getPlayerHistogram(i, true, strategy);
		}	else { // Received any other card
			recursedHistogram = getPlayerHistogram(i + c, ace, strategy);
		}

		// Merge in the recursed probability
		float probC = (float)(c == 10 ? 4 : 1) / 13;
		for (size_t i = 0; i < 22; i++)
			retval[i] += probC * recursedHistogram[i];
	}

	// Return the computed histogram
	return retval;
}


// Computes the expected winnings given the players sum and the dealers distribution
double expectedWinnings(size_t playerSum, endHistogram_t * dealerHistogram) {
	if (playerSum > 21) return 0;
	else {
		double retval = (*dealerHistogram)[playerSum] / 2;
		for (size_t i = 0; i < playerSum; i++) retval += (*dealerHistogram)[i];
		return retval;
	}
}


// Recursively computes the optimal strategy, given a dealer distribution, returning its expected payoff
typedef double expectedReward_t[2][22];
double optimalStrategy(size_t i, bool ace, strategySet_t * strategy, expectedReward_t * expectedReward, endHistogram_t * dealerHistogram) {
	endHistogram_t retval;

	// Bust
	if ((!ace && i >= 22) || (ace && i >= 21)) return 0;

	// If already computed
	if ((*strategy)[ace][i] != UNDEFINED) return (*expectedReward)[ace][i];

	// Stand
	double standReward;
	if (!ace) standReward = expectedWinnings(i, dealerHistogram);
	else if (i <= 10) standReward = expectedWinnings(i + 11, dealerHistogram);
	else standReward = expectedWinnings(i + 1, dealerHistogram);

	// Hit
	double hitReward = 0;
	for (size_t c = 1; c <= 10; c++) {
		double recursedHitReward;
		if (c == 1) { // Received an ace
			if (ace) recursedHitReward = optimalStrategy(i + 1, true, strategy, expectedReward, dealerHistogram);
			else recursedHitReward = optimalStrategy(i, true, strategy, expectedReward, dealerHistogram);
		} else { // Received any other card
			recursedHitReward = optimalStrategy(i + c, ace, strategy, expectedReward, dealerHistogram);
		}

		// Merge in the recursed probability
		float probC = (float)(c == 10 ? 4 : 1) / 13; // To consider 10=J=Q=K occurs 4 times more
		hitReward += probC * recursedHitReward;
	}

	// Make SubgamePerfectEquilibrium choice
	action_t action;
	double reward;
	if (standReward > hitReward) {
		action = S;
		reward = standReward;
	} else {
		action = H;
		reward = hitReward;
	}

	// Memorize results and return
	(*strategy)[ace][i] = action;
	(*expectedReward)[ace][i] = reward;
	return reward;
}


// Computes the probability distibution at the end of the game for the dealer, given his visible card
// By default, computes the distribution for all deques
endHistogram_t getDealerHistogram(size_t dealerStartingAmount = 0, bool startingAce = false) {
	strategySet_t dealerStrat = {
		/* Hard */{ H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, S, S, S, S, S },
		/* Soft */{ H, H, H, H, H, H, S, S, S, S, S, H, H, H, H, H, S, S, S, S, S, S }
	};
	return getPlayerHistogram(dealerStartingAmount, startingAce, &dealerStrat);
}


// Computes how much a particular player strategy is expected to earn
double strategyExpectedWinnings(playerStrategySet_t * strategy) {
	double winnings = 0;
	for (size_t c = 1; c <= 10; c++) {

		// Compute the dealer end-histogram for this card
		endHistogram_t dealerHistogram;
		if (c == 1) dealerHistogram = getDealerHistogram(0, true);
		else dealerHistogram = getDealerHistogram(c, false);

		// Compute the outcome of the strategy
		endHistogram_t playerHistogram = getPlayerHistogram(0, false, &(*strategy)[c]);

		// Compute the winnings
		float probC = (float)(c == 10 ? 4 : 1) / 13;
		for (size_t i = 1; i < 22; i++)
			winnings += probC * playerHistogram[i] * expectedWinnings(i, &dealerHistogram);
	}
	return winnings;
}


// Computes the optimal strategy for each dealer card
playerStrategySet_t * getOptimalStrategy() {
	action_t * aux = (action_t*)calloc(11 * 2 * 22, sizeof(action_t));
	playerStrategySet_t * optimalStrat = (playerStrategySet_t*)aux;

	for (size_t c = 1; c <= 10; c++) {

		// Compute the dealer end-histogram for this card
		endHistogram_t dealerHistogram;
		if (c == 1) dealerHistogram = getDealerHistogram(0, true);
		else dealerHistogram = getDealerHistogram(c, false);

		// Compute the optimal strategy for this visible card
		expectedReward_t expectedReward; memset(expectedReward, 0x00, sizeof(expectedReward_t));
		optimalStrategy(0, false, &((*optimalStrat)[c]), &expectedReward, &dealerHistogram);
	}

	return optimalStrat;
}

int main(void){
	// Compute the overall dealer distribution
	//printf("Dealer final card histogram: \n");
	//endHistogram_t dealerHistogram = getDealerHistogram();
	//for (size_t i = 0; i < 22; i++) printf("%.3lf ", dealerHistogram[i]);
	//printf("\n");

	// Compute the optimal strategy
	printf("Optimal strategy: \n");
	playerStrategySet_t * strategy = getOptimalStrategy();
	for (size_t j = 0; j < 2; j++) {
		printf("%s Hand:\n", j == 0 ? "Hard" : "Soft");
		for (size_t k = (j == 0 ? 4 : 1); k < 22; k++) {
			printf("player sum %02zu: ", k);
			for (size_t i = 1; i <= 10; i++) {
				printf("%c ", (*strategy)[i][j][k] == H ? 'H' : 'S');
			}
			printf("\n");
		}
	}

	// Compute the expected rewards for the player
	printf("SPE strategy has expected winnings of %lf for ever 1 unit spent\n", strategyExpectedWinnings(strategy));
	free(strategy);
}
