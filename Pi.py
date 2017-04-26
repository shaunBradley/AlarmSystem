import serial
import threading
import time
ser = serial.Serial('/dev/ttyACM0', 9600);
correctPassword = "123";
armed = True;
armedString = "print(     Armed)\n";
disarmedString = "print(    Disarmed)\n";

while(True):
    passwordAttempt = "";
    while(ser.inWaiting()):
        newByte = ser.read(1);
        if(newByte != '\n'):
            passwordAttempt += newByte;
        else: break;
    time.sleep(1);
    if(passwordAttempt == correctPassword):
        if(armed):
            ser.write(disarmedString);
        else:
            ser.write(armedString);
        armed = not armed;
    elif(passwordAttempt != ""):
        ser.write("print(   try again)\n");
        time.sleep(1);
        ser.write(armedString);
        
ser.close();

    
#thread1 = threading.Thread(target=read_serial);
#thread1.start();
