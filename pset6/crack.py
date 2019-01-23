import crypt
import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: python crack.py hash")
        exit(1)
    
    hashed = sys.argv[1]
    salt = hashed[0] + hashed[1]
    seedchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    password = ""
    
    # test password of length 1
    for i in seedchars:
        password = i
        check_password(password, hashed, salt)
    
    # test password of length 2
    for i in seedchars:
        for j in seedchars:
            password = i + j
            check_password(password, hashed, salt)
                
    # test password of length 3
    for i in seedchars:
        for j in seedchars:
            for k in seedchars:
                password = i + j + k
                check_password(password, hashed, salt)
                    
    # test password of length 4
    for i in seedchars:
        for j in seedchars:
            for k in seedchars:
                for l in seedchars:
                    password = i + j + k + l
                    check_password(password, hashed, salt)
                
    print("Password not found :(")
    exit(1)
                    
def check_password(password, hashed, salt):
    encrypted = crypt.crypt(password, salt)
    if encrypted == hashed:
        print("{}".format(password))
        exit(0)
        
if __name__ == "__main__":
    main()