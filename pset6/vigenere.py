import cs50
import sys

if len(sys.argv) != 2:
    print("Usage: python vigenere.py k")
    exit(1)
    
k = sys.argv[1]
klen = len(k)

c = 0

print("ciphertext: ", end="")
p = cs50.get_string()
plen = len(p)

# don't advance the k[i] if not alpha
for i in range(plen):
    if p[i].isalpha():
        
        j = c % klen
        if k[j].isupper():
            shift = ord(k[j]) - 65
            
        elif k[j].islower():
            shift = ord(k[j]) - 97
        
        
        if p[i].isupper():
            temp = chr((((ord(p[i]) - 65) + shift) % 26) + 65)
            print("{}".format(temp), end="")
            c += 1
                
        elif p[i].islower():
            temp = chr((((ord(p[i]) - 97) + shift) % 26) + 97)
            print("{}".format(temp), end="")
            c += 1

    else:
        print("{}".format(p[i]), end="")
        
print()