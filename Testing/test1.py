import serial
from termcolor import colored

com = input('Введите компорт:')
ser = serial.Serial(com if len(com) != 0 else 'COM4', 115200)
task = input('Введите номер задания:')
answer = input('Введите ожидаемый результат:') if task != '1.4.2' else None
fibonacci = [0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711,
             28657, 46368, 75025, 121393, 196418, 317811, 514229]


def test(answer, data):
    if data == answer:
        print(colored('Тест пройден', 'green'))
        exit(0)
    else:
        print(colored(f'Тест провален, ожидалось - "{answer}", получено - "{data}"', 'red'))
        exit(-1)


def testFib(answer, data, i):
    if data != answer:
        print(colored(f'Тест провален, ошибка в {i}-ом числе Фибоначчи, ожидалось - "{answer}", получено - "{data}"',
                      'red'))
        exit(-1)
    elif i == 30:
        print(colored('Тест пройден', 'green'))
        exit(0)


if task == '1.2':
    error = input('Округлить полученный результат до 5-ти знаков после запятой(да/нет):')
    while True:
        data = ser.read(size=(len(answer)))
        data = round(float(data), 5) if error == 'да' else data
        answer = round(float(answer), 5) if error == 'да' else answer
        test(answer, data)
elif task == '1.4.1':
    answerASCII = ''
    for char in answer:
        answerASCII += str(ord(char))
    while True:
        data = ser.read(size=(len(answerASCII))).decode('utf-8')
        test(answerASCII, data)
elif task == '1.4.2':
    for i in range(0, 30):
        data = ser.read(size=(len(str(fibonacci[i])))).decode('utf-8')
        data = int(data)
        test(fibonacci[i], data)
else:
    while True:
        data = ser.read(size=(len(answer))).decode('utf-8')
        test(answer, data)
