import time
import threading



def bubblesort(data):
    # 定義資料長度
    n = len(data)
    for i in range(n-2):                   # 有 n 個資料長度，但只要執行 n-1 次
        for j in range(n-i-1):             # 從第1個開始比較直到最後一個還沒到最終位置的數字 
            if data[j] > data[j+1]:        # 比大小然後互換
                data[j], data[j+1] = data[j+1], data[j]








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

print(len(numlist))

start = time.time()

bubblesort(numlist)

end = time.time()

print("執行時間：%f 秒" % (end - start))

newlist=[]

step = int(len(numlist) / k)+1

if len(numlist) % k == 0 :
    add = -1
    step -= 1
else:
    add = len(numlist) % k
print("len:",step)

print( len(numlist) )
index = 0
while ( index < len(numlist) ) :
    #print(index)
    #newlist.append(numlist[index: index + step])
    if( add > 0 ):
        #print(numlist[index: index + step])
        newlist.append(numlist[index: index + step])
        add-=1
        
        
    else:
        #print(numlist[index: index + step])
        newlist.append(numlist[index: index + step])
    index = index + step 
    if( add == 0 ):
        add = -1
        step -= 1  
        
len(newlist)        
for i in range(len(newlist)):
    print( len(newlist[i]) )


