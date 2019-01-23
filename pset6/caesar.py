import sys
import cs50

# sanity check
if len(sys.argv) != 2:
    print("Usage: python caesar.py k")
    exit(1)
    
# set the key to the user's passed key number
k = ord(sys.argv[1])

# get the client's plaintext string
print("plaintext: ", end="")
plaintext = cs50.get_string()

# Print a nice prefix to the ciphertext
print("ciphertext: ", end="")

# Encipher the text
for c in plaintext:
    
    if c.isupper():
        temp = ord(c) - 65
        c = chr(((temp + k) % 26) + 65)
        print("{}".format(c), end="")
    
    elif c.islower():
        temp = ord(c) - 97
        c = chr(((temp + k) % 26) + 97)
        print("{}".format(c), end="")
    
    else:
        print("{}".format(c), end="")
        
print()