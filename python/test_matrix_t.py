import numpy as np

x = np.array([[1,2,3], [3,4,5]])
#x = np.array([1,2,3,4])
print(x.shape)    # Prints "[[1 2]
            #          [3 4]]"
x = x.T
print(x.shape)  # Prints "[[1 3]
            #          [2 4]]"

# Note that taking the transpose of a rank 1 array does nothing:
v = np.array([1,2,3])
print(v)    # Prints "[1 2 3]"
print(v.T)  # Prints "[1 2 3]"
