import serial
from termcolor import colored

com = input('Введите компорт:')
ser = serial.Serial(com if len(com) != 0 else 'COM4', 115200)
task = input('Введите номер задания:')


def test(answer, data, successMsg='Тест пройден'):
    if data == answer:
        print(colored(successMsg, 'green'))
    else:
        print(colored(f'Тест провален, ожидалось - "{answer}", получено - "{data}"', 'red'))
        exit(-1)


if task == '4.1':
    initNumber = input('Введите начальное число: ')
    expectedAnswer = input('Введите ожидаемый ответ: ')
    data = ser.read(size=(len(expectedAnswer))).decode('utf-8')
    test(expectedAnswer, data)
elif task == '4.2':
    expected = input('Введите через запятую(без пробелов) ожидаемую последовательность значений:')
    expected = expected.split(',')
    error = False
    for i in range(0, len(expected)):
        temp = int((ser.read(7).decode('utf-8').replace('\n\r', '')))
        reallyExpected = round(18500 - 500 * int(expected[i]) / 90)
        if reallyExpected == int(temp):
            print(colored(f'Значение {i + 1} - верно', 'green'))
        else:
            print(colored(f'ошибка, ожидалось  {reallyExpected}, получено {temp}', 'red'))
            error = True
    if not error:
        print(colored('Тест пройден', 'green'))
    else:
        print(colored('Тест провален', 'red'))
elif task == '4.3':
    print('плюс +, минус -б делить /, умножить *')
    print('в конце выражения необходимо поставить знак =')
    expression = input('Введите математическое выражение:')
    operandFirst = 0
    operandSecond = 0
    strNum = ''
    lastSign = None
    signs = ['+', '-', '/', '*', '=']
    currentOperand = 1;
    for letter in expression:
        if letter in signs:
            if currentOperand == 1:
                operandFirst = int(strNum)
                currentOperand = 2
            else:
                operandSecond = int(strNum)
                operandFirst = int(eval(str(operandFirst) + lastSign + str(operandSecond)))
            data = ser.read(size=(len(str(operandFirst)))).decode('utf-8')
            test(operandFirst, int(data), 'Операция выполнена правильно')
            lastSign = letter
            strNum = ''
            if letter == '=':
                print(colored('Тест пройден', 'green'))
                exit(0)
        else:
            strNum += letter
