import numpy as np
import matplotlib.pyplot as plt

distances = np.loadtxt("data.txt")

plt.hist(distances, bins=300)
plt.savefig("plot.pdf")