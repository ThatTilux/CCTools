#include "Logger.hh"
#include "model_handler.h"
#include "harmonic_drive_parameter.h"
#include "model_calculator.h"

/**
 * This is a simple example to demonstrate the usage the main library components: ModelHandler and ModelCalculator.
 * If you do not want to or cannot run the code yourself, you can find the output of the following code at the end of this file.
 */
int main() {
    // Create a ModelHandler object to handle the JSON file. Path is relative to the build folder.
    Logger::info("Now creating a ModelHandler object...");
    ModelHandler model_handler("../examples/cct.json");

    // Get the harmonic drive values for the B components
    Logger::info("Now getting the harmonic drive values for the B components...");
    HarmonicDriveParameterMap param_map_B = model_handler.getHarmonicDriveValues("B");

    // Print the drive values
    Logger::info("Printing the harmonic drive values for the B components...");
    // 'name' has type std::string and 'param' has type HarmonicDriveParameters
    for (auto [name, param] : param_map_B) {
        // to_string function from harmonic_drive_parameter.h
        Logger::info(name + ": " + to_string(param));
    }

    // Override the B1 drive values. B1 has an amplitude of constant, so we need to set a corresponding constant value
    Logger::info("Now setting the harmonic drive values for B1...");
    // Set it to 5e-05
    HarmonicDriveParameters params_B1(5e-05, HarmonicDriveParameterType::Constant);
    // Option 1:
    model_handler.setHarmonicDriveValue("B1", params_B1);
    // Option 2:
    HarmonicDriveParameterMap new_param_map_B;
    new_param_map_B["B1"] = params_B1;
    model_handler.apply_params(new_param_map_B);
    // Both options do the same thing. The second one is useful when multiple parameters need to be set at once.

    // Get the updated harmonic drive values for the B components to see the change
    Logger::info("Now getting the updated harmonic drive values for the B components...");
    param_map_B = model_handler.getHarmonicDriveValues("B");
    for (auto [name, param] : param_map_B) {
        Logger::info(name + ": " + to_string(param));
    }

    // The changes above are only made to the temporary JSON file. The original file remains untouched.

    // Do some computations now. For this, we need a ModelCalculator object.
    Logger::info("Now creating a ModelCalculator object...");
    // The model calculator needs the path to the JSON file. We can use the temporary JSON file path from the ModelHandler object.
    // This way, the ModelCalculator will use the updated JSON file when we make changes.
    ModelCalculator model_calculator(model_handler.getTempJsonPath());

    // The model has a harmonics calculation and mesh calculation in the calculation tree.
    // We will run the harmonics calculation as an example.
    Logger::info("Now running the harmonics calculation...");
    // Provide a HarmonicsDataHandler object. The ModelCalculator will store the results in that object.
    HarmonicsDataHandler harmonics_handler;
    // Run the harmonics calculation. It will use the GPU when available and RAT has been installed with CUDA support.
    model_calculator.calc_harmonics(harmonics_handler);

    // Access the results in the harmonics handler now.
    // The harmonics handler provides various info from the calculation, e.g., the b_n values
    Logger::info("Printing the b_n values...");
    // b_n values are stored in a vector
    std::vector<double> bn_values = harmonics_handler.get_bn();
    for (size_t i = 0; i < bn_values.size(); i++) {
        Logger::info("b_" + std::to_string(i + 1) + ": " + std::to_string(bn_values[i]));
    }

    // Change the B1 value again to see the results change
    Logger::info("Now setting the harmonic drive values for B1 again...");
    // Set it to 1e-04
    HarmonicDriveParameters params_B1_new(1e-04, HarmonicDriveParameterType::Constant);
    model_handler.setHarmonicDriveValue("B1", params_B1_new);

    // Run the harmonics calculation again
    Logger::info("Now running the harmonics calculation again...");
    // Note: whenever we change the JSON file, we need to reload the model calculator to use the updated file
    // This would yield the same results as before: model_calculator.calc_harmonics(harmonics_handler);
    // The reload_and_calc_harmonics function reloads the JSON file and runs the harmonics calculation:
    model_calculator.reload_and_calc_harmonics(model_handler.getTempJsonPath(), harmonics_handler);

    // Print the new b_n values
    Logger::info("Printing the updated b_n values...");
    bn_values = harmonics_handler.get_bn();
    for (size_t i = 0; i < bn_values.size(); i++) {
        Logger::info("b_" + std::to_string(i + 1) + ": " + std::to_string(bn_values[i]));
    }

    return 0;
}


