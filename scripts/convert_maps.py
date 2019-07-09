from PIL import Image
from sys import argv
from struct import pack

arrows = ['→', '↘', '↓', '↙', '←', '↖', '↑', '↗']
# obstacles
image = Image.open(argv[1] + "_obstacles.bmp")
width, height = image.size
obstacles = b''
byte = 0
i = 0
for y in range(height):
    for x in range(width):
        pixel = image.getpixel((x, y))
        byte |= (2 ** (i % 8)) * (pixel == (0, 0, 0))
        print(i,pixel,byte)
        _=input()
        if i % 8 == 7:
            obstacles += bytes([byte])
            byte = 0
        i += 1
if i % 8 != 7:
    obstacles += bytes([byte])

with open(argv[1] + "_obstacles.data", 'wb') as file:
    file.write(obstacles)

# path
image = Image.open(argv[1] + "_path.bmp")
width, height = image.size
instructions = b''
i = 0
for i, pixel in enumerate(image.getdata()):
    if pixel == (255, 0, 0):
        break
start_loc_x = x = i % width
start_loc_y = y = i // width
prev_x = prev_y = next_x = next_y = 0
pixels = image.load()
pixel = pixels[x, y]

length = 0
prev_locations = [(x, y)]
directions = ((1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1))
while pixel != (255, 255, 0):
    length += 1
    possible_locations = [(pixels[x + delta_x, y + delta_y], x + delta_x, y + delta_y, mask) for
                          mask, (delta_x, delta_y) in
                          enumerate(directions) if (0 <= x + delta_x < width) and (0 <= y + delta_y < height) and not (
                x + delta_x == prev_x and y + delta_y == prev_y) and pixels[x + delta_x, y + delta_y] != (
                              255, 255, 255)]
    if len(possible_locations) == 1:
        pixel, next_x, next_y, mask = possible_locations[0]
    else:
        special_locations = sorted([loc for loc in possible_locations if loc[0][2] == 255])
        if len(special_locations) > 0:
            pixel, next_x, next_y, mask = special_locations[0]
            ttl = pixel[1] - 1
            pixels[next_x, next_y] = (pixel[0] if ttl else 0, ttl, 255 if ttl else 0)
        else:
            new_locations = [loc for loc in possible_locations if (loc[1], loc[2]) not in prev_locations]
            if len(new_locations) > 0:
                pixel, next_x, next_y, mask = new_locations[0]
            else:
                old_locations = sorted((prev_locations.index((loc[1], loc[2])), loc) for loc in possible_locations if
                                       (loc[1], loc[2]) in prev_locations)[::-1]
                pixel, next_x, next_y, mask = old_locations[0][1]
    if (next_x, next_y) in prev_locations:
        prev_locations.remove((next_x, next_y))
    prev_locations.append((next_x, next_y))
    instructions += bytes([mask])
    prev_x = x
    prev_y = y
    x = next_x
    y = next_y

with open(argv[1] + "_path.data", 'wb') as file:
    file.write(pack('<H', start_loc_x))
    file.write(pack('<H', start_loc_y))
    file.write(pack('<I', length))
    file.write(instructions)
    file.write(pack('<H', x))
    file.write(pack('<H', y))
