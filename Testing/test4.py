import serial
from termcolor import colored

com = input('Введите компорт:')
ser = serial.Serial(com if len(com) != 0 else 'COM4', 115200)
task = input('Введите номер задания:')

coding = zip(
    [1000,900,500,400,100,90,50,40,10,9,5,4,1],
    ["M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"]
)

def test(answer, data):
    if data == answer:
        print(colored('Тест пройден', 'green'))
        exit(0)
    else:
        print(colored(f'Тест провален, ожидалось - "{answer}", получено - "{data}"', 'red'))
        exit(-1)

def decToRoman(num):
    if num <= 0 or num >= 4000 or int(num) != num:
        return ""
    result = []
    for d, r in coding:
        while num >= d:
            result.append(r)
            num -= d
    return ''.join(result)

if task == '4.1':
    initNumber = input('Введите начальное число: ')
    expectedAnswer = input('Введите ожидаемый ответ: ')
    data = ser.read(size=(len(expectedAnswer))).decode('utf-8')
    test(expectedAnswer, data)
elif task == '4.2':
    num = int(input("Введите число: "))
    expectedAnswer = decToRoman(num)
    data = ser.read(size=(len(expectedAnswer))).decode('utf-8')
    test(expectedAnswer, data)



