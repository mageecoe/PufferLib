# BluffPig

  This environment emulates the game of Pig, augmented with the ability to
  bluff and call bluff. 

# Pig
  
  In Pig, players take turns to roll a single dice as many times as they wish,
  adding all roll results to a winning total, but losing their gained score for
  the turn if they roll a 1.
  
  The first player to score 100 or more points wins.
  
# BluffPig

  To make this environment more interesting, we add the option to bluff and
  call bluff on the opponent. In this setting, the player tells his opponent
  what value the roll of the dice shows. Here, the player can say something
  different than the actual value (e.g., if the actual value is 1 the player
  may benefit from saying 6, if the opponent doesn't call the bluff). The
  opponent is not able to see the dice value, unless he calls bluff.
  
  If the opponent challenges and calls bluff, the player must reveal the actual
  value of the dice. If the actual value differs from what the player said, the
  player's current score gets added to the opponent.
  
# Self play

  Instead of using self play, this environment trains a policy that plays
  against random opponents, from a set of opponents with fixed strategies.

# A turn

  Each turn consists of: 
  - Roll dice
  - Tell a value
  - If opponent calls bluff, value is revealed
    - If dice value differs, add turn score (including claimed value) to
      opponent and forfeit turn
    - If dice value matches, dice score is doubled for player
  - Add value to turn score
  - Choose to continue or forfeit turn
  
# Opponent strategy

  The opponent strategy is determined by two probabilities: the probability of
  bluffing, and the probability of calling a bluff. The probability of bluffing
  is given by the matrix {p(m | n)}, m=1,...6, n=1,...6, where each row index m
  denotes the true value, each column index n denotes the claimed value,
  and p(m | n) denotes the probability at the combination m, n.
  
  The probability of calling bluff is likewise a matrix {p(m)}, m=1,...,6,
  where m is the 

