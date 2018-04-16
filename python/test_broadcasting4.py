from numpy import array, argmin, sqrt, sum
observation = array([111.0,188.0])
codes = array([[102.0, 203.0],
               [132.0, 193.0],
               [45.0, 155.0],
               [57.0, 173.0]])
# here is the broadcast
diff = codes - observation
dist = sqrt(sum(diff**2,axis=-1))
print dist
nearest = argmin(dist)
print nearest
