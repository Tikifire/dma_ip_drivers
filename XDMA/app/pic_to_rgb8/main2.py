from PIL import Image
import pyscreenshot as ImageGrab
import subprocess
import numpy as np
import time

target_size = (1920, 1080)

while True:
    screenshot = ImageGrab.grab().convert("RGB").resize(target_size, Image.LANCZOS)
    img_bytes = np.array(screenshot).tobytes()
    with open('output3', "wb") as f:
        f.write(img_bytes)
    command = ['sudo', '../../linux-kernel/tools/dma_to_device', '-d', '/dev/xdma0_h2c_0', '-s', '6220800', '-a', '0x0', '-c', '1', '-f', './output3']
    subprocess.Popen(command)
    time.sleep(1/30)

'''
with mss.mss() as sct:
    monitor = sct.monitors[0]
    while True:
        img = sct.grab(monitor)
        img = img.resize((1920,1080))
        with open('output3', 'wb') as f:
            f.write(img.tobytes())
        command = ['sudo', '../../linux-kernel/tools/dma_to_device', '-d', '/dev/xdma0_h2c_0', '-s', '6220800', '-a', '0x0', '-c', '1', '-f', './output3']
        subprocess.Popen(command)
        time.sleep(1/30)
'''
