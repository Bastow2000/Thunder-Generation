import json
import ast
import pandas as pd

# Define audio files and their labels
audioFiles = [
    "Sound 1.wav", "Sound 2.wav", "Sound 3.wav", 
    "Sound 4.wav", "Sound 5.wav", "Sound 6.wav", "Sound 7.wav"
]

labels = ["Farnell", "Fineberg", "FXive", "Real Thunder", "Saksela", "Bastow 1", "Bastow 2"]

# Define hardware categories
hardwareCategories = [
    "Consumer headphones (e.g. Bose or Apple)",
    "Studio headphones (e.g. Audio Technica or other)",
    "Computer speakers",
    "Phone speakers"
]

# Initialise a dictionary to store scores
hardwareEvalMap = {hardware: {label: [] for label in labels} for hardware in hardwareCategories}

# Load survey responses from the JSON file
jsonFilePath = '/Users/bastow_boii/Audio Programming/Python/Audio Signal Processing/DSP/acr-test-20240820153103060340.json'
with open(jsonFilePath) as f:
    surveyData = json.load(f)

# Process each response in the survey data
for response in surveyData:
    items = response.get("items", [])
    hardwareUsed = None

    for item in items:
        # Determine hardware type
        if "questionControl" in item:
            question = item["questionControl"].get("question")
            value = item["questionControl"].get("value")
            if question == "What are you using to listen to these audio clips ? ":
                try:
                    hardwareUsed = ast.literal_eval(value)[0]
                except (ValueError, IndexError):
                    hardwareUsed = value.strip("[]\"")

        # Record the score if the hardware is valid
        if "example" in item and hardwareUsed in hardwareCategories:
            media = item["example"].get("medias", [])
            if media:
                sound = media[0].get("filename")
                score = int(media[0].get("value", 0))
                
                if sound in audioFiles:
                    label = labels[audioFiles.index(sound)]
                    hardwareEvalMap[hardwareUsed][label].append(score)

# Compute average scores for each label across different hardware
avgScores = {label: [] for label in labels}
for hardware in hardwareCategories:
    for label in labels:
        scores = hardwareEvalMap[hardware][label]
        avgScores[label].append(sum(scores) / len(scores) if scores else 0)

# Create a DataFrame from the average scores and print it
df = pd.DataFrame(avgScores, index=hardwareCategories).T
df.columns = ["Consumer Headphones", "Studio Headphones", "Computer Speakers", "Phone Speakers"]

print(df)
