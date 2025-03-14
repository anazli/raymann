# raymann
A Ray Tracer. 
Still under construction...

Current Scene
-------------
![alt text](https://github.com/anazli/raymann/blob/main/scenes/scene.jpg?raw=true)

Building
--------
```bash
$ cmake -S . -B build -DBUILD_TESTING=OFF
$ cmake --build build
```
To run all tests:
```bash
$ cmake -S . -B build -DBUILD_TESTING=ON
$ cmake --build build
$ ctest --test-dir build
```
Running
-------
To run the program, from the main directory
```bash
$ bin/raymann
```
Docker
------
To build the Docker container run
```bash
$ docker build -t raymann .
```
This will build the container with the tag raymann and also start running it. 
To run the container with interactive shell run
```bash
$ docker run -it -u guest raymann bash --login
```
You can also run the program inside the container.
To copy the image created in the container to a local host-folder, start the container first interactively as above,
open a new terminal, find the container ID with
```bash
$ docker ps | grep raymann
```
and copy it with
```bash
$ docker cp [container-id]:/raymann/scenes/scene.ppm [destination]
```
