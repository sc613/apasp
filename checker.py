import sys

def chk(apsp_path, apasp_path):
    f1 = open(apsp_path, 'r')
    f2 = open(apasp_path ,'r')

    apsp = f1.readlines()
    apasp = f2.readlines()

    for l1, l2 in zip(apsp, apasp):
        t1 = l1.split()
        t2 = l2.split()

        for s1, s2 in zip(t1, t2):
            d1 = int(s1)
            d2 = int(s2)

            if d1 > d2 or d1+2 < d2:
                return False
    
    return True

if __name__ == "__main__":
    p1 = sys.argv[1]
    p2 = sys.argv[2]

    if chk(p1, p2):
        print("Correct!")
    else:
        print("Incorrect!")