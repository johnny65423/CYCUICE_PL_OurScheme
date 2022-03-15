

class node:
    def __init__(self, freq, char, left=None, right=None):
        self.freq = freq
        self.char = char
        self.left = left
        self.right = right
        self.huff = ''

def appendtolist(node, val='', list=[]):
    newVal = val + str(node.huff)

    if node.left == None and node.right == None :
        result = str(node.char) + ' ' + str(newVal)
        list.append( result )

    if node.left != None :
        appendtolist(node.left, newVal, list)
    if node.right != None :
        appendtolist(node.right, newVal, list)


def Makehuffmancode(num):
    ch = []
    freq = []
    for i in range(num):
        item = input()
        item = item.split()
        if len(item) != 2:
            print('error')
            break
        ch.append(item[0])
        freq.append(int(item[1]))

    nodes = []

    for x in range(num):

        nodes.append(node(freq[x], ch[x]))

    while len(nodes) > 1:
        nodes = sorted(nodes, key=lambda x: x.freq)

        left = nodes[0]
        left.huff = 0
        right = nodes[1]
        right.huff = 1

        nfreq = left.freq + right.freq
        newNode = node(nfreq, '', left, right)

        nodes.remove(left)
        nodes.remove(right)
        nodes.append(newNode)
    list = []
    appendtolist(nodes[0], list=list)
    list.sort(key=lambda c: c[0])
    return list

count = 0
num = input()
num = int( num )
while num != 0 :
    result = Makehuffmancode( num )
    count += 1
    print( 'Huffman Codes #', count )
    for i in range( len(result) ) :
        print( result[i] )
    print( '' )
    num = input()
    num = int(num)

print( 'quit' )