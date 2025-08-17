# CPP-Terminal-Database
Terminal Database Manager is a C++ OOP project where the user can manage simple databases inside a terminal window. The app's UI is simple, the user navigates through the menus using the number keys, alongside Enter for confirmation or cancellation.
The UI look can be changed, from the border decorator to text alignment. *Somewhat annoying* Sound can be added to user input in the settings.

The databases so far, which can be managed by user with Create, Load, Save and Delete commands have Item ID, Name, Type and Quantity. There CAN be duplicates, as each item gets its own unique ID inside the database at creation.

This project can be compiled and used on Linux & Windows

## Compilation on Linux

To compile on Linux, after cloning the repo, the user has to install ncurses and CMake.
Afterwards, simply use the CMake command:

`cmake --preset linux`

Then navigate into the build folder and run the build command:

`cmake --build .`

This will create the app executable in the root folder. From here, simply go out of the build folder and run the app:

`cd ..`

`./TerminalDatabaseApp`

## Compilation on Windows

To compile on Windows, the user *MUST* install Visual Studio build tools for C++ development. 

Afterwards, in the folder of choice, install vcpkg using the terminal. The process is straight forward:

1. Clone the repo:            `git clone https://github.com/microsoft/vcpkg.git`

2. Navigate into the folder:  `cd vcpkg`

3. Run Bootstrap:             `.\bootstrap-vcpkg.bat`

After this, simply use vcpkg to install PDCurses:

`.\vcpkg install pdcurses`

After installing PDCurses, the last thing is to add vcpkg to path. Make a new environment variable called VCPKG_ROOT and add to it the path to vcpkg root folder.

The last steps are similar to the Linux method. 

Go into the root folder of the project and run the CMake command with the windows preset

`cmake --preset windows`

Navigate into the build folder and run the build command

`cd build`

`cmake --build .`

This will create the app executable and dependencies in the root folder. From here, simply go out of the build folder and run the app:

`cd ..`

`.\TerminalDatabaseApp.exe`
