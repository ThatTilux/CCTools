#include "gtest/gtest.h"
#include "model_calculator.h"
#include "mesh_data_handler.h"
#include <boost/filesystem.hpp>
#include <constants.h>

namespace CCTools {

class MeshDataHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        test_file = TEST_DATA_DIR + "quad_test.json";
        calculator_ = ModelCalculator(test_file);
        calculator_.calc_mesh(mesh_handler_);

        test_file_2 = TEST_DATA_DIR + "curvature_model.json";
        calculator2_ = ModelCalculator(test_file_2);
        calculator2_.calc_mesh(mesh_handler2_);
    }

    boost::filesystem::path test_file;
    boost::filesystem::path test_file_2;
    ModelCalculator calculator_;
    ModelCalculator calculator2_;
    MeshDataHandler mesh_handler_;
    MeshDataHandler mesh_handler2_;
};

TEST_F(MeshDataHandlerTest, getMaxMagnutiudeCurvature)
{
    double max_curvature_magnitude;
    ASSERT_NO_THROW({
        max_curvature_magnitude = mesh_handler_.getMaxCurvature(MeshFieldComponent::MAGNITUDE);
    });
    ASSERT_NEAR(max_curvature_magnitude, 4921.0, 1); // this value is from RAT-GUI
}

TEST_F(MeshDataHandlerTest, getMinMaxZValues)
{
    std::pair<double, double> min_max;
    ASSERT_NO_THROW({
        min_max = mesh_handler_.getMinMaxZValues();
    });
    ASSERT_NEAR(min_max.first, -0.118, 1e-3); // these two values are from this very software. They seem correct.
    ASSERT_NEAR(min_max.second, 0.125, 1e-3); 
}

TEST_F(MeshDataHandlerTest, getFilteredMaxCurvature){
    double max_curvature_unfiltered =  mesh_handler2_.getMaxCurvature(MeshFieldComponent::MAGNITUDE);
    ASSERT_NEAR(max_curvature_unfiltered, 236.686, 1e-1); // value from RAT-GUI

    double max_curvature_filtered;
    ASSERT_NO_THROW({
        max_curvature_filtered =  mesh_handler2_.getMaxCurvature(MeshFieldComponent::MAGNITUDE, Cube3D(28.0e-3, 84.0e-3, 27.0e-3, 52.0e-3, 133.0e-3, 207.0e-3));
    });
    ASSERT_NEAR(max_curvature_filtered, 6.280, 1e-1); // value from RAT-GUI

    // assert error when no nodes inside cube
    ASSERT_THROW({
        mesh_handler2_.getMaxCurvature(MeshFieldComponent::MAGNITUDE, Cube3D(280000.0, 8400000.0, 0, 0, 0, 0));
    }, std::invalid_argument);
}


} // namespace CCTools