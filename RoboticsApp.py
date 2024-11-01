import matplotlib.pyplot as plt
import numpy as np
import scipy.io.wavfile as spw
from scipy import signal
from scipy.fft import fftshift
from scipy.ndimage import uniform_filter1d

def findpeaks(data, times, d = None):
    peaks = []
    peaktimes = []
    """for i in range(len(sounddata)):
        if np.abs(sounddata[i]) > np.abs(sounddata[i - 1]) and np.abs(sounddata[i]) > np.abs(sounddata[i + 1]):
            peaks.append(np.abs(sounddata[i]))
            peaktimes.append(time[i])"""
    peaknum, props = signal.find_peaks(data, distance = d, width = 30)

    for peak in peaknum:
        if data[peak] > 0:
            peaks.append(data[peak])
            peaktimes.append(times[peak])
        
    return peaks, peaktimes

#setup matplotlib figure
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)
fig.set_figwidth(13)
fig.set_figheight(9)


#import sound and make variables for useful data
filename = "DrumTest"
soundrate, sounddata = spw.read(filename + ".wav")#tuple, [0] is the rate, [1] is the data

height = soundrate/2
time = np.linspace(0, len(sounddata) / soundrate, len(sounddata)) #creates an array from 0 to length of file, with a float for each item in sound

#plot sound graph
ax1.set_xlabel("time")
ax1.set_title(filename + ".wav")
ax1.plot(time, sounddata)

window = 60
cumsum = np.cumsum(np.pad(sounddata, int(window/2)))
lowpassog = (cumsum[window:] - cumsum[:-window]) / window

#peaks, so the volume of the file, vs time
peaks, peaktimes = findpeaks(sounddata, time, 100);
        
"""window = 10000
cumsum = np.cumsum(np.pad(peaks, int(window/2)))
avgpeaks = (cumsum[window:] - cumsum[:-window]) / window"""

"""avgpeaks = uniform_filter1d(peaks, size=window)"""

"""avgpeaks = signal.savgol_filter(np.abs(sounddata), window, 3)"""
        
#local maxima again
"""peaks2 = []
peaktimes2 = []
peaknum2, props2 = signal.find_peaks(peaks, distance=100)

for peak in peaknum2:
    peaks2.append(peaks[peak])
    peaktimes2.append(peaktimes[peak])"""
    
window = 8
cumsum = np.cumsum(np.pad(peaks, int(window/2)))
avgpeaks = (cumsum[window:] - cumsum[:-window]) / window


ax2.set_xlabel("time")
ax2.set_ylabel("amplitude")
ax2.set_title(filename + ".wav amplitudes")
ax1.plot(peaktimes, peaks)
ax2.plot(peaktimes, peaks)
#ax1.plot(peaktimes2, avgpeaks)

#print(len(peaknum2))

#for peak in peaknum2:
    #peaks2.append(np.abs(avgpeaks[peak]))
    #peaktimes2.append(peaktimes[peak])
        
"""peaks3 = []
peaktimes3 = []

peaknum3, props3 = signal.find_peaks(avgpeaks)
for peak in peaknum3:
    peaks3.append(avgpeaks[peak])
    peaktimes3.append(peaktimes2[peak])"""

#ax1.vlines(x = peaktimes3, colors='r', ymin=0, ymax=height)



#scipy spectrogram data
freq, time, spectrogam = signal.spectrogram(sounddata, soundrate, nfft=1024, nperseg=1024)
print ("freq = ", len(freq))

s = spectrogam

#plot scipy spectrogram
ax3.pcolormesh(time, freq, np.log10(s), norm='linear')
ax3.set_ylim(12, height)
ax3.set_yscale('log', nonpositive='clip')
ax3.set_ylabel('frequency')
ax3.set_xlabel('time')
ax3.set_title("scipy spectrogram")


#matplotlib spectrogram data and plotting
Pxx, freqs, bins, im, = ax4.specgram(sounddata, NFFT=1024, Fs = soundrate, noverlap=900)

ax4.set_ylim(12, height)
ax4.set_yscale('log', nonpositive='clip')
ax4.set_ylabel('frequency')
ax4.set_xlabel('time')
ax4.set_title("matplotlib specgram")


plt.show()  
