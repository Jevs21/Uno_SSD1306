import sys
from PIL import Image

def convert_image_to_bitmap(filename):
    image = Image.open(filename).convert('1')  # Open the image and convert it to 1-bit black and white

    width, height = image.size
    print(f"#define IMAGE_WIDTH {width}")
    print(f"#define IMAGE_HEIGHT {height}")
    print("static const unsigned char PROGMEM image_bmp[] = {")

    for y in range(height):
        for x in range(0, width, 8):
            byte = 0
            for bit in range(8):
                if x + bit < width:
                    pixel = image.getpixel((x + bit, y))
                    byte |= (1 << (7 - bit)) if pixel == 0 else 0  # Set bit if pixel is black
            print(f"0b{byte:08b},", end=' ')
        print()
    print("};")

if __name__ == '__main__':
    image_filename = sys.argv[1]
    convert_image_to_bitmap(image_filename)
