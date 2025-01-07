#include <armadillo>

namespace CCTools
{
    /**
     * @brief Class for spanning a cube in 3D space.
     *
     * This class provides functionality to define a cube in 3D space using minimum and maximum
     * coordinates along the x, y, and z axes. It also includes a method to determine if a given
     * point is inside the cube.
     */
    class Cube3D
    {
    public:
        /**
         * @brief Construct a Cube3D object.
         * @param xMin Minimum x-coordinate of the cube.
         * @param xMax Maximum x-coordinate of the cube.
         * @param yMin Minimum y-coordinate of the cube.
         * @param yMax Maximum y-coordinate of the cube.
         * @param zMin Minimum z-coordinate of the cube.
         * @param zMax Maximum z-coordinate of the cube.
         * @param invertCube If true, the cube is considered inverted, meaning the isPointInCube method
         *                   will return true for points outside the cube and false for points inside.
         */
        Cube3D(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax, bool invertCube = false)
            : x_min(xMin), x_max(xMax), y_min(yMin), y_max(yMax), z_min(zMin), z_max(zMax), invert_cube(invertCube) {}

        /**
         * @brief Determine if point is inside a cube.
         * @param point Coordinates of the point (m).
         * @param cube 3D cube. Values in (m).
         * @return true if the point is in the cube, false otherwise.
         */
        bool isPointInCube(const arma::subview<double> &point) const
        {
            double x = point(0);
            double y = point(1);
            double z = point(2);

            if (x < x_min || x > x_max || y < y_min || y > y_max || z < z_min || z > z_max)
            {
                // point is outside the cube
                return invert_cube;
            }
            // point is inside the cube
            return !invert_cube;
        }

    private:
        /**
         * @brief x coordinates of the cube (m).
         */
        double x_min, x_max;

        /**
         * @brief y coordinates of the cube (m).
         */
        double y_min, y_max;

        /**
         * @brief z coordinates of the cube (m).
         */
        double z_min, z_max;

        /**
         * @brief Flag to invert the cube.
         * 
         * If set to true, the cube is spans across the entire coordinate space, excluding the area specified with the coordinates.
         */
        bool invert_cube;
    };
} // namespace CCTools