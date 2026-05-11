import cv2 as cv

# 图片降低像素质量算法 - 1MB以下

import os

img = cv.imread("./data/penguin.jpg")

# 1. 缩小分辨率：宽高各缩小一半
h, w = img.shape[:2]
img = cv.resize(img, (w // 2, h // 2), interpolation=cv.INTER_AREA)

# 2. 降低JPEG质量，反复压缩直到小于1MB
output_path = "./data/penguin_small.jpg"
quality = 90

while True:
    cv.imwrite(output_path, img, [cv.IMWRITE_JPEG_QUALITY, quality])
    size = os.path.getsize(output_path)  # 字节数
    if size < 1024 * 1024 or quality <= 10:
        break
    quality -= 5

print(f"完成：{size / 1024:.1f} KB，质量={quality}")
