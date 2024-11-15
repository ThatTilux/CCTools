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
    }

    boost::filesystem::path test_file;
    ModelCalculator calculator_;
    MeshDataHandler mesh_handler_;
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


} // namespace CCTools