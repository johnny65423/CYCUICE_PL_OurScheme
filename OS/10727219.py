import time
import threading
from numba import jit
@jit()



def Bubblesort(data, num):
    # 定義資料長度
    n = len(data)
    xxx = n / 100
    int = 1
    for i in range(n-2):                   # 有 n 個資料長度，但只要執行 n-1 次
        #if( i % xxx == 0 ):
        #    print( int,"%" )
        #    int+= 1
        for j in range(n-i-1):             # 從第1個開始比較直到最後一個還沒到最終位置的數字 
            if data[j] > data[j+1]:        # 比大小然後互換
                data[j], data[j+1] = data[j+1], data[j]
    
    print(num,"done")


#filename = input("請輸入檔案名稱：")
k = input("請輸入要切成幾份：")
k = int(k)
filename = "input_1w"
filename = filename + ".txt"
f = open(filename, "r")

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

m1list = numlist.copy()

start = time.time()

Bubblesort(m1list,0)

end = time.time()

print("M1執行時間：%f 秒" % (end - start))

#====================================Mission1====================================

#====================================Mission2====================================

step = int(len(numlist) / k)+1

if len(numlist) % k == 0 :
    add = -1
    step -= 1
else:
    add = len(numlist) % k
    
start = time.time()    
    
index = 0
m2list=[]
while ( index < len(numlist) ) :

    if( add > 0 ):
        m2list.append(numlist[index: index + step])
        add-=1
        
        
    else:
        m2list.append(numlist[index: index + step])
    index = index + step 
    if( add == 0 ):
        add = -1
        step -= 1  
        
m2thread = []    
for i in range(len(m2list)):
    m2thread.append(threading.Thread(target = Bubblesort, args = (m2list[i],i)))
    m2thread[i].start()

for i in range(len(m2thread)):
    m2thread[i].join()
    
end = time.time()

print("M2執行時間：%f 秒" % (end - start))

#for i in range(len(m2list)):
#    print(m2list[i])

#====================================Mission2====================================
