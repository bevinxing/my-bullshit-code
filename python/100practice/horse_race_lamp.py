import os
import time

def main():
    content = 'Welcome to Beijing...'
    while True:
        os.system('clear')
        print(content)
        time.sleep(0.2)
        content = content[1:] + content[0]

if __name__ == '__main__':
    main()
