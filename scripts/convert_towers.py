from struct import pack
import json

components = [
    'VISIBILITY',
    'POSITION',
    'VELOCITY',
    'SPEED',
    'ATTACK_SPEED',
    'ACCELERATION',
    'PATH_INDEX',
    'LIVES',
    'KIND',
    'SHOT_KIND',
    'TYPE',
    'RANGE',
    'DAMAGE',
    'PIERCE',
    'SPREAD',
    'SHOTS_AMOUNT',
    'DISTANCE',
    'SEQUENCE',
    'ACTION',
    'DRAGGABLE',
    'RANGE_SHADOW',
    'STRATEGY',
    'REGROW',
    'CAMO',
    'FORTIFIED',
    'GOO',
    'COST',
    'POPPED_BLOONS',
    'UPGRADES',
    'UPGRADE_P',
]
tower_kinds = [
    'DART_MONKEY',
    'TACK_SHOOTER',
    'SUPER_MONKEY',
    'SNIPER_MONKEY',
    'BOMB_TOWER',
    'GLUE_GUNNER',
]
shot_kinds = [
    'SPIKE',
    'JUGGERNAUT',
    'DART',
    'RADIAL_DART',
    'GUN',
    'BOMB',
    'EXPLOSION',
    'GOO_SHOT',
    'GOO_SPLASH',
    'GLUE',
    'GUM',
    'CORROSIVE',
]
towers = json.load("assets/Towers.json")
with open(f'assets/Towers.data', 'wb') as out_file:
    for tower in towers:
        componentsMask = 0
        for i, component in enumerate(components):
            componentsMask |= (2 ** i) if component.lower() in towers[tower] else 0

        # delay = int(delay)
        # amount = int(amount)
        # bps = float(bps)
        # kind = kind.split(' ')
        # out_file.write(pack('<I', delay))
        # out_file.write(pack('<I', kinds[kind[0]]))
        # out_file.write(pack('<I', amount))
        # out_file.write(pack('<f', bps))
        # out_file.write(bytes([len(kind) > 1 and 'r' in kind[1]]))
        # out_file.write(bytes([len(kind) > 1 and 'c' in kind[1]]))
        # out_file.write(bytes([len(kind) > 1 and 'f' in kind[1]]))
