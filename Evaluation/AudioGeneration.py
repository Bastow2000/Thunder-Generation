from scipy.fftpack import fft, ifft
from scipy.io.wavfile import write
import numpy as np

sampleRate = 44100
numSamples = int(sampleRate)

# Generate white noise
whiteNoise = np.random.normal(0, 1, numSamples)

# Normalises Function
def normaliseFunc(buffer):
    
    maxValue = np.max(np.abs(buffer))
    # Check for zero or NaN
    if maxValue == 0 or np.isnan(maxValue): 
        return buffer
    return buffer / maxValue
    
# Writes Wav file for white noise
# Scale for 16-bit integer needed for audio playback
write("/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Samples used for analysis/Python Generated/WhiteNoise.wav", sampleRate, (whiteNoise * 32767).astype(np.int16))

# Gaussian Curve for natural generation
def gaussianCurve():
    # box Muller Transformation
    rand1 = np.clip(np.random.random(), 1e-10, 1)
    rand2 = np.clip(np.random.random(), 1e-10, 1)
    gaussian = np.sqrt(-2.0 * np.log(rand1)) * np.cos(2.0 * np.pi * rand2)
    return gaussian

# Generates probability box for bias white noise
def calculateNoise():
    value = 0.0
    chances = gaussianCurve() 

    # 10% probability
    if chances <= 0.1:
        value = (np.random.random() * 1.75 - 0.75) * gaussianCurve()
    # 20% probability
    elif chances <= 0.3:
        value = np.random.random() * 1.1 - 0.1
    # 40% probability
    elif chances <= 0.7:
        value = np.random.random() * 1.0005 - 0.0005
    # 1% probability
    elif chances <= 0.99:
        value = (np.random.random() * 1.15 - 0.15) * gaussianCurve()
    # 29% probability
    else:
        value = np.random.random() * 1.05 - 0.05

    return value

# Generate White noise
def whiteNoiseGen():
    alpha = 0.5
    value = calculateNoise()
    smoothen = 0.0
    
    # Sets up buffer with zeros matching 44100
    buffer = np.zeros(numSamples)
    for sample in range(numSamples):
        # Obtains new white Noise bias
        
        biasNoise = np.random.random() * value 
        
        # Applies exponential moving average Filtering
        smoothen = (alpha * biasNoise) + (1.0 - alpha) * smoothen
        
        # Fills buffer with bias white noise
        buffer[sample] = smoothen
    return buffer

modifiedNoise = whiteNoiseGen()

# Normalise and scale for 16-bit integer needed for audio playback
write("/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Samples used for analysis/Python Generated/ModNoise.wav", sampleRate, (normaliseFunc(modifiedNoise)* 32767).astype(np.int16))

# Applies FFT Modifications to code
def fftModification(buffer, decayRate, freqMod):
    
    # Applies FFT
    fftBuffer = fft(buffer)
    
    # Creates an array the length of the frequency components
    frequencies = np.fft.fftfreq(len(buffer), 1/sampleRate)

    def expDecay(freq):
        # Exponential decay of frequencies
        return np.exp(-decayRate * np.log1p(np.abs(freq) / (sampleRate / 2)) / np.log(20))

    # Apply that to the frequency components
    fftBuffer *= expDecay(frequencies)
    
    # Modify the frequency components based on the modifications in freq mod
    for startFreq, endFreq, factor in freqMod:
        # Apply the modification only to the frequencies within the specified range
        for i in range(len(frequencies)):
            frequency = np.abs(frequencies[i])
            if startFreq <= frequency <= endFreq:
                fftBuffer[i] *= factor

    # Convert back to time domain
    return np.real(ifft(fftBuffer))

# Frequency modifications
freqMod = [
    (0, 63, -1),
    (64, 127, -5),
    (128, 257, -5),
    (258, 511, -7),
    (512, 1023, -10),
    (1024, 2047, -15),
    (2048, 4095, -15),
    (4096, 6000, -15),
    (6000, 8171, -20),
    (8172, 16343, -30),
    (16344, 20000, -35),
    (20001, 44100, 0)
]

fftaudio = fftModification(modifiedNoise, 95, freqMod)

# Normalise and scale for 16-bit integer needed for audio playback
write("/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Samples used for analysis/Python Generated/FFTOnlyNoise.wav", sampleRate, (normaliseFunc(fftaudio)* 32767).astype(np.int16))

# Generates N wave and multiplies it with original signal
def nWaveGen(buffer):
    # Create nWave Buffer filled with zeros
    nWave = np.zeros(numSamples)

    # Loops over buffer to calulate NWave
    for sample in range(numSamples):
        
        # Calculates the total time
        t = (float(sample) / sampleRate) 
        
        # Calculates the period of the wave
        T = 1 / float(sampleRate)
        
        # Center of wave
        time = 0.010155

        if t < time:
            nWave[sample] = -t / T
        else:
            nWave[sample] =  -t / T

    return buffer * nWave

Nwave = nWaveGen(fftaudio)

# Normalise and scale for 16-bit integer needed for audio playback
write("/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Samples used for analysis/Python Generated/NWaveNoise.wav", sampleRate, (normaliseFunc(Nwave)* 32767).astype(np.int16))

# Low pass filter coefficients 
def lowPassFilter(input, frequency, lastX, lastY):
  
    omega_c = 2 * np.pi * frequency / sampleRate
    b0 = omega_c / (omega_c + 1)
    a1 = (omega_c - 1) / (omega_c + 1)
    output = b0 * input + b0 * lastX - a1 * lastY
    return output, input, output 

def varyingLowpassFilter(buffer, sampleRate, cutoffFrequencyStart, cutoffFrequencyEnd, steepness):
    # Normalise buffer previous error
    buffer = normaliseFunc(buffer) 
    
    # Buffer length
    bufferSize = len(buffer)
    
    # Controls how the value changes over time
    step = (cutoffFrequencyEnd - cutoffFrequencyStart) / (bufferSize ** steepness)
    
    lastX = 0
    lastY = 0
    
    # New buffer filled with zeros
    filteredBuffer = np.zeros(bufferSize)

    for i in range(bufferSize):
        # Calculates cutoff frequency
        frequency = max(cutoffFrequencyStart + (i ** steepness) * step, 0)
        input = buffer[i]
        # Gets low pass coefficients
        output, lastX, lastY = lowPassFilter(input, frequency, lastX, lastY)
        filteredBuffer[i] = output

    return filteredBuffer

# Applying the varying low-pass filter with steepness control
filteredBuffer = varyingLowpassFilter(Nwave, 44100, 20000, 0, 0.05)

# Normalise and scale for 16-bit integer needed for audio playback
write("/Users/bastow_boii/Desktop/FMP Thunder Model C++/Evaluation/survey audio samples/Samples used for analysis/Python Generated/VaryingFilter.wav", sampleRate, (normaliseFunc(filteredBuffer) * 32767).astype(np.int16))