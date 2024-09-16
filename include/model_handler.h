#ifndef MODEL_HANDLER_H
#define MODEL_HANDLER_H

#include <string>
#include <vector>
#include <regex>
#include <map>
#include <boost/filesystem.hpp>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <stdexcept>



/**
 * @brief Enum class representing harmonic drive parameter types for custom cct harmonics.
 * 
 * This enum class is used to represent the type of a harmonic drive parameter. 
 * For custom cct harmonics with an amplitude of constant, this value can only be constant.
 * For custom cct harmonics with an amplitude of linear, this value can be offset or slope.
 */

enum class HarmonicDriveParameterType {
    Offset,
    Slope,
    Constant
};

// Forward declaration of HarmonicDriveParameters
struct HarmonicDriveParameters;

// Type definition for the unordered map
// Keys have format "B1" to "B10"
using HarmonicDriveParameterMap = std::unordered_map<std::string, HarmonicDriveParameters>;

/**
 * @class ModelHandler
 * @brief Class to manipulate a magnet model JSON file.
 * 
 * Provides functions to extract and manipulate drive values for custom cct harmonics.
 */
class ModelHandler {
public:

    /**
     * @brief Construct a new ModelHandler object.
     * @param json_file_path Path to the JSON file.
     * 
     * This constructor copies the JSON file to a temporary folder. All changes inside this class are only made to the temporary file, the original file remains untouched.
     * Path to the temporary JSON file can be accessed using `getTempJsonPath()`.
     */
    ModelHandler(const boost::filesystem::path& json_file_path);

    /**
     * @brief Dummy constructor. Use the parameterized constructor instead.
     */
    ModelHandler();

    /**
     * @brief Get all harmonic drive values from the JSON file.
     * @param prefix Prefix for the custom cct harmonic names.
     * @return HarmonicDriveParameterMap Map of harmonic drive values.
     * 
     * This function extracts the harmonic drive values and their type from the JSON file.
     * Only extracts drives from custom cct harmonics whose name starts with the prefix.
     */
    HarmonicDriveParameterMap getHarmonicDriveValues(const std::string& prefix = "B");

    /**
     * @brief Apply a harmonic drive value in the JSON file.
     * @param name Name of the custom cct harmonic.
     * @param params Harmonic drive parameters.
     * 
     * This function updates a harmonic drive value for a custom cct harmonic in the JSON file. When multiple custom harmonics with the `name` are found, 
     * all are updated with the new `params`.
     */
    void setHarmonicDriveValue(const std::string &name, const HarmonicDriveParameters &params);

    /**
     * @brief Apply a set of parameters to the model.
     * @param paramMap Map of harmonic drive parameters.
     * 
     * This function applies a set of parameters to the model. The keys of the map should be the names of the custom cct harmonics.
     */
    void apply_params(const HarmonicDriveParameterMap &paramMap);

    /**
     * @brief Get the file path of the temporary model file.
     * @return boost::filesystem::path Path to the temporary JSON file.
     * 
     * This function returns the path to the temporary JSON file where all the changes are made. The original file remains untouched.
     */
    boost::filesystem::path getTempJsonPath() const;

private:

    /**
     * @brief Create a temporary folder and copy the JSON file to it.
     * @param json_file_path Path to the JSON file.
     * 
     * This function creates a temporary folder and copies the JSON file to it. The path to the temporary JSON file can be accessed using `getTempJsonPath()`.
     */
    void createTemporaryFolder(const boost::filesystem::path& json_file_path);

    /**
     * @brief Parse the JSON file and extract harmonic drive values.
     * @param root JSON root object.
     * @param harmonics_map Map to store the harmonic drive values.
     * @param prefix Prefix for the custom cct harmonic names.
     * @param suffix_regex Regex to match the suffix of the custom cct harmonic names.
     * 
     * This helper function recursively parses the JSON file and extracts the harmonic drive values for custom cct harmonics whose name starts with the prefix.
     * The suffix of the custom cct harmonic names should match the regex.
     */
    void parseHarmonicDrive(const Json::Value &root, HarmonicDriveParameterMap &harmonics_map, const std::string &prefix = "B", const std::regex &suffix_regex = std::regex(R"(\d{1,2})")); // suffix means 1 or 2 numbers
    
    /**
     * @brief Update a harmonic drive value in the JSON file.
     * @param root JSON root object.
     * @param name Name of the custom cct harmonic.
     * @param type Type of the harmonic drive parameter.
     * @param value New value for the harmonic drive parameter.
     * @throws std::runtime_error If the type of the harmonic drive parameter does not match the type of the custom cct harmonic.
     * 
     * This helper function recursively parses the JSON file and updates the harmonic drive value for all custom cct harmonics with the matching name in the JSON file.
     * If a matching harmonic does not meet the type requirement, an error is thrown.
     */
    void updateHarmonicDrive(Json::Value &root, const std::string &name, const HarmonicDriveParameterType &type, double value);

    /**
     * @brief Path of the folder storing the temporary JSON file.
     */
    boost::filesystem::path temp_folder_;

    /**
     * @brief Path of the temporary JSON file.
     */
    boost::filesystem::path temp_json_path_;
};

#endif // MODEL_HANDLER_H