import threading
import time

def print111():
    print('111')

def Merge(list1, list2):
    # 定義資料長度
    i1 = 0
    i2 = 0
    relist = []
    while ( i1 < len(list1) - 1 or i2 < len(list2) ) :
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
    
    print("done")    
    return relist


def Mergelist( arraylist, num1, num2 ):
    if( num1 != num2 ) :
        arraylist[num1] = Merge( arraylist[num1], arraylist[num2] )
        del arraylist[num2]


a = []
a.append( [4,6,8] )
a.append( [7,10] )
print(a)


m2thread2 = threading.Thread(target = Mergelist, args = (a,0, 1))

m2thread2.start()

m2thread2.join() 

print(a)
