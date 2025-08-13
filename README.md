# CPP-Terminal-Database
Terminal Database Manager is a C++ OOP project where the user can manage simple databases inside a terminal window. The app's UI is simple, the user navigates through the menus using the number keys, alongside Enter for confirmation or cancellation.
The UI look can be changed, from the border decorator to text alignment. *Somewhat annoying* Sound can be added to user input in the settings.

The databases so far, which can be managed by user with Create, Load, Save and Delete commands have Item ID, Name, Type and Quantity. There CAN be duplicates, as each item gets its own unique ID inside the database at creation.

This project can be compiled and used on Linux & Windows

## Compilation on Linux

To compile on Linux, after cloning the repo, the user has to install ncurses and CMake.
Afterwards, create a build folder from project root and run the CMake command:

`mkdir build`             -> Create the folder

`cmake -S src -B build`   -> Tell CMake to use the src folder to build the app:

Then navigate into the build folder and run the build command:

`cmake --build .`

This will create the app executable in the root folder. From here, simply go out of the build folder and run the app:

`cd ..`

`./TerminalDatabaseApp`

## Compilation on Window

To compile on Windows, the user *MUST* install Visual Studio build tools for C++ development. 

Afterwards, in the folder of choice, install vcpkg using the terminal. The process is straight forward:

1. Clone the repo:            `git clone https://github.com/microsoft/vcpkg.git`

2. Navigate into the folder:  `cd vcpkg`

3. Run Bootstrap:             `.\bootstrap-vcpkg.bat`

After this, simply use vcpkg to install PDCurses:

`.\vcpkg install pdcurses`

The last steps are similar to the Linux method. 

Go into the root folder of the project and create a build folder and use the CMake command:

`mkdir build`

`cmake -B build -S src -DCMAKE_TOOLCHAIN_FILE="*path/to/vcpkg*/scripts/buildsystems/vcpkg.cmake"`

Navigate into the build folder

`cd build`

And run the build commad

`cmake --build .`

This will create the app executable and dependencies in the root folder. To run the app simply go out of the build folder and use the command:

`cd ..`

`.\TerminalDatabaseApp.exe`
