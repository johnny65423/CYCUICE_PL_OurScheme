# 將資料夾底下的圖片檔案依序命名
import os
import re
import shutil
# from pathlib2 import Path

path = r"E:\manga\black"
# 批量命名图片
def renamePic(filelist):
    i = 1
    for item in filelist:
        # 获取图片名
        newName = str(i) + ".jpg"
        i = i + 1
        # 重命名
        os.rename(os.path.join(path, item), os.path.join(path, newName))
        print(os.path.join(path, item))

if __name__ == '__main__':
    # 文件路径--跟代码同目录
    filelist = os.listdir(path)
    renamePic(filelist)