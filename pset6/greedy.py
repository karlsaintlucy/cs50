import cs50

while True:
    print("How much change is owed?")
    change = cs50.get_float()
    if change > 0:
        break

# convert the input to an integer
change = round(change * 100)

# calculate quantity of each type of coin
q = change // 25
d = change % 25 // 10
n = change % 25 % 10 // 5
p = change % 25 % 10 % 5 // 1

# print the sum of all the coin quantities
print(q + d + n + p)

exit(0)