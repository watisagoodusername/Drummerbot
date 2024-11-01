import numpy as np
import scipy.io.wavfile as spw

def stereoToMono(audiodata):
    newaudiodata = []

    for i in range(len(audiodata)):
        d = (audiodata[i][0]/2) + (audiodata[i][1]/2)
        newaudiodata.append(d)

    return np.array(newaudiodata, dtype='int16')

filename = "440hz"

rate, data = spw.read(filename + ".wav")

monodata = stereoToMono(data)

spw.write(filename + ".wav", rate, monodata)