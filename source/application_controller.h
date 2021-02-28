#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <QString>

class ApplicationController
{
    public:
        /**
         * @brief C-tor
         */
        ApplicationController();

        /**
         * @brief D-tor
         */
        ~ApplicationController();


        /**
         * @brief Get dlib data base folder path
         * @return path of data folder
         */
        const QString& getMaskModelPath() const;


    private:
        /**
         * @brief Request all neccessary permission from mobile phone
         * @return true: permission granted
         */
        bool RequestPermissions();

        /**
         * @brief initialize home directory or create it
         * @return if true: directory created
         */
        bool initializeHomePath();


        /**
         * @brief Store shape_predictor_68_face_landmarks.dat data
         * in phone
         * @return if true: file copyed correctly
         */
        bool initializeMaskModelPath();


    private:
        QString m_homePath;
        QString m_maskModelPath;

};
#endif //APPLICATION_CONTROLLER_H
