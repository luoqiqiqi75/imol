#ifndef IMOL_FORMATTER_H
#define IMOL_FORMATTER_H

#include "Eigen/Dense"

namespace ix {
namespace robot {
typedef Eigen::Transform<double, 3, Eigen::Isometry> Frame;
typedef Eigen::Quaterniond Quaternion;
typedef Eigen::Matrix<double, 3, 1> Translation;
typedef Eigen::AngleAxisd Rotation;
using Eigen::Vector3d;
}
}

#endif // IMOL_FORMATTER_H
