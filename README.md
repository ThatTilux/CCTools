# CCTools

The CCTools Library in C++ extends the [RAT-Library](https://gitlab.com/Project-Rat) by providing tools to facilitate the workflow of programatically doing computations on canted-cosine-theta (CCT) magnet models defined in JSON. 
This library can be used for CCT magnet models in the JSON format created using the [RAT-GUI](https://rat-gui.com/index.html) or [RAT-Library](https://rat-gui.com/library.html).
Special focus is on manipulating the so-called custom CCT harmonics of the CCT magnet.

This project is part of the FCC-ee HTS4 research project at CERN.

## Features

### Main Classes

- **ModelHandler**: Provides functions to manipulate a magnet model JSON file, including extracting and modifying drive values for custom CCT harmonics.

- **ModelCalculator**: Runs computations on a magnet model, including harmonics and mesh calculations, providing results through simplified handler classes.

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
- When a CUDA compatible GPU is available, make sure to follow the steps for installing CUDA. This is highly recommended as it reduces the runtime of this software by some orders of magnitude.
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
Email: [tim.ole.kuhlmann@cern.ch](mailto:tim.ole.kuhlmann@cern.ch)  
GitHub: [ThatTilux](https://github.com/ThatTilux)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
