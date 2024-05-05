set1 = {1, 2, 3, 3, 3, 2}
print(set1)
print('Length = ', len(set1))

set2 = set(range(1, 10))
print(set2)

set3 = {num for num in range(1, 100) if num % 3 == 0 or num % 5 == 0}
print(set3)
