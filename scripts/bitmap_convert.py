import sys
from PIL import Image

def image_to_progmem(input_path, output_path):
    img = Image.open(input_path).convert('1').resize((16, 16))
    logo_bmp = []

    for y in range(img.height):
        row_bytes = [0, 0]
        for x in range(img.width):
            bit_value = img.getpixel((x, y)) // 255
            if x < 8:
                row_bytes[0] |= bit_value << (7 - x)
            else:
                row_bytes[1] |= bit_value << (15 - x)

        logo_bmp.extend(row_bytes)

    with open(output_path, 'w') as f:
        f.write('#define LOGO_HEIGHT   16\n')
        f.write('#define LOGO_WIDTH    16\n')
        f.write('static const unsigned char PROGMEM logo_bmp[] =\n')
        f.write('{\n')
        for i in range(0, len(logo_bmp), 2):
            f.write(f' 0b{logo_bmp[i]:08b}, 0b{logo_bmp[i+1]:08b},\n')
        f.write('};')

def progmem_to_image(input_path, output_path):
    with open(input_path, 'r') as f:
        progmem_text = f.read().replace('\n', '')

    logo_bmp_data = progmem_text.split('{')[1].split('}')[0]
    logo_bmp_bytes = [int(byte_str, 2) for byte_str in logo_bmp_data.strip().split(',') if byte_str.startswith('0b')]

    img = Image.new('1', (16, 16))

    for i, byte in enumerate(logo_bmp_bytes):
        for j in range(8):
            y = i // 2
            x = j + 8 * (i % 2)
            bit_value = (byte >> (7 - j)) & 1
            img.putpixel((x, y), 255 * (1 - bit_value))

    img.save(output_path)

def main(input_path, output_path):
    input_ext = input_path.split('.')[-1].lower()
    output_ext = output_path.split('.')[-1].lower()

    if input_ext in ['bmp', 'png', 'jpg', 'jpeg'] and output_ext == 'txt':
        image_to_progmem(input_path, output_path)
    elif input_ext == 'txt' and output_ext in ['bmp', 'png', 'jpg', 'jpeg']:
        progmem_to_image(input_path, output_path)
    else:
        print('Error: Invalid input-output file extension combination.')
        print('Usage: bitmap_convert.py <input_file_path> <output_file_path>')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Error: Invalid number of arguments.')
        print('Usage: bitmap_convert.py <input_file_path> <output_file_path>')
        sys.exit(1)

    input_file_path = sys.argv[1]
    output_file_path = sys.argv[2]
    main(input_file_path, output_file_path)
