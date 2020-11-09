# importa a biblioteca serial encontrada no endereço: https://pypi.python.org/pypi/pyserial
import serial
import tkinter as tk

arduino_uno = serial.Serial('COM1', 9600, timeout=2)
print("criou serial")

root = tk.Tk()

canvas1 = tk.Canvas(root, width=400, height=300)
canvas1.pack()

entry1 = tk.Entry(root)
canvas1.create_window(200, 140, window=entry1)

label1 = tk.Label(root, text='Aguardando respostas')
wait_response = False


def delButton():
    label1.destroy()


def sendCommand():
    global label1, wait_response

    cmd = entry1.get()

    arduino_uno.write(cmd.encode())
    print("comando enviado: ", cmd)


send_button = tk.Button(text='Enviar comando', command=sendCommand)
canvas1.create_window(200, 180, window=send_button)

delete_button = tk.Button(text='Delete the label', command=delButton)
canvas1.create_window(200, 210, window=delete_button)

# root.mainloop()

while True:
    root.update()

    buf = arduino_uno.inWaiting()
    if buf > 0:
        response = arduino_uno.readline()
        # response = response_raw[2:][:-3]

        print("resposta:", response)

        if response == '':
            response = 'Sem retorno'

        label1.destroy()
        label1 = tk.Label(root, text=response)
        label1.pack(pady=10)

# fecha comunicaçao
arduino_uno.close()
print("fim")
