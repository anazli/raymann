# raymann
Under Construction...

Building
--------
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
To build and run the test:
```bash
$ cmake .. -DBUILD_TESTING=ON
$ make
$ make test
```
To run the program
```bash
$ cd ..
$ bin/raymann
```

To run the tests
```bash
$ test/bin/Test...
```
