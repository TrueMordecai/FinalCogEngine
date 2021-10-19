#ifndef CALC_UTILS_H
#define CALC_UTILS_H

#include <iostream>
#include <stddef.h>
#include <assert.h>
#include <math.h>

#include <QDebug>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

//OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

class MovingAverage
{
public:

    MovingAverage(unsigned int period);
    ~MovingAverage();

    // Adds a value to the average, pushing one out if nescessary
    void add(double val);

    // Returns the average of the last P elements added to this SMA.
    // If no elements have been added yet, returns 0.0
    double avg() const;

private:
    unsigned int period;
    double * window; // Holds the values to calculate the average of.

    // Logically, head is before tail
    double * head; // Points at the oldest element we've stored.
    double * tail; // Points at the newest element we've stored.

    double total; // Cache the total so we don't sum everything each time.

    // Bumps the given pointer up by one.
    // Wraps to the start of the array if needed.
    void inc(double * & p);

    // Returns how many numbers we have stored.
    ptrdiff_t size() const;
};



//! Given a set of points, this class calculates the linear regression parameters and evaluates the regression line at arbitrary abscissas.
class Linear
{
public:
    Linear(int n, std::vector<double> x, std::vector<double> y)
    {

        // calculate the averages of arrays x and y
        double xa = 0, ya = 0;
        for (int i = 0; i < n; i++) {
            xa += x[i];
            ya += y[i];
        }
        xa /= n;
        ya /= n;

        // calculate auxiliary sums
        double xx = 0, yy = 0, xy = 0;
        for (int i = 0; i < n; i++) {
            double tmpx = x[i] - xa, tmpy = y[i] - ya;
            xx += tmpx * tmpx;
            yy += tmpy * tmpy;
            xy += tmpx * tmpy;
        }

        // calculate regression line parameters

        // make sure slope is not infinite
        assert(fabs(xx) != 0);

        m_b = xy / xx;
        m_a = ya - m_b * xa;
        m_coeff = (fabs(yy) == 0) ? 1 : xy / sqrt(xx * yy);

    }

    //! Evaluates the linear regression function at the given abscissa.
    double getValue(double x) const
    {
        return m_a + m_b * x;
    }

    //! Returns the slope of the regression line
    double getSlope() const
    {
        return m_b;
    }

    //! Returns the intercept on the Y axis of the regression line
    double getIntercept() const
    {
        return m_a;
    }

    //! Returns the linear regression coefficient
    double getCoefficient() const
    {
        return m_coeff;
    }

private:
    double m_a, m_b, m_coeff;
};


class ImageUtils
{

public:
    ImageUtils();

    static std::vector<cv::Point> findLocalMaxima(cv::Mat, cv::Mat&, int);
    static QVector3D convert2DcoordCamera_to_3D(float _pixX, float _pixY, float _pixZ);
    static QVector3D convert2DcoordCamera_to_tool3D(float _pixX, float _pixY, float _pixZ, QVector3D _robotToolOrientation_RPYdeg);
    static QVector3D convert2DcoordCamera_to_3DcoordRobotBase_meter(float _pixX, float _pixY, float _pixZ, QVector3D _robotToolOrientation_RPYdeg, std::vector<double> _robotActualPose);
    static QVector3D convertObject3DcoordCamera_to_3DcoordRobot(QVector3D _coord3Dobj, QVector3D _robotToolOrientation_RPYdeg);
    static uint16_t depthMapGetDistance(QSharedPointer<uint16_t> &_imgDepthMap, uint32_t widthImg, uint32_t posX, uint32_t posY);
    static cv::Vec3i getHSVcolorOfCenterPixel(cv::Mat &img);
    static std::vector<uint32_t> getMeanHSVcolorAroundPixel(cv::Mat &img, uint32_t _xpos, uint32_t _ypos, uint32_t _nbPixelEachSideForMeanCalc = 5);
    static uint16_t convertDepthPixelFromCamSpaceToRobotSpace2(const int x, const int y, const int _depthMin, const int _depthRange, const QSharedPointer<uint16_t> _depthDatatoProcess, const QVector3D _robotToolOrientation_RPYdeg, const std::vector<double> _robot_actual_TCP_pose);

    static std::string getClosestStandardColorFromHSV(std::vector<uint32_t> _HSVcolor);
    static bool getColorClosestFromTwoColorsHSV(QVector3D _colorHSV, QVector3D _color1refHSV, QVector3D _color2refHSV);


    static std::vector<uint32_t> convertRGBtoHSV(std::vector<uint32_t> rgb);
    static std::vector<uint32_t> convertHSVtoRGB(std::vector<uint32_t> hsv);
    static vector<Point> bhFindLocalMaximum(Mat _src, int neighbor = 2);
    static Mat Binary2Color(Mat bwImg);
    static vector<Point> bhContoursCenter(const vector<vector<Point> > &contours, bool centerOfMass, int contourIdx = -1);
    static QVector3D convert3DcoordCamera_to_3DcoordRobotBase_meter(QVector3D point3D, QVector3D _robotToolOrientation_RPYdeg, std::vector<double> _robotActualPose);
    static bool Contains(RotatedRect rectangle, Point2f point);
};


class Utils3D
{
public:
    Utils3D();

    static void convertRotatedVectorRadToRPYDeg(QVector3D rotVector_rad, float &roll, float &pitch, float &yaw);
    static void convertRotatedVectorDegToRPYDeg(QVector3D rotVector_deg, float &roll, float &pitch, float &yaw);
    static QVector3D convertRPYDegToRotatedVectorDeg(double roll, double pitch, double yaw);
    static QVector3D bringDiffRPYDegToRotatedVectorDeg(QVector3D rotVector_deg, double diffRoll, double diffPitch, double diffYaw);
    static QVector3D getRotatedVectorRadFromSpecificRPY(QVector3D rotVector_deg, double diffRoll, double diffPitch, double diffYaw);
    static QVector3D getRotatedVectorDegFromSpecificRPY(QVector3D rotVector_deg, double newRoll, double newPitch, double newYaw);
};

class Utils2D
{
public:
    Utils2D();

    static double findAngleRad(cv::Point2f p0, cv::Point2f p1, cv::Point2f p2);
    static double findAngleDeg(cv::Point2f p0, cv::Point2f p1, cv::Point2f p2);
    static cv::Point2f  find2DPointFromAngleDeg(cv::Point2f  p0, cv::Point2f  p1, double angle);
};

class DetectionUtils
{
public:
    DetectionUtils();

    static QVector3D getLastPose(double speed, int deltaTime, cv::Point2f tableCenter, QVector3D actualPose);
};

class ConversionUtils
{
public:
    ConversionUtils();

    static std::vector<double> convertFloatTabToStdVectorDouble(float* floatTab, int size);
    static void convertStdVectorDoubleToFloatTab(std::vector<double> vectDouble, float* floatTab);
    static void convertPathStdVectorDoubleToPathFloatTab(std::vector<std::vector<double>> pathVectDouble, float **pathFloatTab, int sizeTab, int nbCoords);
};


#endif // CALC_UTILS_H
