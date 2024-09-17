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
#include "calc_result_handler_base.h"

namespace CCTools {

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
