# raymann
A Ray Tracer. 
Still under construction...

Current Scene
-------------
![alt text](https://github.com/anazli/raymann/blob/main/scenes/scene.jpg?raw=true)

Building
--------
To build this repository, first clone it and create a build folder.
```bash
$ git clone https://github.com/anazli/raymann.git
$ cd raymann
$ mkdir build
$ cd build
```
Building without tests
```bash
$ cmake .. -DBUILD_TESTING=OFF
$ make
```
To build and run all tests:
```bash
$ cmake .. -DBUILD_TESTING=ON
$ make
$ make test
```
Running
-------
To run the program, from the main directory
```bash
$ cd ..
$ bin/raymann
```

To run a certain testcase, you can run from the main directory its binary located in test/bin, for example the ray tests:
```bash
$ test/bin/TestRay
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
