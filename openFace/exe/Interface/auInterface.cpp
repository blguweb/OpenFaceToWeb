// Local includes
#include <opencv2/core/core.hpp>
#include "LandmarkCoreIncludes.h"
#include <Face_utils.h>
#include <FaceAnalyser.h>

#ifndef CONFIG_DIR
#define CONFIG_DIR "~"
#endif

//Convert arguments to more convenient vector form
// std::vector<std::string> arguments;

//init
LandmarkDetector::FaceModelParameters det_parameters("/home/lyr/kidominox/OpenFace-master/build/bin");
FaceAnalysis::FaceAnalyserParameters face_analysis_params("/home/lyr/kidominox/OpenFace-master/build/bin");
LandmarkDetector::CLNF *face_model;
FaceAnalysis::FaceAnalyser *face_analyser;

extern "C" {
    void Init()
    {

        // arguments.push_back(std::string("./bin/AuDetection"));
        
        // det_parameters = new LandmarkDetector::FaceModelParameters(arguments);
        // face_analysis_params = new FaceAnalysis::FaceAnalyserParameters(arguments);
        face_analysis_params.OptimizeForImages();
        face_model = new LandmarkDetector::CLNF(det_parameters.model_location);
        face_analyser = new FaceAnalysis::FaceAnalyser(face_analysis_params);
        face_model->face_detector_MTCNN.Read(det_parameters.mtcnn_face_detector_location);
        face_model->mtcnn_face_detector_location = det_parameters.mtcnn_face_detector_location;
    }

    char* AuInerface(int height ,int width,unsigned char* frame_data)
    {
        cv::Mat frame(height,width,CV_8UC3,(void *)frame_data);
        // std::cout<< frame_data << std::endl;
        // std::cout<< frame <<std::endl;
        cv::Mat grayscale_image;
        bool detection_success = LandmarkDetector::DetectLandmarksInVideo(frame, *face_model, det_parameters, grayscale_image);
        std::cout<< "get the frame "<< std::endl;
        std::cout<<detection_success<<std::endl;
        std::string auResult = "Detection failed!";
        char* auCpyResult = new char[20];
        if(detection_success)
        {
            face_analyser->PredictStaticAUsAndComputeFeatures(frame, face_model->detected_landmarks);
            std::vector<std::pair<std::string, double>> AUsClass = face_analyser->GetCurrentAUsClass();
            std::map<std::string, double> mapAUsClass;
            for (size_t i = 0; i < AUsClass.size(); i++)
            {
                mapAUsClass.insert(AUsClass[i]);
            }
            if (mapAUsClass["AU01"] + mapAUsClass["AU04"] + mapAUsClass["AU20"] + mapAUsClass["AU25"] > 3.5)
            {
                auResult = "fear";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult; 
            }
            else if (mapAUsClass["AU01"] + mapAUsClass["AU02"] + mapAUsClass["AU25"] + mapAUsClass["AU26"] > 3.5)
            {
                auResult = "suprise";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult;
            }
            else if (mapAUsClass["AU09"] + mapAUsClass["AU10"] + mapAUsClass["AU17"] > 2.5)
            {
                auResult = "disgusted";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult;
            }
            else if (mapAUsClass["AU04"] > 0.9)
            {
                auResult = "sad";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult;
            }
            else if (mapAUsClass["AU04"] + mapAUsClass["AU07"] + mapAUsClass["AU23"] > 2.5)
            {
                auResult = "angry";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult;
            }

            else if (mapAUsClass["AU17"] + mapAUsClass["AU20"] + mapAUsClass["AU23"]> 2.3)
            {
                auResult = "smile";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult;
            }
            else if (mapAUsClass["AU06"] + mapAUsClass["AU12"] > 1.9 || mapAUsClass["AU12"] > 0.9)
            {
                auResult = "happy";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult;
            }
            else
            {
                auResult = "normal";
                strcpy(auCpyResult,auResult.c_str());
                return auCpyResult;
            }
        }
        else
        {
            std::cout<<auResult<<std::endl;
            strcpy(auCpyResult,auResult.c_str());
            return auCpyResult;
        }
            

    }
}




