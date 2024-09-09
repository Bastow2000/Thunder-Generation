
import numpy as np
import os
import matplotlib.pyplot as plt
import librosa.display

# Extracts pitch and magnitude
def pitchDistribution(files, labels):
    # Initialise empty lists 
    allPitches = []
    allMagnitudes = []
    maxPitch = 0
    maxMagnitude = 0

    # Loop through each file
    for file in files:
        # Load audio file
        y, sr = librosa.load(file, sr=None)

        # Extract pitch and magnitude
        pitches, magnitudes = extractPitch(y, sr)

        # Append pitch and magnitude to the lists
        allPitches.append(pitches)
        allMagnitudes.append(magnitudes)

        # Global max values for pitches and magnitudes
        maxPitch = 4000
        maxMagnitude = 900

    # Plot the pitch distribution for each audio file in separate groups of three
    pitchDistributionPlots(allPitches, allMagnitudes, labels, maxPitch, maxMagnitude)

# Function that extracts the pitch's
def extractPitch(y, sr):
    pitches, magnitudes = librosa.piptrack(y=y, sr=sr)
    
    # Ensuring non zero
    pitches = pitches[magnitudes > np.median(magnitudes)]
    magnitudes = magnitudes[magnitudes > np.median(magnitudes)]
    return pitches, magnitudes

def pitchDistributionPlots(allPitches, allMagnitudes, labels, maxPitch, maxMagnitude):
    numFiles = len(labels)
    # Number of items per plot
    items = 3  
    
    for start in range(0, numFiles, items):
        end = min(start + items, numFiles)
        currentPitches = allPitches[start:end]
        currentMagnitudes = allMagnitudes[start:end]
        currentLabels = labels[start:end]
        
        numPlots = len(currentPitches)
        _, axs = plt.subplots(1, numPlots, figsize=(15, 5))
        
        if numPlots == 1:  
            axs = [axs]
        
        # Plot each file's pitch-magnitude histogram in a separate subplots
        for i, (pitches, magnitude, label) in enumerate(zip(currentPitches, currentMagnitudes, currentLabels)):
            axs[i].hist(pitches, bins='auto', color='blue', alpha=0.7)
            axs[i].set_title(label)
            axs[i].set_xlabel('Frequency (Hz)')
            axs[i].set_ylabel('Magnitude')
            axs[i].set_xlim(0, maxPitch)
            axs[i].set_ylim(0, maxMagnitude)

        plt.tight_layout()
        plt.show()

# List of audio files and corresponding labels
audioFiles1 = [
    "Sound 1.wav", "Sound 2.wav", "Sound 3.wav", 
    "Sound 4.wav", "Sound 5.wav", "Sound 6.wav", "Sound 7.wav"
]
labels1 = ["Farnell", "Fineberg", "FXive", "Real Thunder", "Saksela", "Bastow 1", "Bastow 2"]

# Assuming these files are in a specific directory
directory1 = '/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Un labelled'

# Combine folder path with file names to get the full file paths
files1 = [os.path.join(directory1, file) for file in audioFiles1]

directory2 = '/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Varied Bastow Samples'

audioFiles2 = ["Bastow 1.wav","Bastow 2.wav","Real Thunder.wav","Thunder Analysis 1.wav","Thunder Analysis 2.wav",
               "Thunder Analysis 3.wav","Thunder Analysis 4.wav","Thunder Analysis 5.wav","Thunder Analysis 6.wav"
            ]
labels2 = ["Bastow 1", "Bastow 2", "Real Thunder", "Bastow 3", "Bastow 4", "Bastow 5","Bastow 6","Bastow 7","Bastow 8"]

# Combine folder path with file names to get the full file paths
files2 = [os.path.join(directory2, file) for file in audioFiles2]

pitchDistribution(files2, labels2)

pitchDistribution(files1, labels1)