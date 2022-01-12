# raymann
Under Construction...

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
To run the program, in main directory
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
This will build the container with the tag raymann.
To run the container with interactive shell run
```bash
$ docker run -it -u guest raymann bash --login
```
You can also run the program inside the container.
