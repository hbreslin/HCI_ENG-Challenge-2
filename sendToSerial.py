import serial
# import re
import tkinter as tk

# Arduino/serial setup!
arduino = serial.Serial(
    port='/dev/cu.usbmodem1101',
    baudrate=9600,
    timeout=0.1
)

root = tk.Tk()
root.title("Temperature & Humidity Monitor")


def get_serial_input():
    try:
        # get the response and trim it to the integer values
        response = arduino.readline().decode().strip()
        # if response is not None and not response.isspace():
        print(response)
        values = response.split(" ")

        # if getting temperature and humidity input
        if len(values) >= 4:
            cl = f"Current Temperature: {values[0]}°C // Ideal : {values[2]}°C"
            ch = f"Current Humidity: {values[1]}% // Ideal : {values[3]}%" 
            temp_label.config(text=cl)
            humid_label.config(text=ch)

        # if getting status
        if response and len(values)==0:
            status_label.config(text=response)


    # in case ...
    except Exception as e:
        print(e)

    root.after(100, get_serial_input)

def send_temp():
    # send new target temp 
    value = temp_entry.get()
    # print("sending new temp")
    arduino.write(f"T:{value}\n".encode())
    temp_entry.delete(0,'end')


def send_humidity():
    # send new target humidity
    # print("sending new humidity")
    value = humid_entry.get()
    arduino.write(f"H:{value}\n".encode())
    humid_entry.delete(0, 'end')

current_readings_frame = tk.Frame(root)
current_readings_frame.pack(side=tk.TOP)

set_values_frame = tk.Frame(root)
set_values_frame.pack(side=tk.BOTTOM)



# TKinter GUI stuff
temp_label = tk.Label(current_readings_frame, text="Current Temperature: ", font=("Arial", 10))
temp_label.pack(side=tk.TOP)

humid_label = tk.Label(current_readings_frame, text="Current Humidity: ", font=("Arial", 10))
humid_label.pack(side=tk.BOTTOM)

temp_frame = tk.Frame(set_values_frame)
temp_frame.pack(side=tk.LEFT)
temp_entry = tk.Entry(temp_frame, width=10)
temp_entry.pack()
tk.Button(temp_frame, text="Set Ideal Temperature", command=send_temp, font=("Arial", 7)).pack(pady=5)

humid_frame = tk.Frame(set_values_frame)
humid_frame.pack(side=tk.RIGHT)
humid_entry = tk.Entry(humid_frame, width=10)
humid_entry.pack()
tk.Button(humid_frame, text="Set Ideal Humidity", command=send_humidity, font=("Arial", 7)).pack(pady=5)

status_label = tk.Label(root, text="", wraplength=300)
status_label.pack(pady=10)

# need to use the TK loop
root.after(100, get_serial_input)
root.mainloop()
