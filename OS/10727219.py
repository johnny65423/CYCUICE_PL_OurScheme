import time
import threading
import multiprocessing as mp
from numba import jit
@jit()



def Bubblesort(data):
    # 定義資料長度
    n = len(data)
    xxx = n / 1000
    count = 0
    for i in range(n-1):                   # 有 n 個資料長度，但只要執行 n-1 次
        if( i % xxx == 0 ):
            count+= 1
        for j in range(n-i-1):             # 從第1個開始比較直到最後一個還沒到最終位置的數字 
            if data[j] > data[j+1]:        # 比大小然後互換
                data[j], data[j+1] = data[j+1], data[j]
    return data           
    
def Merge(list1, list2):
    # 定義資料長度
    i1 = 0
    i2 = 0
    relist = []
    
    while ( i1 < len(list1) or i2 < len(list2) ) :
        
        if( i1 == len(list1) ) :
            relist.append(list2[i2])
            i2+= 1
        elif ( i2 == len(list2) ) :
            relist.append(list1[i1])
            i1+= 1
        else :
            
            if( list1[i1] <= list2[i2] ) :
                relist.append(list1[i1])
                i1+= 1
            else : 
                relist.append(list2[i2])
                i2+= 1
  
    return relist


def Mergelist( arraylist, num1, num2 ):

    if( num1 != num2 ) :
        arraylist[num1] = Merge( arraylist[num1], arraylist[num2] )
        
def Makelist( numlist, k ):

    step = int(len(numlist) / k)+1
    
    if len(numlist) % k == 0 :
        add = -1
        step -= 1
    else:
        add = len(numlist) % k

    relist = []
    index = 0  
    
    while ( index < len(numlist) ) :

        if( add > 0 ):
            relist.append(numlist[index: index + step])
            add-=1
        else:
            relist.append(numlist[index: index + step])
        index = index + step 
        if( add == 0 ):
            add = -1
            step -= 1  
    
    return relist        


if __name__ == "__main__" : 
    #filename = input("請輸入檔案名稱：")
    k = input("請輸入要切成幾份：")
    k = int(k)
    filename = "input"
    openname = filename + ".txt"
    f = open(openname, "r")

    numlist = []
    s = 0 
    num = f.readline()
    while num != '':
        numlist.append(int(num))
        #s += int(num)
        #print(int(num))
        num = f.readline()
    f.close()

#====================================Mission1====================================

    '''
    m1list = numlist.copy()

    start = time.time()

    Bubblesort(m1list)

    end = time.time()

    m1name = filename+"_output1.txt"
    f = open( m1name, "w" )

    f.writelines("Sort : \n")

    for i in range(len(m1list)):
        f.writelines(str(m1list[i]))
        f.writelines("\n")
    print("M1執行時間：%f 秒" % (end - start))
    f.writelines("CPU Time：%f\n" % (end - start))
    f.writelines("Output Time：")
    f.writelines(str(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) ))

    f.writelines("\n")
    f.close()

#====================================Mission1====================================

#====================================Mission2====================================
    
    start = time.time()    
      
    m2list = Makelist( numlist, k )
    m2thread = []  
     
    for i in range(len(m2list)):
        #print("list", i, ":", m2list[i])
        m2thread.append(threading.Thread(target = Bubblesort, args = (m2list[i])))
        m2thread[i].start()
        

    for i in range(len(m2thread)):
        m2thread[i].join()

    while( len(m2list) > 1 ):
        m2thread2 = []
        for i in range( int((len(m2list) + 1) / 2 ) ):
            m2thread2.append(threading.Thread(target = Mergelist, args = (m2list,i, len(m2list) - i - 1)))

        for i in range(len(m2thread2)):
            m2thread2[i].start()
        #print(m2list)    
            
        for i in range(len(m2thread2)):
            m2thread2[i].join()
          
        
        num = int(len(m2list) - 1) 
        endindex = int(((len(m2list) + 1) / 2) - 1 )
        
        while( num > endindex ) :
            del m2list[num]
            num -= 1
        
    end = time.time()


    m2name = filename+"_output2.txt"
    f = open( m2name, "w" )

    f.writelines("Sort : \n")

    for i in range(len(m2list[0])):
        f.writelines(str(m2list[0][i]))
        f.writelines("\n")
    print("M2執行時間：%f 秒" % (end - start))
    f.writelines("CPU Time：%f\n" % (end - start))
    f.writelines("Output Time：")
    f.writelines(str(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) ))

    f.writelines("\n")
    f.close()

    ''' 
#====================================Mission2====================================

#====================================Mission3====================================
    
    start = time.time()    
      
    m3list = Makelist( numlist, k )
    print( m3list )

      # 設定處理程序數量
    pool = mp.Pool(k)

      # 運行多處理程序
    resultlist = pool.map(Bubblesort, m3list)

    print( resultlist )    
    end = time.time()


    m3name = filename+"_output3.txt"
    f = open( m3name, "w" )

    f.writelines("Sort : \n")

    for i in range(len(m3list[0])):
        f.writelines(str(m3list[0][i]))
        f.writelines("\n")
    print("M3執行時間：%f 秒" % (end - start))
    f.writelines("CPU Time：%f\n" % (end - start))
    f.writelines("Output Time：")
    f.writelines(str(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) ))

    f.writelines("\n")
    f.close()


#====================================Mission3====================================