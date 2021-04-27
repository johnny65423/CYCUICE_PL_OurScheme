import multiprocessing as mp
import time


def mmm( list1, q ) :
    list1[0] = list1[0] + list1[1]
    print(list1[0])
    
    q.put(list1)

if __name__ == '__main__':
    m3list = [[1,2], [3,4], [5,6], [7,8] ]
    relist = []
    print( m3list )
    plist = []
    q = mp.Queue()
    for i in range(4) :
        plist.append( mp.Process( target = mmm, args=( m3list[i], q ) ) )
        plist[i].start()
        
    for i in range(4) :
        relist.append(q.get())
        plist[i].join()

    #relist.append(q.get())
    print( relist )