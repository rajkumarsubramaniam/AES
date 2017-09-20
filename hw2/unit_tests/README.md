# cmocka unit testing for ECEN 5013 - HW2

* [cmocka](https://cmocka.org/)

* cmocka [API](https://api.cmocka.org/index.html)

cmocka is included in this repository as a git subtree. See
3rd-party/README.md for details.

## Requirements

* cmake - available through most package managers.

## Build

* building cmocka:

    ```SHELL
    make cmocka
    ```

    This will go into the 3rd-party libraries directory and call cmake
    correctly to build cmocka.

* building and running tests:

    ```SHELL
    make test
    ```
    
* building main executable

    ```SHELL
    make all
    ```
    
* cleaing up local build artifacts:

    ```SHELL
    make clean
    ```
    
* removing all generated files, including 3rd-party builds

    ```SHELL
    make clobber
    ```
    
## Source

Included source files are:

* circbuff, doublyLinked.{h|c} - The source files for circular buffer and linked list code

* testCB.c, testDL.c - unit tests for circular buffer and linked list

Run make test - to execute circular buffer test cases
Run make test SELECT=DL - to execute double linked list test cases

