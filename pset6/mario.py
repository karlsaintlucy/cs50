import cs50

while True:
    print("Height: ", end="")
    height = cs50.get_int()
    if height >= 0 and height <= 23:
        break

for i in range(height):
    print(" " * (height - i), end="")
    print("*" * (i + 1), end="")
    print("  ", end="")
    print("*" * (i + 1))
    
exit(0)