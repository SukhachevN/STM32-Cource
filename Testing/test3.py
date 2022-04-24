import serial
from termcolor import colored
from datetime import datetime

com = input('Введите компорт:')
ser = serial.Serial(com if len(com) != 0 else 'COM4', 115200)
task = input('Введите номер задания:')
end = 'END'
mkFreq = 16000000

if task == '3.1':
    data = ''
    expFreq = int(input('Введите частоту(Гц): '))
    expPwm = int(input('Введите скважность: '))
    while end not in data:
        data += (ser.read().decode('utf-8'))
    data = data.split(end)[0]
    strPwm, strPsc, strArr = data.split('<>')
    pwm = int(strPwm)
    psc = int(strPsc)
    arr = int(strArr)
    realPwm = (arr+1)/(arr+1-pwm)
    if realPwm == expPwm:
        print(colored('Скважность - верно', 'green'))
        realFreq = mkFreq/((arr+1)*(psc+1))
        if realFreq == expFreq:
            print(colored('Частота - верно', 'green'))
            print(colored('Тест пройден', 'green'))
        else:
            print(colored(f'Частота - ошибка, ожидалось {expFreq}, получено {realFreq}', 'red'))
    else:
        print(colored(f'Скважность - ошибка, ожидалось {expPwm}, получено {realPwm}', 'red'))
elif task == '3.2':
    expected = input('Введите через запятую(без пробелов) ожидаемую последовательность значений:')
    expected = expected.split(',')
    error = False
    for i in range(0, len(expected)):
        temp = int((ser.read(5).decode('utf-8').replace('\n\r', '')))
        if int(expected[i]) == int(temp):
            print(colored(f'Значение {i+1} - верно', 'green'))
        else:
            print(colored(f'Частота - ошибка, ожидалось {expected[i]}, получено {temp}', 'red'))
            error = True
    if not error:
        print(colored('Тест пройден', 'green'))
    else:
        print(colored('Тест провален', 'red'))
elif task == '3.3':
        data = ''
        expPeriod = int(input('Введите период(сек): '))
        expFreq = 1/expPeriod
        while end not in data:
            data += (ser.read().decode('utf-8'))
        data = data.split(end)[0]
        strPsc, strArr = data.split('<>')
        psc = int(strPsc)
        arr = int(strArr)
        realFreq = mkFreq/((arr+1)*(psc+1))
        realPeriod = 2/realFreq
        if expPeriod == realPeriod:
            print(colored('Тест пройден', 'green'))
        else:
            print(colored(f'Период - ошибка, ожидалось {expPeriod}, получено {realPeriod}', 'red'))

