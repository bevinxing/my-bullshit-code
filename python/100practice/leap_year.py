year = int(input('Please input the year: '))
is_leap_year = (0 == year % 4 and 0 != year % 100) or (0 == year % 400)
print(is_leap_year)
