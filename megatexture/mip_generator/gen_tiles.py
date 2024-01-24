import numpy as np
import os
import cv2


def gen_tiles(img_path: str, tile_size: int = 64, mip_level: int = 0):
    org = cv2.imread(img_path)
    tiles = [
        org[x : x + tile_size, y : y + tile_size]
        for x in range(0, org.shape[0], tile_size)
        for y in range(0, org.shape[1], tile_size)
    ]

    if not os.path.exists(f"{mip_level}"):
        os.mkdir(f"{mip_level}")

    for i, tile in enumerate(tiles):
        cv2.imwrite(f"{mip_level}/{i}.png", tile)


if __name__ == "__main__":
    for i in range(7):
        gen_tiles(f"source_image/{i}.png", mip_level=i)
