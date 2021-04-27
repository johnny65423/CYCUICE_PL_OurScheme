import multiprocessing as mp
import time


def mmm(list1) :
    list1[0] = list1[0] + list1[1]
    print(list1[0])
    return list1
    q.put(list1)

if __name__ == '__main__':
    m3list = [[1,2], [3,4], [5,6], [7,8] ]
    relist = []
    print( m3list )
    q = mp.Queue()
      # 設定處理程序數量
    pool = mp.Pool(4)

      # 運行多處理程序
    relist = pool.map(mmm, m3list)
    #relist.append(q.get())
    print( relist )