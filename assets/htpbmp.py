from PIL import Image
import numpy as np

# paste your array values here
data = np.array([
0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7940,7940,7940,7940,0,0,0
], dtype=np.uint16)

WIDTH = 24
height = int(np.ceil(len(data) / WIDTH))
# pad if needed
if len(data) < WIDTH*height:
    data = np.pad(data, (0, WIDTH*height - len(data)), 'constant', constant_values=0)

# convert RGB565 -> RGB888
vals = data.reshape((height, WIDTH))
r = ((vals >> 11) & 0x1F) << 3
g = ((vals >> 5) & 0x3F) << 2
b = (vals & 0x1F) << 3
rgb = np.dstack([r, g, b]).astype(np.uint8)

img = Image.fromarray(rgb, 'RGB')
img.save('assets\\deco1.png')
print('Saved deco1.png', img.size)
