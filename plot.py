import numpy as np
import matplotlib.pyplot as plt

distances = np.loadtxt("data.tmp")

plt.hist(distances, bins=300)
plt.savefig("plot.pdf")