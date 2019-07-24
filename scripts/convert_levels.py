from struct import pack

i = 1
kinds = {
    'red': 0,
    'blue': 1,
    'green': 2,
    'yellow': 3,
    'pink': 4,
    'purple': 5,
    'white': 6,
    'black': 7,
    'lead': 8,
    'zebra': 9,
    'rainbow': 10,
    'ceramic': 11,
    'moab': 12,
    'bfb': 13,
    'zomg': 14,
    'ddt': 15,
    'bad': 16
}
while True:
    try:
        level = open(f"assets/Levels/level{i}.csv").read().split('\n')[1:]
        with open(f'assets/Levels/level{i}.data', 'wb') as out_file:
            for sequence in level:
                delay, kind, amount, bps = sequence.split(',')
                delay = int(float(delay) * 60)
                amount = int(amount)
                bps = float(bps)
                kind = kind.split(' ')
                out_file.write(pack('<I', delay))
                out_file.write(pack('<I', kinds[kind[0]]))
                out_file.write(pack('<I', amount))
                out_file.write(pack('<f', bps))
                out_file.write(bytes([len(kind) > 1 and 'r' in kind[1]]))
                out_file.write(bytes([len(kind) > 1 and 'c' in kind[1]]))
                out_file.write(bytes([len(kind) > 1 and 'f' in kind[1]]))
    except FileNotFoundError:
        break
    i += 1
