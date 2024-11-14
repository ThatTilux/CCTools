#include "model_handler.h"
#include "harmonic_drive_parameter.h"

namespace CCTools
{

    // Constructor
    ModelHandler::ModelHandler(const boost::filesystem::path &json_file_path)
    {
        createTemporaryFolder(json_file_path);
    }

    // dummy constructor
    ModelHandler::ModelHandler()
    {
    }

    // Creates a tempoary folder with a copy of the model. All operations are done on that model
    void ModelHandler::createTemporaryFolder(const boost::filesystem::path &json_file_path)
    {
        temp_folder_ = boost::filesystem::temp_directory_path() / "model_temp";
        if (!boost::filesystem::exists(temp_folder_))
        {
            boost::filesystem::create_directory(temp_folder_);
        }
        temp_json_path_ = temp_folder_ / json_file_path.filename();
        if (boost::filesystem::exists(temp_json_path_))
        {
            boost::filesystem::remove(temp_json_path_);
        }
        boost::filesystem::copy_file(json_file_path, temp_json_path_);
    }

    // Parses the json file and extracts all the scaling function constants/slopes for all the custom cct harmonics
    // Throws an error if there are multiple harmonics with the same name but different constants/slopes
    HarmonicDriveParameterMap ModelHandler::getHarmonicDriveValues(const std::string &prefix)
    {
        std::ifstream file(temp_json_path_.string());
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open JSON file.");
        }

        Json::Value root;
        file >> root;
        file.close();

        HarmonicDriveParameterMap harmonic_drives_map;
        parseHarmonicDrive(root, harmonic_drives_map, prefix);

