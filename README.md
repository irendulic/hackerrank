# hackerrank
Solutions to HackerRank tasks

## Multiple Linear Regression - Predicting Housing Prices
https://www.hackerrank.com/challenges/predicting-house-prices

Input data is N house records with F features each, and the resulting prices per square foot for each of them.

The goal is to calculate the coefficients for a linear function that minimize the sum of square errors between actual prices per square foot and the predicted values on the line.

Since the constraints in the task are very low (number of features F <= 10), the quickest and logical approach is to use the normal equation solution, theta = (X'*X)^-1 * X' * y

If the number of features was (far) bigger, calculating the matrix inverse would be very expensive and alternative approach, such as gradient descent, would be better.


## Hip board game
https://www.hackerrank.com/challenges/hip

Played on a 9x9 board, the goal is to choose points such that you don't form a square, and to make the opponent form one.

Given the input board situation, we locate each possible square that could be made (for each pair of vertices marked on the board). For each pair of vertices there are three possible squares - either that pair is diagonal, or it is one of the sides (for which there are two options). For the diagonal case the solution for the other two vertices doesn't need to be whole number, so that needs to be checked. The calculation of possible squares is made for both the player and the opponent.

Given the calculated possible square location, a cost function for each field is calculated, both for the player and for the opponent.
 - If the point would create a square for the player, it has -INF value for the player, or -A for the opponent.
 - If the point is one of the two missing vertices to create a square, and both vertices are on the board, it has -1 value.
 - If the point is one of the two missing vertices to create a square, but the other one is outside the board, it has +1 value - this is the favorable solution.
 
Additionally, points that are closer to the edge of the board are more favorable, because they are more likely to create the third, favorable, case.

This solution scored 62.24, rank 9/104, with 34/44 games won, and 10 losses.
