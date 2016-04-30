# hackerrank
Solutions to HackerRank tasks

## Multiple Linear Regression - Predicting Housing Prices
https://www.hackerrank.com/challenges/predicting-house-prices

Input data is N house records with F features each, and the resulting prices per square foot for each of them.

The goal is to calculate the coefficients for a linear function that minimize the sum of square errors between actual prices per square foot and the predicted values on the line.

Since the constraints in the task are very low (number of features F <= 10), the quickest and logical approach is to use the normal equation solution, $$ \theta = (X'*X)^-1 * X' * y $$

If the number of features was (far) bigger, calculating the matrix inverse would be very expensive and alternative approach, such as gradient descent, would be better.
