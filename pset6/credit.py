import cs50

def main():
    
    # sanity check
    while True:
        print("Number: ", end="")
        cc = cs50.get_int()
        if cc > 0:
            break
    
    # immediately exit if <13 digits
    if cc < 1E12:
        print("INVALID")
        exit(0)

    # set a checksum to zero
    checksum = 0
    
    # grab every other digit from the end,
    # multiply each by two
    for i in step_by_multiply(10, cc, 100):
        x = (cc % (i * 10) // i) * 2
        
        # parse out the digits from the 
        # previous line and sum them up
        for j in step_by_divide(10, 0, 10):
            checksum += x // j
    
    # snag all the other digits and
    # add each one to the checksum
    for k in step_by_multiply(1, cc, 100):
        y = cc % (k * 10) // k
        checksum += y

    if checksum % 10 == 0:
        
        # added these vars to improve readability
        amex_prefix = cc // 1E13
        mc_prefix = cc // 1E14
        visa_prefix13 = cc // 1E12
        visa_prefix16 = cc // 1E15
        discover_4prefix = cc // 1E12
        discover_2prefix = cc // 1E14

        if amex_prefix == 34 or amex_prefix == 37:
            print("AMEX")

        elif mc_prefix >= 51 and mc_prefix <= 55:
            print("MASTERCARD")

        elif visa_prefix13 == 4 or visa_prefix16 == 4:
            print("VISA")
            
        elif discover_2prefix == 65 or discover_4prefix == 6011:
            print("DISCOVER")

        else:
            print("INVALID")

    else:
        print("INVALID")

# implements a loop to step by multiplication
def step_by_multiply(n, limit, multiplier):
    while n < limit:
        yield n
        n *= multiplier

# implements a loop to step by division
def step_by_divide(n, limit, divisor):
    while n > limit:
        yield n
        n //= divisor

if __name__ == "__main__":
    main()