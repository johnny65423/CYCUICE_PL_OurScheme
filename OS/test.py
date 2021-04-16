import threading
import time

def print111():
    print('111')


# 定義線程
t_list = []

t1 = threading.Thread(target=print111)
t_list.append(t1)
t2 = threading.Thread(target=print111)
t_list.append(t2)
t3 = threading.Thread(target=print111)
t_list.append(t3)

# 開始工作
for t in t_list:
    t.start()

# 調整多程順序
for t in t_list:
    t.join()