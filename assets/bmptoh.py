from PIL import Image
from os import sys

def RGBToWord(r,g,b):
	rvalue=0
	rvalue = rvalue + (g>>5)	
	rvalue = rvalue + ((g & 7)<< 13)
	rvalue = rvalue + ((r>>3)<<8)
	rvalue = rvalue + ((b >> 3) << 3)
	return rvalue


def hex_to_rgb(hexStr: str):

    s: str = hexStr.strip('#')

    if len(s) == 3:
        expanded = []
        for char in s:
            expanded.append(char * 2)
        s = ''.join(expanded)

    if len(s) != 6:
        raise ValueError("Expected hex string in format RRGGBB or RGB")

    asd = tuple(int(hexStr[i:i+2], 16) for i in (0, 2, 4))
    return asd[0], asd[1], asd[2]

def rgb_to_hex(rgb):
    return '#%02x%02x%02x' % rgb

def main():
	user_input = input("Please file name from assets: ")
	
	ImageFileName=f'assets\\{user_input}'
	im=Image.open(ImageFileName)

	pixels=list(im.getdata())
	for px in pixels:
		print(RGBToWord(px[0],px[1],px[2]),end=',')
	
def hexToWord(hex):
	r, g, b = hex_to_rgb(hex)
	asd = RGBToWord(r, g, b)
	print(asd)


if __name__ == "__main__":
	#main()
    hexToWord('FF9526')
	