// If you do not want to or cannot run the code yourself, this is the output of the code:
/*
[2024-09-16 16:22:04] [info] Now creating a ModelHandler object...
[2024-09-16 16:22:04] [info] Now getting the harmonic drive values for the B components...
[2024-09-16 16:22:04] [info] Printing the harmonic drive values for the B components...
[2024-09-16 16:22:04] [info] B9: Constant: 0
[2024-09-16 16:22:04] [info] B8: Constant: 0
[2024-09-16 16:22:04] [info] B7: Constant: 0
[2024-09-16 16:22:04] [info] B5: Constant: 0
[2024-09-16 16:22:04] [info] B3: Constant: 0
[2024-09-16 16:22:04] [info] B10: Constant: 0
[2024-09-16 16:22:04] [info] B1: Constant: 0
[2024-09-16 16:22:04] [info] B6: Constant: 0
[2024-09-16 16:22:04] [info] B4: Constant: 0
[2024-09-16 16:22:04] [info] Now setting the harmonic drive values for B1...
[2024-09-16 16:22:04] [info] Now getting the updated harmonic drive values for the B components...
[2024-09-16 16:22:04] [info] B9: Constant: 0
[2024-09-16 16:22:04] [info] B8: Constant: 0
[2024-09-16 16:22:04] [info] B7: Constant: 0
[2024-09-16 16:22:04] [info] B5: Constant: 0
[2024-09-16 16:22:04] [info] B3: Constant: 0
[2024-09-16 16:22:04] [info] B10: Constant: 0
[2024-09-16 16:22:04] [info] B1: Constant: 5e-05
[2024-09-16 16:22:04] [info] B6: Constant: 0
[2024-09-16 16:22:04] [info] B4: Constant: 0
[2024-09-16 16:22:04] [info] Now creating a ModelCalculator object...
[2024-09-16 16:22:04] [info] Found Harmonics Calculation with the name: Cylyndrical Harmonics
[2024-09-16 16:22:04] [info] Found Mesh Calculation with the name: Mesh
[2024-09-16 16:22:04] [info] Found Axis Path with the name: Axis
[2024-09-16 16:22:04] [info] Now running the harmonics calculation...
[2024-09-16 16:22:04] [info] Running harmonics calculation...
[2024-09-16 16:22:04] [info] No GPU available for Cylyndrical Harmonics. Running on CPU.
[2024-09-16 16:22:11] [info] Printing the b_n values...
[2024-09-16 16:22:11] [info] b_1: -75.684898
[2024-09-16 16:22:11] [info] b_2: 10000.000000
[2024-09-16 16:22:11] [info] b_3: -5.615904
[2024-09-16 16:22:11] [info] b_4: 1.119787
[2024-09-16 16:22:11] [info] b_5: -0.145654
[2024-09-16 16:22:11] [info] b_6: 0.137884
[2024-09-16 16:22:11] [info] b_7: -0.053150
[2024-09-16 16:22:11] [info] b_8: 0.036596
[2024-09-16 16:22:11] [info] b_9: -0.004707
[2024-09-16 16:22:11] [info] b_10: 0.001449
[2024-09-16 16:22:11] [info] Now setting the harmonic drive values for B1 again...
[2024-09-16 16:22:11] [info] Now running the harmonics calculation again...
[2024-09-16 16:22:11] [info] Running harmonics calculation...
[2024-09-16 16:22:18] [info] Printing the updated b_n values...
[2024-09-16 16:22:18] [info] b_1: -109.476886
[2024-09-16 16:22:18] [info] b_2: 10000.000000
[2024-09-16 16:22:18] [info] b_3: -5.692021
[2024-09-16 16:22:18] [info] b_4: 1.119389
[2024-09-16 16:22:18] [info] b_5: -0.147522
[2024-09-16 16:22:18] [info] b_6: 0.137885
[2024-09-16 16:22:18] [info] b_7: -0.053204
[2024-09-16 16:22:18] [info] b_8: 0.036604
[2024-09-16 16:22:18] [info] b_9: -0.004729
[2024-09-16 16:22:18] [info] b_10: 0.001446
*/