import serial
from termcolor import colored

com = input('Введите компорт:')
ser = serial.Serial(com if len(com) != 0 else 'COM4', 115200)
task = input('Введите номер задания:')
DIGITAL = {
    '0': '1111110',
    '1': '0110000',
    '2': '1101101',
    '3': '1111001',
    '4': '0110011',
    '5': '1011011',
    '6': '1011111',
    '7': '1110000',
    '8': '1111111',
    '9': '1111011',
    'A': '1110111',
    'B': '0011111',
    'C': '1001110',
    'D': '0111101',
    'E': '1001111',
    'F': '1000111'
}

if task == '2.1':
    type = input('Введите тип индикатора(DIGITAL/BCD): ')
    expect = input('Введите ожидаемую последовательность чисел(без пробелов): ')
    for i in range(0, len(expect)):
        if type == 'DIGITAL':
            data = ser.read(size=7).decode('utf-8')
            expected = DIGITAL[expect[i]]
        else:
            data = hex(int(ser.read(size=4).decode('utf-8'), 2))
            expected = f'0x{expect[i].lower()}'
        if data != expected:
            print(colored(f'Тест провален, ожидалось - "{expected}", получено - "{data}"', 'red'))
            exit(-1)
        else:
            print(colored(f'Число {i+1} - верно', 'green'))
    print(colored('Тест пройден', 'green'))
    exit(0)
elif task == '2.2':
    lines = input("Введите количество задействованных строк:")
    print("Запустите симуляцию в Proteus")
    for i in range(int(lines)):
        line = input(f"Введите строку {i+1}:")
        data = ser.read(size=len(line)).decode('utf-8')
        if data != line:
            print(colored(f'Тест провален, ожидалось - "{line}", получено - "{data}"', 'red'))
            exit(-1)
        else:
            print(colored(f'Строка {i + 1} - верно', 'green'))
    print(colored('Тест пройден', 'green'))
    exit(0)
elif task == '2.3':
    answer = input('Введите ожидаемую последовательность символов: ')
    data = ser.read(size=(len("LAST PRESSED KEY - "))).decode('utf-8')
    if data == "LAST PRESSED KEY - ":
        for i in range(0, len(answer)):
            answerASCII = str(ord(answer[i]))
            data = ser.read(size=(len(answerASCII))).decode('utf-8')
            if data != answerASCII:
                print(colored(f'Тест провален, ожидалось - "{answerASCII}", получено - "{data}"', 'red'))
                exit(-1)
            else:
                print(colored(f'Символ {i+1} - верно', 'green'))

        print(colored('Тест пройден', 'green'))
        exit(0)
    else:
        print(colored(f'Тест не пройден, не удалось получить сообщение "LAST PRESSED KEY - "', 'red'))
        exit(-1)
elif task == '2.4':
    math = input('Введите математическое выражение:')
    expect = eval(math.replace('x','*'))
    expectError = input('Ожидается ошибка(Да/пропустить):')
    expect = "ERROR" if expectError else str(expect)
    mathAnswer = ser.read(size=(len(math))).decode('utf-8')
    answer = ser.read(size=(len(expect))).decode('utf-8')
    if math == mathAnswer:
        print(colored('Выражение записано верно', 'green'))
        if expect == answer:
            print(colored('Тест пройден', 'green'))
            exit(0)
        else:
            print(colored(f'Тест провален, ожидалось - "{expect}", получено - "{answer}"', 'red'))
            exit(-1)
    else:
        print(colored(f'Тест не пройден, ожидаемое выражение не совпало, ожидалось {math} получено - "{mathAnswer}"', 'red'))
        exit(-1)
