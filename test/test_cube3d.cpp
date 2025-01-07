#include "gtest/gtest.h"
#include "model_calculator.h"
#include "mesh_data_handler.h"
#include <boost/filesystem.hpp>
#include <constants.h>

namespace CCTools
{

class Cube3DTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

        test_file_2 = TEST_DATA_DIR + "curvature_model.json";
        calculator2_ = ModelCalculator(test_file_2);
        calculator2_.calc_mesh(mesh_handler2_);
    }

    boost::filesystem::path test_file_2;
    ModelCalculator calculator2_;
    MeshDataHandler mesh_handler2_;
};

TEST_F(Cube3DTest, isPointInCube){
    Cube3D cube_normal(0.0, 10.0, 0.0, 10.0, 0.0, 10.0, false);
    Cube3D cube_inverted(0.0, 10.0, 0.0, 10.0, 0.0, 10.0, true);


    std::vector<arma::subview<double>> points_inside;
    std::vector<arma::subview<double>> points_outside;

    arma::mat coords_inside = {{1.0, 2.0, 3.0},
                               {4.0, 5.0, 6.0},
                               {7.0, 8.0, 9.0},
                               {2.0, 3.0, 4.0}};
    arma::mat coords_outside = {{-1.0, 2.0, 3.0},
                                {4.0, 15.0, 6.0},
                                {7.0, 8.0, -9.0},
                                {12.0, 3.0, 4.0}};

    for (size_t i = 0; i < coords_inside.n_rows; ++i)
    {
        points_inside.push_back(coords_inside.row(i));
    }

    for (size_t i = 0; i < coords_outside.n_rows; ++i)
    {
        points_outside.push_back(coords_outside.row(i));
    }

    for (const auto& point : points_inside)
    {
        EXPECT_TRUE(cube_normal.isPointInCube(point));
        EXPECT_FALSE(cube_inverted.isPointInCube(point));
    }

    for (const auto& point : points_outside)
    {
        EXPECT_FALSE(cube_normal.isPointInCube(point));
        EXPECT_TRUE(cube_inverted.isPointInCube(point));
    }
}

} // namespace CCTools
