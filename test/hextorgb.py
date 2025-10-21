def hex_to_rgb(hexStr: str):

    s: str = hexStr.strip('#')

    if len(s) == 3:
        expanded = []
        for ch in s:
            expanded.append(ch * 2)
        s = ''.join(expanded)

    if len(s) != 6:
        raise ValueError("Expected hex string in format RRGGBB or RGB")

    return tuple(int(hexStr[i:i+2], 16) for i in (0, 2, 4))

def rgb_to_hex(rgb):
    return '#%02x%02x%02x' % rgb

asd = hex_to_rgb('#AD18A5')
asd = rgb_to_hex((173, 24, 165))

print(asd)