# CCTools
The CCTools Library in C++ serves as an interface between the [RAT-Library](https://gitlab.com/Project-Rat) (the underlying library of the closed-source licensed [RAT-GUI](https://rat-gui.com/index.html) software) and any C++ software performing computations on canted-cosine-theta (CCT) magnet models defined in JSON. CCTools provides user-friendly classes that facilitate the usage of the RAT-Library. This library has two main functionalities, as presented in the following.

### JSON File Manipulation
Magnet model files created using the RAT-GUI software are saved in the JSON format. 
CCTools is intended to work with these serialized JSON magnet model files. However, the RAT-Library does not provide seamless user-friendly functionalities to manipulate _existing_ magnet model files. Rather, it focuses on creating new magnet models programmatically from scratch. 
To address this, the CCTools library provides user-friendly functionality through the `ModelHandler` class to programmatically manipulate JSON magnet models by editing the underlying JSON file.


### Simulation Handlers
In the RAT-GUI, results of any simulation are presented in a convenient fashion. The RAT-Library does not provide these convenient results but the raw data resulting from the simulation. 
To facilitate the use of the RAT-Library, CCTools provides the `ModelCalculator` class to run any simulation. The results are returned in user-friendly handler classes, e.g., as a `CCTools::HarmonicsDataHandler` object for the result of a harmonics calculation. This object provides functions to retrieve the same information displayed in the RAT-GUI, e.g., `HarmonicsDataHandler::get_bn` to retrieve the vector of the 10 bn values from a harmonics calculation.

This project is part of the FCC-ee HTS4 research project at CERN.

## Documentation
The documentation of this library can be viewed [here](https://thattilux.github.io/CCTools/).
Additionally, example code showcasing core functionalities is located at `examples/example.cpp`.

More sophisticated examples can be found in the repository [CCT-Harmonics-Optimizer](https://github.com/ThatTilux/CCT-Harmonics-Optimizer), a project within our group utilizing this library. 

## Prerequisites
**Note: This software has only been tested on Linux.**

Before using CCTools, ensure that your system meets the following requirements.

#### System Packages

1. **Google Test (gtest) Library**:
   - Google Test is a framework for C++ tests.
   ```sh
   sudo apt-get install libgtest-dev
   ```

2. **spdlog**:
   - spdlog is a fast C++ logging library.
   You can install `spdlog` by cloning the repository and installing it manually:
   ```sh
   git clone https://github.com/gabime/spdlog.git
   cd spdlog
   mkdir build && cd build
   cmake ..
   make -j
   sudo make install
   ```

#### RAT-Library
Follow the [RAT documentation](https://gitlab.com/Project-Rat/rat-documentation) to install the RAT library. This may take a while.
- When a CUDA-compatible GPU is available, make sure to follow the steps for installing CUDA. This is highly recommended as it reduces the runtime of this software by some orders of magnitude.
- When using CUDA with WSL, it is advised to increase the allocated memory for WSL, see [here](https://geronimo-bergk.medium.com/optimizing-wsl2-performance-setting-memory-and-cpu-allocation-on-windows-513eba7b6086).


## Installation
### Using CCTools in your project

To incorporate CCTools into your C++ project, follow these steps. Make sure you have followed the **Prerequisites** section.

1. **Include CCTools in `CMakeLists.txt`:**

   Use CMake's `FetchContent` module to fetch and include CCTools.

   ```cmake
   include(FetchContent)

   FetchContent_Declare(
     CCTools
     GIT_REPOSITORY https://github.com/ThatTilux/CCTools.git
     GIT_TAG        main
   )

   FetchContent_MakeAvailable(CCTools)
   ```

2. **Link CCTools to Your Executable:**

   In your `CMakeLists.txt`, link CCTools to your target executable.

   ```cmake
   target_link_libraries(your_executable PRIVATE CCTools)
   ```

3. **Include Directories (if necessary):**

   Ensure that the include directories are set correctly.

   ```cmake
   target_include_directories(your_executable PRIVATE ${CCTools_INCLUDE_DIRS})
   ```


### Building CCTools Standalone

Please note that this step is  **not required** to use this library in your C++ project. If you wish to build and test CCTools independently, follow these steps.

1. **Clone the Repository and Build the Project:**
    ```sh
    git clone <repository_url>
    cd <repository_directory>
    mkdir build
    cd build
    cmake ..
    make -j
    ```

2. **Run Tests:**
    ```sh
    ./bin/cctools_test
    ```

3. **Run the Example:**

   There is an example file `examples/example.cpp` showcasing how to use core features of this library.
   ```sh
    ./bin/cctools_example
    ```

## Author

Ole Kuhlmann  
Email: [ole.kuhlmann@rwth-aachen.de](mailto:ole.kuhlmann@rwth-aachen.de)  
GitHub: [ThatTilux](https://github.com/ThatTilux)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
