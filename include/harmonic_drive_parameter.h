#ifndef HARMONIC_DRIVE_PARAMETER_H
#define HARMONIC_DRIVE_PARAMETER_H

#include <variant>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include "model_handler.h"

namespace CCTools {

// Forward declaration of the HarmonicDriveParameterType enum
enum class HarmonicDriveParameterType;

/**
 * @struct HarmonicDriveParameters
 * @brief Struct to store the harmonic drive parameter(s) of one custom cct harmonic.
 * 
 * One custom cct harmonic can have one or more of these drive parameters, depending on the amplitude attribute. E.g., a harmonic with amplitude linear has an offset and a slope parameter.
 */
struct HarmonicDriveParameters {
    // Enum to track the type of parameter

    /**
     * @enum Type
     * @brief Enum to track the type of the harmonic drive parameter(s) of one cct harmonic.
     */
    enum class Type {
        Offset,
        Slope,
        Constant,
        OffsetAndSlope,
        Undefined
    };

    // Constructors for different parameter types

    /**
     * @brief Dummy constructor.
     * 
     * This constructor initializes the HarmonicDriveParameters object with undefined values that can be set using the setter functions.
     */
    HarmonicDriveParameters();

    /**
     * @brief Constructor for offset and slope parameters.
     * @param offset Offset value.
     * @param slope Slope value.
     * 
     * This constructor initializes the HarmonicDriveParameters object with offset and slope values, e.g., for a cct harmonic with an amplitude of linear.
     */
    HarmonicDriveParameters(double offset, double slope);

    /**
     * @brief Constructor for a single parameter type.
     * @param value Value of the parameter.
     * @param t Type of the parameter.
     * 
     * This constructor initializes the HarmonicDriveParameters object with a single parameter value and type, e.g., for a cct harmonic with an amplitude of constant.
     */
    HarmonicDriveParameters(double value, HarmonicDriveParameterType t);

    // Methods to GET values

    /**
     * @brief Get the constant value.
     * @return Constant value.
     * @throws std::logic_error If the parameter is not a constant parameter.
     * 
     * This function returns the constant value of the parameter. Throws an error if the parameter is not a constant parameter.
     */
    double getConstant() const;

    /**
     * @brief Get the offset value.
     * @return Offset value.
     * @throws std::logic_error If the parameter is not an offset parameter.
     * 
     * This function returns the offset value of the parameter. Throws an error if the parameter is not an offset (or offset and slope) parameter.
     */
    double getOffset() const;

    /**
     * @brief Get the slope value.
     * @return Slope value.
     * @throws std::logic_error If the parameter is not a slope parameter.
     * 
     * This function returns the slope value of the parameter. Throws an error if the parameter is not a slope (or offset and slope) parameter.
     */
    double getSlope() const;

    /**
     * @brief Get the value of a specific type.
     * @param type Type of the parameter.
     * @return Value of the parameter.
     * @throws std::runtime_error If the parameter is not of the specified type.
     * 
     * This function returns the value of the parameter of the specified type. Throws an error if the parameter is not of the specified type.
     */
    double get(HarmonicDriveParameterType type) const;

    // Methods to check the type

    /**
     * @brief Check if the parameter is a constant parameter.
     * @return True if the parameter is a constant parameter, false otherwise.
     */
    bool isConstant() const;

    /**
     * @brief Check if the parameter is an offset parameter.
     * @return True if the parameter is an offset (or offset and slope) parameter, false otherwise.
     */
    bool isOffset() const;

    /**
     * @brief Check if the parameter is a slope parameter.
     * @return True if the parameter is a slope (or offset and slope) parameter, false otherwise.
     */
    bool isSlope() const;

    /**
     * @brief Check if the parameter is an offset and slope parameter.
     * @return True if the parameter is an offset and slope parameter, false otherwise.
     */
    bool isOffsetAndSlope() const;

    /**
     * @brief Check if the parameter is of a specific type.
     * @param t Type to check.
     * @return True if the parameter is of the specified type, false otherwise.
     * 
     * This function checks if the parameter is of the specified type.
     */
    bool isType(HarmonicDriveParameterType t) const;

    // Method to SET values

    /**
     * @brief Set the value of a specific type.
     * @param value New value for the parameter.
     * @param t Type of the parameter.
     * @throws std::runtime_error If the parameter is not of the specified type.
     * 
     * This function sets the value of the parameter of the specified type. Throws an error if the parameter is not of the specified type.
     */
    void setValue(double value, HarmonicDriveParameterType t);

    // Equality operator
    
    /**
     * @brief Check if two HarmonicDriveParameters objects are equal.
     * @param other Other HarmonicDriveParameters object.
     * @return True if the objects are equal, false otherwise.
     * 
     * This function checks if two HarmonicDriveParameters objects are equal. They are considered equal if and only if their type and values are equal.
     */
    bool operator==(const HarmonicDriveParameters& other) const;

private:
    Type type;
    double offset;
    double slope;
    double constant;
};

/**
 * @brief Convert HarmonicDriveParameters to a string.
 * @param params HarmonicDriveParameters object.
 * @return String representation of the HarmonicDriveParameters object.
 * 
 * This function converts a HarmonicDriveParameters object to a string representation.
 */
std::string to_string(const HarmonicDriveParameters &params);

/**
 * @brief Type definition for the HarmonicDriveParameterMap.
 * 
 * This typedefinition defines the HarmonicDriveParameterMap as an unordered map with string keys and HarmonicDriveParameters values.
 * Convention is to use the name of the custom cct harmonic as the string key.
 */
using HarmonicDriveParameterMap = std::unordered_map<std::string, HarmonicDriveParameters>;

} // namespace CCTools

#endif // HARMONIC_DRIVE_PARAMETER_H
