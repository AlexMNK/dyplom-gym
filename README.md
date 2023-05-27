# dyplom-gym
In order to launch aplication, go to `exe` folder. There you can find `client` and `server` folders with executables. Note that in order to run properly, executables need all included `.dll` libraries and folders. Also, for server executable to work, you need `config.ini` file included in a working directory of an executable. Default values in file are: 
[Options]
DBType=QODBC
DBName=Driver={ODBC Driver 17 for SQL Server};server=localhost;database=GymDatabase;trusted_connection=Yes;
ServerPort=1234 

Client side generate `config.ini` itself and do not need it to be created manually. To use application, start a server part first and then launch client executables (as many as you want). Enjoy!
