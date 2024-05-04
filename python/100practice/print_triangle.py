row = int(input('please enter the row: '))
'''
for i in range(1, row + 1):
    print(i)
'''

for i in range(1, row + 1):
    for j in range(1, row + 1):
        if (j <= i):
            print('*', end='')
        else:
            print(' ', end='')
    print()

print()

for i in range(1, row + 1):
    for j in range(1, row + 1):
        if (j <= (row - i)):
            print(' ', end='')
        else:
            print('*', end='')
    print()
print()

