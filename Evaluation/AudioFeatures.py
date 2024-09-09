import numpy as np
import os
import matplotlib.pyplot as plt
import librosa.display
import matplotlib.pyplot as plt

def getAudioFeatures(files, labels):
    # Initialise a dictionary to store features for each file
    audioFeatures = {label: {} for label in labels}
    
    # Extract features for each file
    for file, label in zip(files, labels):
        # Load audio file
        y, sr = librosa.load(file, sr=None)

        # Extract features
        rms = librosa.feature.rms(y=y)
        spectralC= librosa.feature.spectral_centroid(y=y, sr=sr)
        spectralF = librosa.feature.spectral_flatness(y=y)
        spectralB = librosa.feature.spectral_bandwidth(y=y, sr=sr)
        standardDev = np.std(y)

        audioFeatures[label] = {
            'RMS': np.mean(rms),
            'Spectral Centroid': np.mean(spectralC),
            'Spectral Flatness': np.mean(spectralF),
            'Spectral Bandwidth': np.mean(spectralB),
            'Standard Deviation': standardDev
        }

    # Plot the extracted features
    plotGraph(audioFeatures)
        
def plotGraph(audioFeatures):
    features = list(audioFeatures[list(audioFeatures.keys())[0]].keys())
    
    numFeatures = len(features)
    # Number of items per image
    items = 3 
    
    for start in range(0, numFeatures, items):
        end = min(start + items, numFeatures)
        currentFeatures = features[start:end]

        _, axis = plt.subplots(1, len(currentFeatures), figsize=(15, 5))

        # Plot each feature in a separate subplot
        for i, feature in enumerate(currentFeatures):
            values = [audioFeatures[label][feature] for label in audioFeatures]
            axis[i].bar(audioFeatures.keys(), values, color='teal')
            axis[i].set_title(f"{feature} of Waveforms")
            axis[i].set_ylabel(feature)
            axis[i].set_xticklabels(audioFeatures.keys(), rotation=50, ha='right')

        plt.tight_layout()
        plt.show()

# List of audio files and corresponding labels
audioFiles1 = [
    "Sound 1.wav", "Sound 2.wav", "Sound 3.wav", 
    "Sound 4.wav", "Sound 5.wav", "Sound 6.wav", "Sound 7.wav"
]

audioFiles2 = ["Bastow 1.wav","Bastow 2.wav","Real Thunder.wav","Thunder Analysis 1.wav","Thunder Analysis 2.wav",
               "Thunder Analysis 3.wav","Thunder Analysis 4.wav","Thunder Analysis 5.wav","Thunder Analysis 6.wav"
            ]

labels1 = ["Farnell", "Fineberg", "FXive", "Real Thunder", "Saksela", "Bastow 1", "Bastow 2"]

labels2 = ["Bastow 1", "Bastow 2", "Real Thunder", "Bastow 3", "Bastow 4", "Bastow 5","Bastow 6","Bastow 7","Bastow 8"]

# Assuming these files are in a specific directory
folderPath1 = '/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey_audio_samples/Un labelled'

folderPath2 = '/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey_audio_samples/Varied Bastow Samples'

# Combine folder path with file names to get the full file paths
files1 = [os.path.join(folderPath1, file) for file in audioFiles1]

files2 = [os.path.join(folderPath2, file) for file in audioFiles2]

# Call the function to analyze and compare the audio files
getAudioFeatures(files1, labels1)
getAudioFeatures(files2, labels2)