# Super Mario Clone

Super Mario Clone using C++ with SDL2.0.

Used ECS architecture: https://github.com/redxdev/ECS

![Game screenshot](1.png)
![Game screenshot](2.png)
![Game screenshot](3.png)
![Game screenshot](4.png)


## Project Rubric Points

#### Loops, Functions, I/O
- The project reads data from a file and process the data, or the program writes data to a file.
    - [x] The project [reads data from an external file] or writes data to a file as part of the necessary operation of the program.
      - In [renderer.cpp](src/renderer.cpp) line 108 on, external image files are loaded into the game.
 
- The project accepts user input and processes the input.
    - [x] The project accepts input from a user as part of the necessary operation of the program.
      - This can be seen in the `Controller::HandleInput()` function in [controller.cpp](src/controller.cpp)

#### Object Oriented Programming

- The project uses Object Oriented Programming techniques.
    - [x] The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
      - An example of this is the newly defined [terrain.h](src/terrain.h) file which is a class representing the terrain.

- Classes use appropriate access specifiers for class members.
    - [x] All class data members are explicitly specified as public, protected, or private.

- Derived class functions override virtual base class functions.
    - One member function in an inherited class overrides a virtual base class member function.

- Templates generalize functions in the project.
    - One function is declared with a template that allows it to accept a generic parameter.


#### Memory Management

- The project uses move semantics to move data, instead of copying it, where possible.
    - For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.

- The project uses smart pointers instead of raw pointers.
    - The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.


## Dependencies

```
 cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer bz2
```


```

sudo apt install libsdl2-dev libsdl2-2.0-0 -y


sudo apt install libjpeg-dev libwebp-dev libtiff5-dev libsdl2-image-dev libsdl2-image-2.0-0 -y


sudo apt install libmikmod-dev libfishsound1-dev libsmpeg-dev liboggz2-dev libflac-dev libfluidsynth-dev libsdl2-mixer-dev libsdl2-mixer-2.0-0 -y


sudo apt-get install libbz2-dev
```

## Build Instructions
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`.
3. Compile: `cmake .. && make`.
4. run game `./mario` inside `bin` directory.
 


## Game Controls

- Move:  `Up - Down - Left - Right` 
- Jump: `Space`
- Run `LCtrl` 