        return harmonic_drives_map;
    }

    // Updates all the custom cct harmonics' scaling constants/slopes for the given name
    void ModelHandler::setHarmonicDriveValue(const std::string &name, const HarmonicDriveParameters &params)
    {
        std::ifstream file(temp_json_path_.string());
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open JSON file.");
        }

        Json::Value root;
        file >> root;
        file.close();

        // check which parameters are present
        if (params.isConstant())
        {
            updateHarmonicDrive(root, name, HarmonicDriveParameterType::Constant, params.getConstant());
        }
        if (params.isSlope())
        {
            updateHarmonicDrive(root, name, HarmonicDriveParameterType::Slope, params.getSlope());
        }
        if (params.isOffset())
        {
            updateHarmonicDrive(root, name, HarmonicDriveParameterType::Offset, params.getOffset());
        }

        std::ofstream out_file(temp_json_path_.string());
        out_file << root;
        out_file.close();
    }

    // functon to apply set of parameters to the model
    // format: paramMap["B1"] = {offset, slope}
    void ModelHandler::apply_params(const HarmonicDriveParameterMap &paramMap)
    {
        // iterate through parameters and apply them
        for (const auto &parameters : paramMap)
        {
            setHarmonicDriveValue(parameters.first, parameters.second);
        }
    }

    // Recursive helper function to get all the scaling function constants/slopes for all the custom cct harmonics
    void ModelHandler::parseHarmonicDrive(const Json::Value &root, HarmonicDriveParameterMap &harmonics_map, const std::string &prefix, const std::regex &suffix_regex)
    {
        if (root.isObject())
        {
            // get the next object
            std::string name = root["name"].asString();

            // check if the name fits the pattern (prefix)
            if (root["type"].asString() == "rat::mdl::cctharmonicdrive" && name.substr(0, prefix.size()) == prefix)
            {
                // check if the name fits the pattern (prefix)
                std::string suffix = name.substr(prefix.size());
                if (std::regex_match(suffix, suffix_regex))
                {

                    // check the type of scaling function and set the params accordingly
                    HarmonicDriveParameters params;

                    if (root["harmonic_drive"]["type"].asString() == "rat::mdl::drivedc")
                    {

                        double constant = root["harmonic_drive"]["scaling"].asDouble();
                        params = HarmonicDriveParameters(constant, HarmonicDriveParameterType::Constant);
                    }
                    else if (root["harmonic_drive"]["type"].asString() == "rat::mdl::drivelinear")
                    {

                        double slope = root["harmonic_drive"]["slope"].asDouble();
                        double offset = root["harmonic_drive"]["offset"].asDouble();
                        params = HarmonicDriveParameters(offset, slope);
                    }
                    else
                    {
                        throw std::runtime_error("Unsupported harmonic drive type: " + root["harmonic_drive"]["type"].asString() + ". This optimizer only supports rat::mdl::drivedc and rat::mdl::drivelinear.");
                    }

                    // check if a harmonic with the same name was already detected; throw an error if they are not equal
                    if (harmonics_map.find(name) != harmonics_map.end() && !(harmonics_map[name] == params))
                    {
                        throw std::runtime_error("Conflicting values for harmonic drive with the name " + name +
                                                 ". Values: " + to_string(harmonics_map[name]) + " and " + to_string(params));
                    }

                    harmonics_map[name] = params;
                }
            }
        }

        for (const auto &element : root)
        {
            parseHarmonicDrive(element, harmonics_map, prefix, suffix_regex);
        }
    }

    // Recursive helper function to update all the custom cct harmonics' scaling offset/slope/scaling for the given name
    // Only works for custom harmonics with 'amplitude = linear | constant'
    void ModelHandler::updateHarmonicDrive(Json::Value &root, const std::string &name, const HarmonicDriveParameterType &type, double value)
    {
        if (root.isObject())
        {
            if (root["type"].asString() == "rat::mdl::cctharmonicdrive" && root["name"].asString() == name)
            {
                if (root["harmonic_drive"]["type"].asString() == "rat::mdl::drivelinear")
                {
                    switch (type)
                    {
                    case HarmonicDriveParameterType::Offset:
                        root["harmonic_drive"]["offset"] = value;
                        break;
                    case HarmonicDriveParameterType::Slope:
                        root["harmonic_drive"]["slope"] = value;
                        break;
                    default:
                        throw std::runtime_error("Tried to apply non-matching HarmonicDriveParameterType to custom harmonic " + name);
                    }
                }
                else if (root["harmonic_drive"]["type"].asString() == "rat::mdl::drivedc")
                {
                    switch (type)
                    {
                    case HarmonicDriveParameterType::Constant:
                        root["harmonic_drive"]["scaling"] = value;
                        break;
                    default:
                        throw std::runtime_error("Tried to apply non-matching HarmonicDriveParameterType to custom harmonic " + name);
                    }
                }
            }
        }

        for (Json::Value &element : root)
        {
            updateHarmonicDrive(element, name, type, value);
        }
    }

    void ModelHandler::setValueByName(std::string name, std::vector<JSONChildrenIdentifierType> children, JSONChildrenIdentifierType target, Json::Value value)
    {
        std::ifstream file(temp_json_path_.string());
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open JSON file.");
        }

        Json::Value root;
        file >> root;
        file.close();

        bool success = updateValueByName(root, name, children, target, value);
        if (!success)
        {
            throw std::runtime_error("Element with name '" + name + "' not found.");
        }

        std::ofstream out_file(temp_json_path_.string());
        out_file << root;
        out_file.close();
    }

    bool ModelHandler::updateValueByName(Json::Value &root, const std::string &name, const std::vector<JSONChildrenIdentifierType> &children, const JSONChildrenIdentifierType &target, const Json::Value &value)
    {
        // Base case: If the current JSON object has the specified name
        if (root.isObject() && root["name"].asString() == name)
        {
            // Traverse through children if specified
            Json::Value *current = &root;
            for (const auto &child : children)
            {
                // Check type of children identifier
                if (std::holds_alternative<std::string>(child))
                {
                    const std::string &child_str = std::get<std::string>(child);
                    // Identifier is a string; references the 'name' field
                    if (current->isMember(child_str))
                    {
                        current = &(*current)[child_str];
                    }
                    else
                    {
                        throw std::runtime_error("Child element '" + child_str + "' not found.");
                    }
                }
                else if (std::holds_alternative<Json::ArrayIndex>(child))
                {
                    Json::ArrayIndex child_index = std::get<Json::ArrayIndex>(child);
                    // Identifier is an index of an array
                    if (current->isArray() && child_index < current->size())
                    {
                        current = &((*current)[child_index]);
                    }
                    else
                    {
                        throw std::runtime_error("Child index '" + std::to_string(child_index) + "' out of bounds.");
                    }
                }
                else
                {
                    throw std::runtime_error("Unsupported child identifier type: " + std::to_string(child.index()) + ".");
                }
            }

            // Check type of target
            if (std::holds_alternative<std::string>(target))
            {
                const std::string &target_str = std::get<std::string>(target);
                if (current->isMember(target_str))
                {
                    (*current)[target_str] = value;
                    return true; // Indicate success
                }
                else
                {
                    throw std::runtime_error("Target element '" + target_str + "' not found.");
                }
            }
            else if (std::holds_alternative<Json::ArrayIndex>(target))
            {
                Json::ArrayIndex target_index = std::get<Json::ArrayIndex>(target);
                if (current->isArray() && target_index < current->size())
                {
                    (*current)[target_index] = value;
                    return true; // Indicate success
                }
                else
                {
                    throw std::runtime_error("Target index '" + std::to_string(target_index) + "' out of bounds.");
                }
            }
            else
            {
                throw std::runtime_error("Unsupported target identifier type: " + std::to_string(target.index()) + ".");
            }
        }

        // Recursive case: Traverse through all members
        bool found = false;
        for (auto &member : root.getMemberNames())
        {
            if (root[member].isObject())
            {
                found = updateValueByName(root[member], name, children, target, value) || found;
            }
            else if (root[member].isArray())
            {
                // If the current member is an array, iterate through each element
                for (auto &element : root[member])
                {
                    if (element.isObject())
                    {
                        found = updateValueByName(element, name, children, target, value) || found;
                    }
                }
            }
        }

        return found;
    }

    Json::Value ModelHandler::getValueByName(const std::string &name, const std::vector<JSONChildrenIdentifierType> &children, const JSONChildrenIdentifierType &target)
    {
        // Open the JSON file
        std::ifstream file(temp_json_path_.string());
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open JSON file.");
        }

        // Parse the JSON file
        Json::Value root;
        file >> root;
        file.close();

        // Use the recursive helper function to find and retrieve the value
        return parseValueByName(root, name, children, target);
    }

    Json::Value ModelHandler::parseValueByName(const Json::Value &root, const std::string &name, const std::vector<JSONChildrenIdentifierType> &children, const JSONChildrenIdentifierType &target)
    {
        // Base case: If the current JSON object has the specified name
        if (root.isObject() && root["name"].asString() == name)
        {
            // Traverse through children if specified
            const Json::Value *current = &root;
            for (const auto &child : children)
            {
                // Check type of children identifier
                if (std::holds_alternative<std::string>(child))
                {
                    const std::string &child_str = std::get<std::string>(child);
                    // Identifier is a string; references the 'name' field
                    if (current->isMember(child_str))
                    {
                        current = &(*current)[child_str];
                    }
                    else
                    {
                        throw std::runtime_error("Child element '" + child_str + "' not found.");
                    }
                }
                else if (std::holds_alternative<Json::ArrayIndex>(child))
                {
                    Json::ArrayIndex child_index = std::get<Json::ArrayIndex>(child);
                    // Identifier is an index of an array
                    if (current->isArray() && child_index < current->size())
                    {
                        current = &((*current)[child_index]);
                    }
                    else
                    {
                        throw std::runtime_error("Child index '" + std::to_string(child_index) + "' out of bounds.");
                    }
                }
                else
                {
                    throw std::runtime_error("Unsupported child identifier type: " + std::to_string(child.index()) + ".");
                }
            }

            // Retrieve the target element value
            if (std::holds_alternative<std::string>(target))
            {
                const std::string &target_str = std::get<std::string>(target);
                if (current->isMember(target_str))
                {
                    return (*current)[target_str];
                }
                else
                {
                    throw std::runtime_error("Target element '" + target_str + "' not found.");
                }
            }
            else if (std::holds_alternative<Json::ArrayIndex>(target))
            {
                Json::ArrayIndex target_index = std::get<Json::ArrayIndex>(target);
                if (current->isArray() && target_index < current->size())
                {
                    return (*current)[target_index];
                }
                else
                {
                    throw std::runtime_error("Target index '" + std::to_string(target_index) + "' out of bounds.");
                }
            }
            else
            {
                throw std::runtime_error("Unsupported target identifier type: " + std::to_string(target.index()) + ".");
            }
        }

        // Recursive case: Traverse through all members
        for (const auto &member : root.getMemberNames())
        {
            if (root[member].isObject())
            {
                try
                {
                    return parseValueByName(root[member], name, children, target);
                }
                catch (const std::runtime_error &e)
                {
                    // Continue searching if not found in this branch
                }
            }
            else if (root[member].isArray())
            {
                // If the current member is an array, iterate through each element
                for (const auto &element : root[member])
                {
                    if (element.isObject())
                    {
                        try
                        {
                            return parseValueByName(element, name, children, target);
                        }
                        catch (const std::runtime_error &e)
                        {
                            // Continue searching if not found in this branch
                        }
                    }
                }
            }
        }

        // If the element is not found, throw an exception
        throw std::runtime_error("Element with name '" + name + "' not found.");
    }

    // Getter for the temporary JSON file path
    boost::filesystem::path ModelHandler::getTempJsonPath() const
    {
        return temp_json_path_;
    }

} // namespace CCTools
