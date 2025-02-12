#ifndef CCTOOLS_MODEL_CALCULATOR_H
#define CCTOOLS_MODEL_CALCULATOR_H

#include <armadillo>
#include <boost/filesystem.hpp>
#include <rat/common/log.hh>
#include <rat/models/serializer.hh>
#include <rat/models/modelroot.hh>
#include <rat/models/calcgroup.hh>
#include <rat/models/calcharmonics.hh>
#include <rat/models/calcmesh.hh>
#include <rat/models/pathaxis.hh>
#include <iostream>
#include <string>
#include <vector>
#include "harmonics_data_handler.h"
#include "mesh_data_handler.h"
#include "Logger.hh"

namespace CCTools
{

    /**
     * @class ModelCalculator
     * @brief Class to run computations of a magnet model.
     *
     * This class is used to run calculations in the calculation tree of the magnet model JSON file. Results are provided in wrapper classes to simplify the handling of the results.
     */
    class ModelCalculator
    {
    public:
        /**
         * @brief Construct a new ModelCalculator object.
         * @param json_file_path Path to the JSON file.
         *
         * This constructor loads the JSON file and extracts the model tree, calculation tree and the calculation objects.
         * When using a `ModelHandler` to handle the JSON file, the `ModelHandler::getTempJsonPath()` function should be used to set the `json_file_path`.
         */
        ModelCalculator(const boost::filesystem::path &json_file_path);

        /**
         * @brief Dummy constructor. Use parameterized constructor instead.
         */
        ModelCalculator();

        /**
         * @brief Run the harmonics calculation.
         * @param harmonics_handler HarmonicsDataHandler object to store the results.
         * @param disable_logging Flag to disable logging.
         *
         * This function runs the `harmonics_calc_` to calculate the harmonics of the magnet. Requires a harmonics calculation in the calculation tree that was found during the model loading.
         * The `harmonics_handler` will be overridden with the results.
         * If `disable_logging` is set to `true`, the RAT logging will be disabled during the calculation.
         * Note: This function will not reload the model from the JSON file and any recent changes made to the file will not be in effect. Use `reload_and_calc_harmonics` to reload the model and calculate the harmonics.
         */
        void calc_harmonics(HarmonicsDataHandler &harmonics_handler, bool disable_logging = true);

        /**
         * @brief Reload the model from the JSON file and run the harmonics calculation.
         * @param json_file_path Path to the JSON file.
         * @param harmonics_handler HarmonicsDataHandler object to store the results.
         * @param disable_logging Flag to disable logging.
         *
         * This function reloads the model from the JSON file and runs the `calc_harmonics` function.
         */
        void reload_and_calc_harmonics(const boost::filesystem::path &json_file_path, HarmonicsDataHandler &harmonics_handler, bool disable_logging = true);

        /**
         * @brief Run the mesh calculation.
         * @param mesh_handler MeshDataHandler object to store the results.
         * @param disable_logging Flag to disable logging.
         *
         * This function runs the `mesh_calc_` to calculate the mesh of the magnet. Requires a mesh calculation in the calculation tree that was found during the model loading.
         * The `mesh_handler` will be overridden with the results.
         * If `disable_logging` is set to `true`, the RAT logging will be disabled during the calculation.
         * Note: This function will not reload the model from the JSON file and any recent changes made to the file will not be in effect. Use `reload_and_calc_mesh` to reload the model and calculate the mesh.
         */
        void calc_mesh(MeshDataHandler &mesh_handler, bool disable_logging = true);

        /**
         * @brief Run the mesh calculation.
         * @param json_file_path Path to the JSON file.
         * @param mesh_handler MeshDataHandler object to store the results.
         * @param disable_logging Flag to disable logging.
         *
         * This function reloads the model from the JSON file and runs the `calc_mesh` function.
         */
        void reload_and_calc_mesh(const boost::filesystem::path &json_file_path, MeshDataHandler &mesh_handler, bool disable_logging = true);

