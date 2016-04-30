import numpy as np

if __name__ == "__main__":
  # Read the data.
  # X is Nx(F+1) array, with first column equal to ones (artificial feature x0 for convenience).
  # y is the matching result vector.
  line = raw_input().split(' ')
  F = int(line[0])
  N = int(line[1])
  X = np.zeros((N, F+1))
  y = np.zeros((N, 1))
  for i in range(0,N):
    line = raw_input().split(' ')
    X[i, 0] = 1
    X[i, 1:] = line[0:-1]
    y[i] = line[-1]

  # Read the data we need to predict.
  T = int(raw_input())
  X_u = np.zeros((T, F+1))
  for i in range(0, T):
    X_u[i, 0] = 1
    X_u[i, 1:] = raw_input().split(' ')

  # The normal equation for optimal multivariate linear regression solution.
  # Given the task constraints on number of features, normal equation makes more sense than gradient descent.
  theta = np.linalg.pinv(np.transpose(X).dot(X)).dot(np.transpose(X)).dot(y)
  y_u = X_u.dot(theta)
  for i in range(0, T):
    print y_u[i,0]
