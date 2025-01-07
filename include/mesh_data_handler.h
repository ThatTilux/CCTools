#ifndef CCTOOLS_MESH_DATA_HANDLER_H
#define CCTOOLS_MESH_DATA_HANDLER_H

#include <armadillo>
#include <boost/filesystem.hpp>
#include <rat/common/log.hh>
#include <rat/models/serializer.hh>
#include <rat/models/modelroot.hh>
#include <rat/models/calcgroup.hh>
#include <rat/models/calcmesh.hh>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include "calc_result_handler_base.h"
#include "cube3d.hh"

namespace CCTools
{

    /**
     * @brief Enum class for specifying the field component of a Mesh calculation.
     */
    enum MeshFieldComponent
    {
        LONGITUDINAL,
        NORMAL,
        TRANSVERSE,
        MAGNITUDE
    };

    /**
     * @class MeshDataHandler
     * @brief Class for handling the result of a mesh calculation.
     *
     * This class is used to handle the result of a mesh calculation. It provides functions to extract information from the raw mesh data.
     */
    class MeshDataHandler : public CalcResultHandlerBase
    {
    public:
        /**
         * @brief Construct an empty dummy MeshDataHandler object.
         *
         * This constructor is used to create an empty MeshDataHandler object.
         */
        MeshDataHandler();

        /**
         * @brief Construct a new MeshDataHandler object.
         * @param mesh_data List of mesh data objects.
         *
         * This constructor initializes the MeshDataHandler object with the raw data from a mesh calculation.
         */
        MeshDataHandler(std::list<rat::mdl::ShMeshDataPr> &mesh_data);

        /**
         * @brief Get the min and max z coordinates of the magnet.
         * @return Pair containing the min and max z values.
         *
         * This function extracts the minimum and maximum z coordinates of the magnet from the mesh data.
         * These can be used to calculate the length of the magnet along the z-axis.
         */
        std::pair<double, double> getMinMaxZValues();

        /**
         * @brief Get the maximum curvature of the magnet.
         * @param field_component Field component to compute the max curvature for.
         * @param filter_area (Optional). 3D cube in space to filter curvature data. Values in (m).
         *
         * Computes the curvature of of the magnet from the mesh, extracts the set field component and returns the maximum.
         * If `filter_area` is set, the function will only consider curvature values from mesh nodes inside the specified area.
         */
        double getMaxCurvature(MeshFieldComponent field_component, const std::optional<Cube3D> &filter_area = std::nullopt);

    private:
        /**
         * @brief Get the mesh data at the specified index.
         * @param index Index of the mesh data.
         * @return Mesh data object.
         *
         * This function returns the raw mesh data object at the specified index of the raw mesh data list.
         */
        rat::mdl::ShMeshDataPr getMeshData(int index);

        /**
         * @brief Get the size of the mesh data list.
         * @return Size of the mesh data list.
         *
         * This function returns the size of the raw mesh data list.
         */
        int getMeshDataSize();

        /**
         * @brief Get the curvature of the magnet.
         * @param field_component Field component to compute the curvature for.
         * @param mesh_data_index Index of the mesh data to be used in the `mesh_data_` list.
         * @param filter_area (Optional). 3D cube in space to filter curvature data. Values in (m).
         * @returns Row of curvature per node in the mesh.
         *
         * Computes the curvature of of the magnet from the mesh and extracts the set field component.
         * If `filter_area` is set, the function will only return curvature datapoints for points inside the specified area.
         */
        arma::Row<double> getCurvature(MeshFieldComponent field_component, size_t mesh_data_index, const std::optional<Cube3D> &filter_area = std::nullopt);

        /**
         * @brief Filter curvature data by coordinates.
         * @param K Curvature data
         * @param filter_area 3D cube in space to filter the data.
         * @param mesh_data Mesh data that produced `K`.
         *
         * Filters the curvature data by removing all curvature values in `K` from datapoints that are not inside the specified `filter_area`.
         */
        void filterCurvature(arma::dmat *K, const Cube3D &filter_area, rat::mdl::ShMeshDataPr mesh_data);

        /**
         * @brief Get the z coordinates of the mesh data.
         * @param mesh_data Mesh data object.
         * @param z Field of matrices. The z coordinates will be stored in this field.
         *
         * This function extracts the z coordinates from the measurements of the `mesh_data` object and stores them in the provided `z` parameter.
         */
        void get_z(rat::mdl::ShMeshDataPr mesh_data, arma::field<arma::dmat> &z);

        /**
         * @brief List of mesh data objects.
         *
         * This list contains the raw mesh data objects from the mesh calculation.
         */
        std::list<rat::mdl::ShMeshDataPr> mesh_data_;
    };

} // namespace CCTools

#endif // CCTOOLS_MESH_DATA_HANDLER_H