        /**
         * @brief Get the z coordinate of the harmonic calculation's axis.
         *
         * This function returns the z coordinate of the axis used in the harmonic calculation. The z coordinate is calculated with respect to all transformations applied to the axis.
         * Requires a harmonics calculation in the calculation tree that was found during the model loading.
         */
        double get_axis_z_pos();

        /**
         * @brief Get the length (ell) of the harmonic calculation's axis.
         *
         * This function returns the length of the axis used in the harmonic calculation.
         * Requires a harmonics calculation in the calculation tree that was found during the model loading.
         */
        double get_axis_ell();

        /**
         * @brief Get the model tree.
         * @return Model tree object.
         *
         * This function returns the model tree object of the model.
         */
        rat::mdl::ShModelGroupPr get_model_tree();

        /**
         * @brief Check if a harmonics calculation is available.
         * @return `true` if a harmonics calculation is available, `false` otherwise.
         */
        bool has_harmonics_calc();

        /**
         * @brief Check if a mesh calculation is available.
         * @return `true` if a mesh calculation is available, `false` otherwise.
         */
        bool has_mesh_calc();

    private:
        /**
         * @brief Load the model from the JSON file.
         * @param json_file_path Path to the JSON file.
         * @return `true` if the model was loaded successfully, `false` otherwise.
         *
         * This function loads the model from the JSON file and extracts the model tree, calculation tree and the calculation objects.
         * Will return `false` if the model could not be loaded, e.g., due to an incorrect file path or no calculations in the calculation tree.
         */
        bool load_model(const boost::filesystem::path &json_file_path);

        /**
         * @brief Parse a magnet model JSON file.
         * @param json_file_path Path to the JSON file.
         * @return Tuple containing the model, model root, model tree and calculation tree.
         *
         * This function parses the JSON file and returns the de-serialized RAT-Model C++ objects.
         */
        std::tuple<rat::mdl::ShModelPr, rat::mdl::ShModelRootPr, rat::mdl::ShModelGroupPr, rat::mdl::ShCalcGroupPr>
        load_model_from_json(const boost::filesystem::path &json_file_path);

        /**
         * @brief Enable GPU usage for a calculation.
         * @param calc_leaf Calculation object to enable GPU for.
         *
         * This function enables CUDA GPU usage for the calculation object. Will do nothing if no GPU is available.
         */
        void enable_gpu(rat::mdl::ShCalcLeafPr calc_leaf);

        /**
         * @brief Log the GPU information.
         *
         * This function logs the information about the available CUDA compatible GPU devices.
         */
        void log_gpu_info();

        /**
         * @brief Get the axis from a harmonics calculation.
         * @param harmonics_calc Harmonics calculation object.
         * @return Tuple containing the axis object and its name.
         *
         * This function extracts the axis object from the harmonics calculation object and returns it along with its name.
         */
        static std::tuple<rat::mdl::ShPathAxisPr, std::string> get_axis(const rat::mdl::ShCalcHarmonicsPr &harmonics_calc);

        // template for the find_first_calc function
        /**
         * @brief Find the first calculation of a specific type in the calculation tree.
         * @param calc_tree Calculation tree object.
         * @return Tuple containing the calculation object and its name.
         *
         * This function searches the calculation tree for a calculation of the specified type and returns the top-most calculation with that type.
         */
        template <typename T>
        static std::tuple<std::shared_ptr<T>, std::string> find_first_calc(const rat::mdl::ShCalcGroupPr &calc_tree);

        rat::mdl::ShModelPr model_;
        rat::mdl::ShModelRootPr root_;
        rat::mdl::ShModelGroupPr model_tree_;
        rat::mdl::ShCalcGroupPr calc_tree_;
        rat::mdl::ShCalcHarmonicsPr harmonics_calc_;
        rat::mdl::ShCalcMeshPr mesh_calc_;
        rat::mdl::ShPathAxisPr harmonics_axis_;
        std::string harmonics_calc_name_;
        std::string mesh_calc_name_;
        std::string harmonics_axis_name_;
    };

} // namespace CCTools

#endif // CCTOOLS_MODEL_CALCULATOR_H
