import matplotlib.pyplot as plt
import numpy as np
import scipy.io.wavfile as spw

sound = spw.read("aggressivehugehit.wav")

time = []

for i in range(len(sound[1])/sound[0]):
    time.append(i)

fig, ax = plt.subplots()             # Create a figure containing a single Axes.
ax.plot(time, sound[1])  # Plot some data on the Axes.
plt.show()    
