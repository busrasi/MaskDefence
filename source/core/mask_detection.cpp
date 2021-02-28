#include "mask_detection.h"

#include <dlib/opencv/cv_image.h>
#include <dlib/opencv/to_open_cv.h>
#include <dlib/array2d.h>
#include <dlib/geometry.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/frontal_face_detector.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <cmath>

#include <QDebug>


static constexpr int const& LEFT_EYE_SHAPE_SIZE = 6;
static constexpr int const& RIGHT_EYE_SHAPE_SIZE = 12;

MaskDetection::MaskDetection(const std::string maskDataPath)
    : m_maskDataPath(maskDataPath)
    , m_maskResultCollector(std::make_unique<MaskResultCollector>())
{
    // Frontal Detector
    m_FaceDetector = dlib::get_frontal_face_detector();

    // Deserialize trained data
    dlib::deserialize(m_maskDataPath) >> m_Predictor;
}

MaskDetection::~MaskDetection()
{}

MaskResultCollector* MaskDetection::maskResultCollector()
{
    return m_maskResultCollector.get();
}

bool MaskDetection::executeMaskDetection(cv::Mat& image, int resizeFactor)
{
    // Resize Image
    if(resizeFactor != 1) {
        cv::resize(image, image, cv::Size(image.cols * resizeFactor, image.rows * resizeFactor), 0, 0, cv::INTER_LINEAR);
    }

    // Convert BGR to RGB
    dlib::cv_image<dlib::bgr_pixel> rgb_image(image);

    // Find all faces from rbg_image
    std::vector<dlib::rectangle> faces = m_FaceDetector(rgb_image);

    // This will store shapes of faces
    std::vector<dlib::full_object_detection> shapes;

    // Get the number of vectors in the container, that is, the number of faces
    std::for_each(faces.begin(), faces.end(), [&](dlib::rectangle const& face)
    {
        // if the current index is needed:
        // auto i = &face - &faces[0];
        shapes.emplace_back(m_Predictor(rgb_image, face));
    });

    // Get shapes of Left and Right Eye
    for(const auto& shape : shapes)
    {
        // Is Left Eye Found?
        if(shape.num_parts() >= LEFT_EYE_SHAPE_SIZE)
        {
            // Left Eye
            m_LeftEye.eyeCoordinates = {
                // The coordinates of point 36
                cv::Point(shape.part(0).x(), shape.part(0).y()),

                // The coordinates of point 37
                cv::Point(shape.part(1).x(), shape.part(1).y()),

                // The coordinates of point 38
                cv::Point(shape.part(2).x(), shape.part(2).y()),

                // The coordinates of point 39
                cv::Point(shape.part(3).x(), shape.part(3).y()),

                // The coordinates of point 40
                cv::Point(shape.part(4).x(), shape.part(4).y()),

                // The coordinates of point 41
                cv::Point(shape.part(5).x(), shape.part(5).y())
            };

            cv::circle(image, cv::Point(shape.part(0).x(), shape.part(0).y()), 4, cv::Scalar(0, 0, 255), -1);
            cv::circle(image, cv::Point(shape.part(1).x(), shape.part(1).y()), 4, cv::Scalar(0, 0, 255), -1);
            cv::circle(image, cv::Point(shape.part(2).x(), shape.part(2).y()), 4, cv::Scalar(0, 0, 255), -1);
            cv::circle(image, cv::Point(shape.part(3).x(), shape.part(3).y()), 4, cv::Scalar(0, 0, 255), -1);
            cv::circle(image, cv::Point(shape.part(4).x(), shape.part(4).y()), 4, cv::Scalar(0, 0, 255), -1);
            cv::circle(image, cv::Point(shape.part(5).x(), shape.part(5).y()), 4, cv::Scalar(0, 0, 255), -1);
        }

        // Is Right Eye Found?
        if(shape.num_parts() >= RIGHT_EYE_SHAPE_SIZE)
        {
            // Right Eye
            m_RightEye.eyeCoordinates = {
                // The coordinates of point 42
                cv::Point(shape.part(6).x(), shape.part(6).y()),

                // The coordinates of point 43
                cv::Point(shape.part(7).x(), shape.part(7).y()),

                // The coordinates of point 44
                cv::Point(shape.part(8).x(), shape.part(8).y()),

                // The coordinates of point 45
                cv::Point(shape.part(9).x(), shape.part(9).y()),

                // The coordinates of point 46
                cv::Point(shape.part(10).x(), shape.part(10).y()),

                // The coordinates of point 47
                cv::Point(shape.part(11).x(), shape.part(11).y())
            };
        }

        cv::circle(image, cv::Point(shape.part(6).x(), shape.part(6).y()), 4, cv::Scalar(255, 0, 0), -1);
        cv::circle(image, cv::Point(shape.part(7).x(), shape.part(7).y()), 4, cv::Scalar(255, 0, 0), -1);
        cv::circle(image, cv::Point(shape.part(8).x(), shape.part(8).y()), 4, cv::Scalar(255, 0, 0), -1);
        cv::circle(image, cv::Point(shape.part(9).x(), shape.part(9).y()), 4, cv::Scalar(255, 0, 0), -1);
        cv::circle(image, cv::Point(shape.part(10).x(), shape.part(10).y()), 4, cv::Scalar(255, 0, 0), -1);
        cv::circle(image, cv::Point(shape.part(11).x(), shape.part(11).y()), 4, cv::Scalar(255, 0, 0), -1);
    }


    return false;
}


float MaskDetection::euclidean_distance(const cv::Point& p1, const cv::Point& p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}


