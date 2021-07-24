
# Application for the evaluation of performance in a MES system

This project is for the course of Programación en Plataformas abiertas from the
Universidad de Costa Rica. The project consists in a low version of how a MES
system works. MES is a software solution to ensure quality and efficiency in the
indsutry. In here we use an Arduino to count objects with an ultrasonic sensor, the
count will be added to the database every 'x' seconds. Then we use a GUI to define
the setpoint (x) and the theoric value of how many items it should count, also
the GUI works to set a time frame to obtain the KPI associated, the KPI determines
the efficiency of the system.



## How it works

This project has 3 parts: Arduino counter, SQL Data base and a GUI. Each part works
with the other to create the MES system.

### Arduino

The goal of the Arduino is to count objects. This simulates an industry when they need
to know how many objects they are producing, for example a beverage company
where they need to know how many bottles are being produced. For this we use an
ultrasonic sensor (HC-SR04) and we delimit a distance of 10 cm. 

![App Screenshot](https://github.com/Edson0121/ProyectoPlataformas/blob/main/arduino.png)

The file `sensorArduino.c` is the one that needs to be uploaded to the Arduino. This
code will show the count in the Serial Monitor of the Arduino IDE. To export this
count and use it in other files, we use `ReadArduino.h` that read the USB connection
and can be added as a header to any program and read the last value counted.

### SQL Database

To save the values counted by the Arduino and work with them, we use a database.
In here we create two tables, the first one is for devices and for now only includes
one entry, the Arduino. The second table is for the values, we save up the start
date of the count and end date, and it's corresponding count value. With both values
we can get the substraction (Delta) and know how many items are being counted in
a specific time frame given by the setpoint.

First, we have the `runSQL.c` file that creates the Database if it hasn't being
created. After that, we can work with the default values for the setpoint and
theoretical value or change it with the GUI. Then, with the Arduino connected and
running, we can compile and execute the `runScheduler.c` file that will read
the value given by the Arduino and save it in the database with the start and end
time. The `sqlData.h` header contains every function needed to work with the
database, we use this header for the above C files and in the GUI.

### GUI

The goal for the GUI is to offer that interaction with the user to use the
application easier. It consists of two sections, the first one has two spin buttons
for the user to define the setpoint and theoric value and update them.

The second section consists in a series of spin button to define the start 
and end date to obtain the KPI from that time frame. This section is very useful
so the user can know the performance in an hour, day or even a year.

![App Screenshot](https://github.com/Edson0121/ProyectoPlataformas/blob/main/GUI_window.png)
## Installation

This project uses the SQLite anbd GTK library, so first we need to
install them.

```bash
  sudo apt-get install libgtk-3-dev
  sudo apt-get install libsqlite3-dev
```

We also use Glade to create the GUI and a database browser, to install them
we use:

```bash
  sudo apt-get install glade
  sudo apt install sqlitebrowser
```
To download the repository in your computer you just need to clone it and have every
file in the same directory. The only exception is the window_main.glade file that needs
to be in a folder named "glade".


    
## Run Locally

To download the repository in your computer you just need to clone it and have every
file in the same directory. The only exception is the `window_main.glade` file that needs
to be in a folder named "glade".

Clone the project

```bash
  git clone https://github.com/Edson0121/ProyectoPlataformas
```

Go to the project directory

```bash
  cd ProyectoPlataformas
```

Install dependencies, they are GTK, SQLite, Glade and SQL Browser.
To compile the GUI you just need to use the makefile.

```bash
  make
```

If for any reason the makefile doesn't work, you can compile the GUI with the
following command:

```bash
  gcc -o proyecto main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
```
To compile the `runSQL.c` and `runScheduler.c`, use the following commands:

```bash
  gcc -o runSQL runSQL.c -lsqlite3 
  gcc -o runScheduler runScheduler.c -pthread -lsqlite3
```

And to excute them use these commands:

```bash
  ./proyecto
  ./runSQL
  ./runScheduler
```
## Usage

The way this application works is to first run `runSQL.c` to create the database.
After the database is created, you can work either with the default values of
the setpoint (s) and theoric value (upm) or you can use the makefile to compile
the GUI and update these values. After they are updated, close the GUI window and
connect the Arduino. Upload the `sensorArduino.c` code to the Arduino and then run
the `runScheduler.c` file, in here you'll be asked the execution time. The program
will run the given time and save the Arduino's count value every 'X' time defined by
the setpoint, in the database. After the execution time finishes, you can open
the database that will be created in the directory under the name `MES.db`.
You can only open this file if you installed SQLite Browser or any other similar
app. Later, to know the KPI of your process, open again the GUI and set the
start and end date of the time frame and press the update button.
  
## Demo

Demonstration video of the application can be found in this
[link](https://www.youtube.com/watch?v=WIUHj24Jtn8) in Youtube. It is necessary
to clarify this video is in Spanish, but the visuals are helpful enough. 

  
## Authors

- [@Edson0121](https://github.com/Edson0121)
- [@juankobe3](https://github.com/juankobe3)
- [@DCWu123](https://github.com/DCWu123)
- [@yanelysr988](https://github.com/yanelysr988)

  
## License

[MIT](https://choosealicense.com/licenses/mit/)

  
## Feedback

If you have any feedback, please reach out to us at edson_murillo@outlook.com
## Support

For support, email edson_murillo@outlook.com
