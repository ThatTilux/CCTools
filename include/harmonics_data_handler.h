#ifndef CCTOOLS_HARMONICS_DATA_HANDLER_H
#define CCTOOLS_HARMONICS_DATA_HANDLER_H

#include <armadillo>
#include <boost/filesystem.hpp>
#include <rat/common/log.hh>
#include <rat/models/serializer.hh>
#include <rat/models/modelroot.hh>
#include <rat/models/calcgroup.hh>
#include <rat/models/calcharmonics.hh>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "calc_result_handler_base.h"

namespace CCTools {

/**
 * @class HarmonicsDataHandler
 * @brief Class for handling the result of a harmonics calculation.
 *
 * This class is used to handle the result of a harmonics calculation. It provides functions to extract information from the raw harmonics data.
 */
class HarmonicsDataHandler : public CalcResultHandlerBase
{
public:
    /**
     * @brief Construct an empty dummy HarmonicsDataHandler object.
     *
     * This constructor is used to create an empty HarmonicsDataHandler object.
     */
    HarmonicsDataHandler();

    /**
     * @brief Construct a new HarmonicsDataHandler object.
     * @param harmonics_data Harmonics data object.
     *
     * This constructor initializes the HarmonicsDataHandler object with the raw data from a harmonics calculation.
     */
    HarmonicsDataHandler(rat::mdl::ShHarmonicsDataPr harmonics_data);

    /**
     * @brief Artificially construct a new HarmonicsDataHandler object.
     * @param ell List of ell values.
     * @param Bn_per_component List of Bn values per component.
     *
     * This constructor artificially initializes the HarmonicsDataHandler object with given ell and Bn data. To be used for testing only.
     */
    HarmonicsDataHandler(std::vector<double> ell, std::vector<std::vector<double>> Bn_per_component);

    /**
     * @brief Get the a_n values for all components A1 to A10.
     */
    std::vector<double> get_an();

    /**
     * @brief Get the b_n values for all components B1 to B10.
     */
    std::vector<double> get_bn();

    /**
     * @brief Get the Bn and ell data for a specific component.
     * @param component Component number (1 to 10).
     * @return Vector of pairs containing ell and Bn values.
     *
     * This function returns the Bn data for a specific component along the magnet.
     * The Bn data indicates the strength [T] of the component at certain points along the magnet (see ell). Only returns data within the set ell bounds.
     */
    std::vector<std::pair<double, double>> get_Bn(int component);

private:
    /**
     * @brief Extract the ell and Bn data from the harmonics data.
     * @param harmonics_data Harmonics data object.
     * @return Tuple containing the ell values and the Bn values per component.
     *
     * This function extracts the ell (= length) and all the Bn data from the harmonics data object.
     */
    std::tuple<std::vector<double>, std::vector<std::vector<double>>> extract_ell_Bn(rat::mdl::ShHarmonicsDataPr harmonics_data);

    /**
     * @brief Extract and set the a_n and b_n values from harmonics data.
     * @param harmonics_data Harmonics data object.
     *
     * This function extracts the a_n and b_n values from the harmonics data object and sets them as the fields `an_` and `bn_`.
     */
    void extract_an_bn(rat::mdl::ShHarmonicsDataPr harmonics_data);

    /**
     * @brief Convert the b_n arma::Row to a vector.
     *
     * This function converts an arma::Row of harmonics data to a vector, omitting the first (dummy) value.
     */
    std::vector<double> convert_bn_to_vector(const arma::Row<rat::fltp> &bn);

    /**
     * @brief Convert an arma::Row to a std::vector.
     * @param row Arma::Row object.
     * @return Vector of doubles.
     */
    std::vector<double> convert_row_to_vector(const arma::Row<rat::fltp> &row);

    /**
     * @brief Convert a column to a vector.
     * @param col Arma::Col object.
     * @return Vector of doubles.
     */
    std::vector<double> convert_col_to_vector(const arma::Col<rat::fltp> &col);

    /**
     * @brief Get the ell values.
     * @return Vector of ell values.
     *
     * This function returns the ell values, which represent the length of the magnet.
     */
    std::vector<double> get_ell_();

    /**
     * @brief Get the Bn values for a specific component.
     * @param component Component number (1 to 10).
     * @return Vector of Bn values.
     *
     * This function returns the Bn data for a specific component along the magnet.
     * The Bn data indicates the strength [T] of the component at certain points along the magnet (ell).
     */
    std::vector<double> get_Bn_(int component);

    rat::mdl::ShHarmonicsDataPr harmonics_data_;
    std::vector<double> an_;
    std::vector<double> bn_;
    std::vector<double> ell_;
    std::vector<std::vector<double>> Bn_per_component_;
};

/**
 * @brief Combine two vectors of doubles into a vector of pairs.
 * @param x Vector of x values.
 * @param y Vector of y values.
 * @return Vector of pairs containing x and y values.
 *
 * This function combines two vectors of doubles into a vector of pairs.
 * The x and y values are combined into pairs, where the first element of the pair is the x value and the second element is the y value.
 */
std::vector<std::pair<double, double>> combinePoints(const std::vector<double> &x, const std::vector<double> &y);

} // namespace CCTools

#endif // CCTOOLS_HARMONICS_DATA_HANDLER_H
