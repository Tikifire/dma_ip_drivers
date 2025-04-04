from PIL import Image
import numpy as np

def convert_to_raw(input_path, output_path, target_size=(1920, 1080)):
    img = Image.open(input_path).convert("L").convert("RGB")

    if img.size != target_size:
        img = img.resize(target_size, Image.LANCZOS)
    
    print(f"{img.mode}")
    # img.save(f"output.bmp", "BMP")

    img_bytes = np.array(img).tobytes()
    with open(output_path, "wb") as f:
        f.write(img_bytes)

convert_to_raw("input.jpeg", "output2")

# sudo ./dma_to_device -d /dev/xdma0_h2c_0 -f ../../app/pic_to_rgb8/output -s 6220800 -a 0x0 -c 1
# sudo ./dma_to_device -d /dev/xdma0_h2c_0 -s 6220800 -a 0x0 -c 1 -f ../../app/pic_to_rgb8/output2
