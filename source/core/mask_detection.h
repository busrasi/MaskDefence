#ifndef MASK_DETECTION_H
#define MASK_DETECTION_H

#include <memory>
#include <string>

#include <opencv2/opencv.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/object_detector.h>

#include "mask_result_collector.h"

/**
  * @brief The Eye struct for storing points of a single eye.
  */

typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;

struct Eye
{
public:
    std::array<cv::Point, 6> eyeCoordinates {
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0)
    };
};


/**
  * \class LivenessDetection
  *
  * \brief Liveness Detection Class
  *
  *       Uses KYCEngine libraries and should be call as a api
  */
class MaskDetection
{
public:
    /**
      * @brief C-tor
      * @param maskDataPath = Data path of shape predictor model. ( File with .dat extension.)
      */
    MaskDetection(const std::string maskDataPath);

    /**
      * @brief D-tor
      */
    ~MaskDetection();

    /**
      * @brief livenessResultCollector
      * @return
      */
    MaskResultCollector* maskResultCollector();

    /**
      * @brief executeLivenessDetection
      * @param Input frame from camera.
      * @param resizeFactor: between 0.1 - 1.0
      * @return
      */
    //bool executeMaskDetection(cv::Mat& image, int resizeFactor = 1.0);
    bool executeObjDetector(cv::Mat& image, int resizeFactor = 1.0);


protected:

private:
    /**
      * @brief Calculates the Euclidean distance between two points.
      * @param p1 : Point1
      * @param p2 : Point2
      * @return float : Distance between two points.
      */
    float euclidean_distance(const cv::Point& p1, const cv::Point& p2);

private:
    std::string m_maskDataPath = "";
    std::unique_ptr<MaskResultCollector> m_maskResultCollector;

    dlib::frontal_face_detector m_FaceDetector;
    //dlib::shape_predictor m_Predictor;
    dlib::object_detector<image_scanner_type> m_Predictor;
    Eye m_LeftEye;
    Eye m_RightEye;

};



#endif // MASK_DETECTION_H
