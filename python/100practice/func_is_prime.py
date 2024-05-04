from math import sqrt

def is_prime_num(n):
    is_prime = True
    end = int(sqrt(num))
    for i in range(2, end + 1):
        if n % i == 0:
            is_prime = False
            break
    return is_prime == True and n != 1

num = int(input('Please input a number:'))
prime = is_prime_num(num)
print(prime)
