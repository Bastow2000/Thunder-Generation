import os
import librosa
import numpy as np
import matplotlib.pyplot as plt

# Plots Waveform
def plotWaveform(waveform, label):
    plt.plot(waveform[70:570])
    plt.title(label)
    plt.xlabel('Samples')
    plt.ylabel('Amplitude')
    plt.show()

# Gets audio from folder
directory1 = '/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Samples used for analysis'
directory2 = '/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Samples used for analysis/Python Generated'


audioFiles = ['Diffusion.wav', 'Varying Filter.wav', 'NWaveNoise.wav', 'FFTNoise.wav', 'ModWhiteNoise.wav', 'WhiteNoise.wav', 'Real thunder.wav']
labels = ['Diffusion', 'Varying Filter', 'NWaveNoise', 'FFTNoise', 'ModWhiteNoise', 'WhiteNoise', 'Real thunder']

audioFiles2 = ['FFTNoise.wav', 'ModNoise.wav', 'NWaveNoise.wav', 'VaryingFilter.wav', 'WhiteNoise.wav']
labels2 = ['FFTNoise', 'ModNoise', 'NWaveNoise', 'VaryingFilter', 'WhiteNoise']


# Create full paths for each file
files1 = [os.path.join(directory1, file) for file in audioFiles]
files2 = [os.path.join(directory2, file2) for file2 in audioFiles2]


# Loop through each file and plot the waveform
for filePath, label in zip(files1, labels):
    waveform, _ = librosa.load(filePath, sr=None)
    plotWaveform(waveform, label)


for filePath, label in zip(files2, labels2):
    waveform, _ = librosa.load(filePath, sr=None)
    plotWaveform(waveform, label)
