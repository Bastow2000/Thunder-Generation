import json
import statistics
import ast
import matplotlib.pyplot as plt

# Modified from previous evaluation code to fit my data

# Define audio files and corresponding labels
audioFiles = [
    "Sound 1.wav", "Sound 2.wav", "Sound 3.wav", 
    "Sound 4.wav", "Sound 5.wav", "Sound 6.wav", "Sound 7.wav"
]

labels = ["Farnell", "Fineberg", "FXive", "Real Thunder", "Saksela", "Bastow 1", "Bastow 2"]

# Initialise evaluation and demographic data storage
scoresMap = {file: [] for file in audioFiles}
demographics = {key: [] for key in ["age", "hearing", "hardware", "room", "audioProf"]}

# Load survey responses from the JSON file
jsonFilePath = '/Users/bastow_boii/Audio Programming/Python/Audio Signal Processing/DSP/acr-test-20240820153103060340.json'
with open(jsonFilePath) as f:
    responses = json.load(f)

# Process each survey response
for response in responses:
    print("Processing response ID:", response["_id"]["$oid"])
    items = response.get("items", [])

    for item in items:
        question = item.get("questionControl", {}).get("question")
        value = item.get("questionControl", {}).get("value")

        if question == "How old are you ?":
            demographics["age"].append(int(value))
        elif question == "How would you describe your hearing ?":
            demographics["hearing"].append(value.strip("[]\""))
        elif question == "What Listening environment are you in right now ?":
            try:
                roomList = ast.literal_eval(value)
                demographics["room"].extend(roomList)
            except ValueError:
                demographics["room"].append(value.strip("[]\""))
        elif question == "What are you using to listen to these audio clips ? ":
            try:
                hardwareList = ast.literal_eval(value)
                demographics["hardware"].extend(hardwareList)
            except ValueError:
                demographics["hardware"].append(value.strip("[]\""))
        elif question == "How would you describe your knowledge of audio quality ?":
            demographics["audioProf"].append(value.strip("[]\""))

        if "example" in item:
            media = item["example"].get("medias", [])
            if media:
                sound = media[0]["filename"]
                score = int(media[0]["value"])
                scoresMap[sound].append(score)

# Calculate and print median age
if demographics["age"]:
    medianAge = statistics.median(demographics["age"])
else:
    medianAge = None
print("Collected ages:", demographics["age"])
print("Median age:", medianAge)

# Function to count occurrences in demographic data
def countOccurrences(dataList):
    occurrenceMap = {}
    for item in dataList:
        occurrenceMap[item] = occurrenceMap.get(item, 0) + 1
    return occurrenceMap

# Count and print demographic data
hearingData = countOccurrences(demographics["hearing"])
roomData = countOccurrences(demographics["room"])
hardwareData = countOccurrences(demographics["hardware"])
audioProfData = countOccurrences(demographics["audioProf"])

print("Hearing Data:", hearingData)
print("Room Data:", roomData)
print("Hardware Data:", hardwareData)
print("Audio Professionalism Data:", audioProfData)

# Calculate and print scores for each audio file
for idx, audioFile in enumerate(audioFiles):
    scores = scoresMap[audioFile]
    if scores:
        avgScore = sum(scores) / len(scores)
        medianScore = statistics.median(scores)
        print(f"{labels[idx]} - Average Score: {avgScore:.2f}, Median Score: {medianScore:.2f}")
    else:
        print(f"{labels[idx]} has no scores.")

# Plot the boxplot for the audio files
boxPlotData = [scoresMap[audioFile] for audioFile in audioFiles]
plt.boxplot(boxPlotData, notch=True, whis=(60, 100), vert=True, patch_artist=True, labels=labels)
plt.title('Realness Score of Thunder Audio Clips')
plt.ylabel('Realness Score')
plt.xlabel('Audio Clips')
plt.show()