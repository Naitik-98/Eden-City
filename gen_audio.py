import wave
import math
import struct
import random

SAMPLE_RATE = 44100
DURATION = 10.0

with wave.open('assets/sounds/ambient.wav', 'w') as wav_file:
    wav_file.setnchannels(1)
    wav_file.setsampwidth(2)
    wav_file.setframerate(SAMPLE_RATE)
    
    for i in range(int(SAMPLE_RATE * DURATION)):
        t = float(i) / SAMPLE_RATE
        # Base low hum
        val = math.sin(2.0 * math.pi * 55.0 * t) * 0.5
        # Modulate
        val *= 0.5 + 0.5 * math.sin(2.0 * math.pi * 0.2 * t)
        
        sample = int(val * 32767.0)
        wav_file.writeframesraw(struct.pack('<h', sample))
