#include "calculationsUtils.h"

//testGitDevOK

MovingAverage::MovingAverage(unsigned int period) :
    period(period),
    window(new double[period]),
    head(NULL),
    tail(NULL),
    total(0)
{
    assert(period >= 1);
}


MovingAverage::~MovingAverage()
{
    delete[] window;
}

void MovingAverage::add(double val)
{
    // Special case: Initialization
    if (head == NULL) {
        head = window;
        *head = val;
        tail = head;
        inc(tail);
        total = val;
        return;
    }

    // Were we already full?
    if (head == tail) {
        // Fix total-cache
        total -= *head;
        // Make room
        inc(head);
    }

    // Write the value in the next spot.
    *tail = val;
    inc(tail);

    // Update our total-cache
    total += val;
}

double MovingAverage::avg() const
{
    ptrdiff_t size = this->size();
    if (size == 0) {
        return 0; // No entries => 0 average
    }
    return total / (double) size; // Cast to double for floating point arithmetic
}

void MovingAverage::inc(double *&p)
{
    if (++p >= window + period) {
        p = window;
    }
}

ptrdiff_t MovingAverage::size() const
{
    if (head == NULL)
        return 0;

    if (head == tail)
        return period;

    return (period + tail - head) % period;
}


ImageUtils::ImageUtils()
{

}

bool ImageUtils::Contains(RotatedRect rectangle, Point2f point)
{
    //Get the corner points.
    Point2f corners[4];
    rectangle.points(corners);

    //Convert the point array to a vector.
    //https://stackoverflow.com/a/8777619/1997617
    Point2f* lastItemPointer = (corners + sizeof corners / sizeof corners[0]);
    vector<Point2f> contour(corners, lastItemPointer);

    //Check if the point is within the rectangle.
    double indicator = pointPolygonTest(contour, point, false);
    bool rectangleContainsPoint = (indicator >= 0);
    return rectangleContainsPoint;
}

vector<Point> ImageUtils::bhContoursCenter(const vector<vector<Point>>& contours,bool centerOfMass,int contourIdx)
{
    vector<Point> result;
    if (contourIdx > -1)
    {
        if (centerOfMass)
        {
            Moments m = moments(contours[contourIdx],true);
            result.push_back( Point(m.m10/m.m00, m.m01/m.m00));
        }
        else
        {
            Rect rct = boundingRect(contours[contourIdx]);
            result.push_back( Point(rct.x + rct.width / 2 ,rct.y + rct.height / 2));
        }
    }
    else
    {
        if (centerOfMass)
        {
            for (uint i=0; i < contours.size();i++)
            {
                Moments m = moments(contours[i],true);
                result.push_back( Point(m.m10/m.m00, m.m01/m.m00));
            }
        }
        else
        {
            for (uint i=0; i < contours.size(); i++)
            {
                Rect rct = boundingRect(contours[i]);
                result.push_back(Point(rct.x + rct.width / 2 ,rct.y + rct.height / 2));
            }
        }
    }
    return result;
}

vector<Point> ImageUtils::bhFindLocalMaximum(Mat _src,int neighbor){
    //https://m.blog.naver.com/PostView.nhn?blogId=hayandoud&logNo=220761386495&proxyReferer=https:%2F%2Fwww.google.com%2F

    Mat peak_img = _src.clone();
    dilate(peak_img,peak_img,Mat(),Point(-1,-1),neighbor);
    peak_img = peak_img - _src;

    Mat flat_img ;
    erode(_src,flat_img,Mat(),Point(-1,-1),neighbor);
    flat_img = _src - flat_img;

    threshold(peak_img,peak_img, 0, 255, cv::THRESH_BINARY);
//    cv::imshow("peak_img", peak_img);
    threshold(flat_img,flat_img, 0, 255, cv::THRESH_BINARY);
//    cv::imshow("flat_img", flat_img);
    bitwise_not(flat_img,flat_img);

    peak_img.setTo(Scalar::all(255),flat_img);
    bitwise_not(peak_img,peak_img);

    vector<vector<Point>> contours;
    findContours(peak_img,contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    return bhContoursCenter(contours,true);
}

Mat ImageUtils::Binary2Color(Mat bwImg)
{
    Mat colorImg;
    vector<Mat> channels;
    channels.push_back(bwImg);
    channels.push_back(bwImg);
    channels.push_back(bwImg);
    merge(channels, colorImg);

    return colorImg;
}

std::vector<cv::Point> ImageUtils::findLocalMaxima(cv::Mat src,cv::Mat &dst,int squareSize)
{
    if (squareSize==0)
    {
        dst = src.clone();
        std::vector<cv::Point> emptyVector = {};
        return emptyVector;
    }

    cv::Mat m0;
    dst = src.clone();
    cv::Point maxLoc(0,0);
    double max;

    //1.Be sure to have at least 3x3 for at least looking at 1 pixel close neighbours
    //  Also the window must be <odd>x<odd>
//    SANITYCHECK(squareSize,3,1);
    int sqrCenter = (squareSize-1)/2;

    //2.Create the localWindow mask to get things done faster
    //  When we find a local maxima we will multiply the subwindow with this MASK
    //  So that we will not search for those 0 values again and again
    cv::Mat localWindowMask = cv::Mat::zeros(cv::Size(squareSize,squareSize),CV_8U);//boolean
    localWindowMask.at<unsigned char>(sqrCenter,sqrCenter)=1;

    std::vector<cv::Point> pointTab;
    //3.Find the threshold value to threshold the image
        //this function here returns the peak of histogram of picture
        //the picture is a thresholded picture it will have a lot of zero values in it
        //so that the second boolean variable says :
        //  (boolean) ? "return peak even if it is at 0" : "return peak discarding 0"
//    int thrshld =  maxUsedValInHistogramData(dst,false);
//    cv::threshold(dst,m0,thrshld,1,THRESH_BINARY);

//    //4.Now delete all thresholded values from picture
//    dst = dst.mul(m0);

    //put the src in the middle of the big array
    for (int row = sqrCenter ; row < dst.size().height-sqrCenter ; row++)
        for (int col = sqrCenter; col < dst.size().width-sqrCenter ; col++)
        {
            //1.if the value is zero it can not be a local maxima
            if (dst.at<unsigned char>(row,col)==0)
                continue;
            //2.the value at (row,col) is not 0 so it can be a local maxima point
            m0 =  dst.colRange(col-sqrCenter,col+sqrCenter+1).rowRange(row-sqrCenter,row+sqrCenter+1);
            cv::minMaxLoc(m0,NULL,&max);
            //if the maximum location of this subWindow is at center
            //it means we found the local maxima
            //so we should delete the surrounding values which lies in the subWindow area
            //hence we will not try to find if a point is at localMaxima when already found a neighbour was
            if ((m0.at<unsigned char>(row,col)==max) && max>200)
            {
                qDebug() << "[CalculationsUtils] findLocalMaxima : Point is local maxima : x = " << row << "  y = " << col << "  value = " << m0.at<unsigned char>(row,col);
                pointTab.push_back(cv::Point(row+sqrCenter,col+sqrCenter));
                cv::drawMarker(dst, cv::Point(row+sqrCenter,col+sqrCenter), cv::Scalar( 0, 255, 0 ), cv::MARKER_CROSS, 5, 1);
                continue;
                m0 = m0.mul(localWindowMask);
//                                we can skip the values that we already made 0 by the above function
                col+=sqrCenter;
            }
        }

    qDebug() << "[CalculationsUtils] return tab point size = " << pointTab.size();
    return pointTab;
}

QVector3D ImageUtils::convert2DcoordCamera_to_3D(float _pixX, float _pixY, float _pixZ)
{
    constexpr float fx = 614.571;
    constexpr float fy = 614.704;
    constexpr float ppx = 316.73;
    constexpr float ppy = 237.401;

    float i = (_pixY - ppx) / fx;
    float j = (_pixX - ppy) / fy;

    float x = _pixZ * i;
    float y = _pixZ * j;
    float z = _pixZ;

    QVector3D point3D = QVector3D({x, y , z});

    return point3D;
}

QVector3D ImageUtils::convert2DcoordCamera_to_tool3D(float _pixX, float _pixY, float _pixZ, QVector3D _robotToolOrientation_RPYdeg)
{
    QVector3D point3D = convert2DcoordCamera_to_3D( _pixX,  _pixY,  _pixZ);
    point3D/=1000;

    QMatrix4x4 myMat0;
    myMat0.rotate(-27, 1, 0, 0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38° //26.54 -1 0 0
    QVector3D objectPos_toolref_noCamAngle = myMat0.map(point3D);
    QVector3D camTranslateToTool(-0.0378, -0.1028, -0.197); // first coord : line going to the right of the gripper, sec coord : to down of gripper, third : in line to the gripper
    QMatrix4x4 myMat1;
    myMat1.translate(camTranslateToTool);
    QVector3D objectPos_toolref_repCam = myMat1.map(objectPos_toolref_noCamAngle);

    QMatrix4x4 myMat2;
    myMat2.rotate(_robotToolOrientation_RPYdeg.x()-90.0, -1.0, 0.0, 0.0);
    QVector3D objectPos_toolref_noAngleTool = myMat2.map(objectPos_toolref_repCam);
    QVector3D objectPos_toolRef_refBase = QVector3D(-objectPos_toolref_noAngleTool.x(), -objectPos_toolref_noAngleTool.z(), -objectPos_toolref_noAngleTool.y());
    //On rajoute la rotation de la base sur l'axe Z :
    QMatrix4x4 myMat3;
    myMat3.rotate(_robotToolOrientation_RPYdeg.z(), 0.0, 0.0, 1.0);
    QVector3D objectPos_toolRef_refBase_RotZ = myMat3.map(objectPos_toolRef_refBase);
    //On rajoute la rotation de la base sur l'axe Z :
    QMatrix4x4 myMat4;
    myMat4.rotate(_robotToolOrientation_RPYdeg.y(), 0.0, 1.0, 0.0);
    QVector3D objectPos_toolRef_refBase_RotY = myMat4.map(objectPos_toolRef_refBase_RotZ);

    qDebug() << "Camera : " << point3D;
    qDebug() << "Rot : " << objectPos_toolref_noCamAngle;
    qDebug() << "Camera : " << objectPos_toolref_repCam;
    QVector3D pointPose = objectPos_toolref_repCam;

    return pointPose;
}

QVector3D ImageUtils::convert2DcoordCamera_to_3DcoordRobotBase_meter(float _pixX, float _pixY, float _pixZ, QVector3D _robotToolOrientation_RPYdeg, std::vector<double> _robotActualPose)
{
    constexpr float fx = 614.571;
    constexpr float fy = 614.704;
    constexpr float ppx = 316.73;
    constexpr float ppy = 237.401;

    float x = _pixZ * (_pixY - ppx) / fx;
    float y = _pixZ * (_pixX - ppy) / fy;
    float z = _pixZ;

    QVector3D point3D = QVector3D({x, y , z});

    // Rotation angle cam
    QMatrix4x4 myMat0;
//    myMat0.rotate(26.54, -1.0, 0.0, 0.0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38°
    myMat0.rotate(27, -1.0, 0.0, 0.0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38°

    QVector3D objectPos_toolref_noCamAngle = myMat0.map(point3D);
    QVector3D camTranslateToTool(-0.0378, -0.1028, -0.197); // first coord : line going to the right of the gripper, sec coord : to down of gripper, third : in line to the gripper
    QMatrix4x4 myMat1;
    myMat1.translate(camTranslateToTool);
    QVector3D objectPos_toolref_repCam = myMat1.map(objectPos_toolref_noCamAngle);
    QMatrix4x4 myMat2;
    myMat2.rotate(_robotToolOrientation_RPYdeg.x()-90.0, -1.0, 0.0, 0.0);
    QVector3D objectPos_toolref_noAngleTool = myMat2.map(objectPos_toolref_repCam);
    QVector3D objectPos_toolRef_refBase = QVector3D(-objectPos_toolref_noAngleTool.x(), -objectPos_toolref_noAngleTool.z(), -objectPos_toolref_noAngleTool.y());
    //On rajoute la rotation de la base sur l'axe Z :
    QMatrix4x4 myMat3;
    myMat3.rotate(_robotToolOrientation_RPYdeg.z(), 0.0, 0.0, 1.0);
    QVector3D objectPos_toolRef_refBase_RotZ = myMat3.map(objectPos_toolRef_refBase);

    QVector3D pointPose = objectPos_toolRef_refBase_RotZ;

    pointPose[0] = double(pointPose[0]) + double(_robotActualPose[0]);
    pointPose[1] = double(pointPose[1]) + double(_robotActualPose[1]);
    pointPose[2] = double(pointPose[2]) + double(_robotActualPose[2]);

    return pointPose;
}

QVector3D ImageUtils::convert3DcoordCamera_to_3DcoordRobotBase_meter(QVector3D point3D, QVector3D _robotToolOrientation_RPYdeg, std::vector<double> _robotActualPose)
{
    // Rotation angle cam
    QMatrix4x4 myMat0;
    myMat0.rotate(65, -5, -2, 0.0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38° //26.54 -1 0 0
    QVector3D objectPos_toolref_noCamAngle = myMat0.map(point3D);
    QVector3D camTranslateToTool(-0.0378, -0.1597, -0.197); // first coord : line going to the right of the gripper, sec coord : to down of gripper, third : in line to the gripper
    QMatrix4x4 myMat1;
    myMat1.translate(camTranslateToTool);
    QVector3D objectPos_toolref_repCam = myMat1.map(objectPos_toolref_noCamAngle);
    QMatrix4x4 myMat2;
    myMat2.rotate(_robotToolOrientation_RPYdeg.x()-90.0, -1.0, 0.0, 0.0);
    QVector3D objectPos_toolref_noAngleTool = myMat2.map(objectPos_toolref_repCam);
    QVector3D objectPos_toolRef_refBase = QVector3D(-objectPos_toolref_noAngleTool.x(), -objectPos_toolref_noAngleTool.z(), -objectPos_toolref_noAngleTool.y());
    //On rajoute la rotation de la base sur l'axe Z :
    QMatrix4x4 myMat3;
    myMat3.rotate(_robotToolOrientation_RPYdeg.z(), 0.0, 0.0, 1.0);
    QVector3D objectPos_toolRef_refBase_RotZ = myMat3.map(objectPos_toolRef_refBase);

    QVector3D pointPose = objectPos_toolRef_refBase_RotZ;

    pointPose[0] = double(pointPose[0]) + double(_robotActualPose[0]);
    pointPose[1] = double(pointPose[1]) + double(_robotActualPose[1]);
    pointPose[2] = double(pointPose[2]) + double(_robotActualPose[2]);

    return pointPose;
}


QVector3D ImageUtils::convertObject3DcoordCamera_to_3DcoordRobot(QVector3D _coord3Dobj, QVector3D _robotToolOrientation_RPYdeg)
{
    qDebug() << "Object pos par rapport à cam, in cam repère  = " << _coord3Dobj;

    //QVector3D(-0.05, 0.275, 0.34)  ,  (0.62*180/3.14)

    // Conversion point to point
    QVector3D objectPos_camRef = _coord3Dobj;

    // Rotation angle cam
    QMatrix4x4 myMat0;
//    myMat0.rotate(26.54, -1.0, 0.0, 0.0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38°
    myMat0.rotate(27, -1.0, 0.0, 0.0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38°
//    myMat0.rotate(27.4, -1.0, 0.0, 0.0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38°
    QVector3D objectPos_toolref_noCamAngle = myMat0.map(objectPos_camRef);

    qDebug() << "Object pos par rapport à la cam, in rotated cam coord system  = " << objectPos_toolref_noCamAngle; // for exemple objectPos_camRef = QVector3D(1.72922, 1.37391, 325) => QVector3D(1.72922, 201.173, 255.258)

    // Translation cam len to the end of the robot tool (in rotated angle cam coord system)
    // TEST // QVector3D camTranslateToTool(-0.032-0.005, -0.148-0.014, -0.185); // first coord : line going to the right of the gripper, sec coord : to down of gripper, third : in line to the gripper
//    QVector3D camTranslateToTool(-0.0378, -0.1597, -0.197); // first coord : line going to the right of the gripper, sec coord : to down of gripper, third : in line to the gripper
    QVector3D camTranslateToTool(-0.0378, -0.1028, -0.197); // first coord : line going to the right of the gripper, sec coord : to down of gripper, third : in line to the gripper
    QMatrix4x4 myMat1;
    myMat1.translate(camTranslateToTool);
    QVector3D objectPos_toolref_repCam = myMat1.map(objectPos_toolref_noCamAngle);

    qDebug() << "Object pos par rapport au tool (in rotated & translated to tool coord system) = " << objectPos_toolref_repCam;

    // And now the pos from ref tool to horizontal
    qDebug() << "Roll rot : " << _robotToolOrientation_RPYdeg.x() << " => rotation tool : " << _robotToolOrientation_RPYdeg.x()-90.0;
    QMatrix4x4 myMat2;
    myMat2.rotate(_robotToolOrientation_RPYdeg.x()-90.0, -1.0, 0.0, 0.0);
    QVector3D objectPos_toolref_noAngleTool = myMat2.map(objectPos_toolref_repCam);

    qDebug() << "Object pos par rapport au tool, in horizontal Z coord system (" << _robotToolOrientation_RPYdeg.x()-90.0 << ") =" << objectPos_toolref_noAngleTool;

    /*
    QMatrix4x4 myMat2;
    myMat2.rotate(90, 0.0, 0.0, 1.0);
    myMat2.rotate(90, 1.0, 0.0, 0.0);
    QVector3D objectPos_toolref_repTool = myMat2.map(objectPos_toolref_repCam);*/
    // Rotate coordinates camera to coordinates tool (2x 90°)
    QVector3D objectPos_toolRef_refBase = QVector3D(-objectPos_toolref_noAngleTool.x(), -objectPos_toolref_noAngleTool.z(), -objectPos_toolref_noAngleTool.y());

    qDebug() << "Object pos par rapport au tool, in base coord system = " << objectPos_toolRef_refBase;

    //On rajoute la rotation de la base sur l'axe Z :
    QMatrix4x4 myMat3;
    myMat3.rotate(_robotToolOrientation_RPYdeg.z(), 0.0, 0.0, 1.0);
    QVector3D objectPos_toolRef_refBase_RotZ = myMat3.map(objectPos_toolRef_refBase);

    qDebug() << "Object pos par rapport au tool, in base coord system avec rotation de la base = " << objectPos_toolRef_refBase_RotZ;

    return objectPos_toolRef_refBase_RotZ;
}

uint16_t ImageUtils::depthMapGetDistance(QSharedPointer<uint16_t> &_imgDepthMap, uint32_t widthImg, uint32_t posX, uint32_t posY)
{
    uint16_t val = _imgDepthMap.data()[(posY*widthImg) +  posX];
    return val;
}


cv::Vec3i ImageUtils::getHSVcolorOfCenterPixel(cv::Mat &img)
{
//    if (m_showImgDebugOnOff) {
//        cv::imshow("01 - My Image", img);
//    }

    // filter by skin color
    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_RGB2HSV);

    //if (m_showImgDebugOnOff) {
        //cv::drawMarker(imgHLS, cv::Vec2i(640/2, 480/2), cv::Scalar( 255,0,0 ), cv::MARKER_DIAMOND, 20, 2);

        cv::Mat HSV_pixel;
        cv::Mat BGR_pixel=img(cv::Rect(640/2,480/2,1,1)); // use your x and y value

        cv::cvtColor(BGR_pixel, HSV_pixel,cv::COLOR_RGB2HSV);
        cv::Vec3b hsv_pixcolor=HSV_pixel.at<cv::Vec3b>(0,0);
        int H=double(hsv_pixcolor.val[0])/255.0*100.0; //hue
        int S=double(hsv_pixcolor.val[1])/255.0*100.0; //saturation
        int V=double(hsv_pixcolor.val[2])/255.0*100.0; //value
        //qDebug() << "H = " << H << "S = " << S << "V = " << V;
        //cv::Mat matColor(50, 200, CV_8UC3, cv::Scalar(BGR_pixel.ptr(0,0)[2], BGR_pixel.ptr(0,0)[1], BGR_pixel.ptr(0,0)[0]));

    //}

        return cv::Vec3i(H, S, V);
}

// Return the mean color HSV as a std::vector<uint32_t> [3]
std::vector<uint32_t> ImageUtils::getMeanHSVcolorAroundPixel(cv::Mat &img, uint32_t _xpos, uint32_t _ypos, uint32_t _nbPixelEachSideForMeanCalc)
{
        // filter by skin color
        cv::Mat pixelsAround = img(cv::Rect(_xpos-_nbPixelEachSideForMeanCalc, _ypos-_nbPixelEachSideForMeanCalc, _nbPixelEachSideForMeanCalc*2, _nbPixelEachSideForMeanCalc*2)); // use your x and y value

        uint32_t sumB=0, sumG=0, sumR=0;
        uint32_t nbPixels = 0;

        for (int x = 0; x < pixelsAround.rows; x++)
        {
            for (int y = 0; y < pixelsAround.cols; y++)
            {
                cv::Vec3b pixel = pixelsAround.at<cv::Vec3b>(x, y);
                sumB += (pixel.val[0]);
                sumG += (pixel.val[1]);
                sumR += (pixel.val[2]);
                nbPixels++;
            }
        }
        uint32_t meanB = sumB/nbPixels;
        uint32_t meanG = sumG/nbPixels;
        uint32_t meanR = sumR/nbPixels;

//        cv::Mat matColor(50, 200, CV_8UC3, cv::Scalar(meanB, meanG, meanR));

//        qDebug() << "R = " << meanR << " G = " << meanG << " B = " << meanB;
        //std::vector<uint32_t> RGBval =
        std::vector<uint32_t> resMeanHSV = convertRGBtoHSV(std::vector<uint32_t>({meanR, meanG, meanB}));
//        qDebug() << "H = " << resHSV[0] << " S = " << resHSV[1] << " V = " << resHSV[2];

        return resMeanHSV;
}

std::string ImageUtils::getClosestStandardColorFromHSV(std::vector<uint32_t> _HSVcolor)
{

    // HSV Distance
    std::string closestColor;
    QVector3D vec3D_currentColorHSV(_HSVcolor[0], _HSVcolor[1], _HSVcolor[2]);
    QVector3D vec3D_colorRefHSV_orange(12, 99, 32); // orange
    //QVector3D vec3D_color3refHSV(138, 30, 33); // green
    QVector3D vec3D_colorRefHSV_green(120, 50, 23); // green
    QVector3D vec3D_colorRefHSV_darkGray(205, 10, 22); // dark gray


    if (_HSVcolor[2]<15) // black
    {
        qDebug() << "Dist to black color = LOW V detected : " << _HSVcolor[2] << " color black";
        closestColor = "Black";
    }
    else
    {
        QPoint distToOrangeVec = QPoint(vec3D_currentColorHSV[0], vec3D_currentColorHSV[1]) - QPoint(vec3D_colorRefHSV_orange[0], vec3D_colorRefHSV_orange[1]);
        double distToOrangeDbl = std::sqrt(std::pow(distToOrangeVec.x(), 2) + std::pow(distToOrangeVec.y(), 2));
        QPoint distToGreenVec = QPoint(vec3D_currentColorHSV[0], vec3D_currentColorHSV[1]) - QPoint(vec3D_colorRefHSV_green[0], vec3D_colorRefHSV_green[1]);
        double distToGreenDbl = std::sqrt(std::pow(distToGreenVec.x(), 2) + std::pow(distToGreenVec.y(), 2));
        QPoint distToDarkGrayVec = QPoint(vec3D_currentColorHSV[0], vec3D_currentColorHSV[1]) - QPoint(vec3D_colorRefHSV_darkGray[0], vec3D_colorRefHSV_darkGray[1]);
        double distToDarkGrayDbl = std::sqrt(std::pow(distToDarkGrayVec.x(), 2) + std::pow(distToDarkGrayVec.y(), 2));

        qDebug() << "Dist to orange color = " << distToOrangeDbl;
        qDebug() << "Dist to green color = " << distToGreenDbl;
        qDebug() << "Dist to dark gray color = " << distToDarkGrayDbl;

        if (distToOrangeDbl < 40) {
            closestColor = "Orange";
        }
        else if (distToGreenDbl < 40) {
            closestColor = "Green";
        }
        else if (distToDarkGrayDbl < 50) {
            closestColor = "Black";
        }
        else {
            closestColor = "- undef.";
        }
    }

//        char str[200];
//        sprintf(str,"H=%d, S=%d, V=%d => %s", resHSV[0], resHSV[1], resHSV[2], closestColor.c_str());
//        cv::putText(matColor, str, cv::Point2f(20,20), cv::FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(0,255,0,255));
//        cv::imshow("87 - Sample mean color", matColor);

    return closestColor;
}

// Return true if _colorHSV is closer to _color1refHSV, and return in the other case
bool ImageUtils::getColorClosestFromTwoColorsHSV(QVector3D _colorHSV, QVector3D _color1refHSV, QVector3D _color2refHSV)
{
    // HSV Distance
    if (_colorHSV.distanceToPoint(_color1refHSV) < _colorHSV.distanceToPoint(_color2refHSV)) {
        return true;
    }
    else{
        return false;
    }
}



// RGB input as 0-255 vector[3]
// HSV return as 0-360°, 0-100%, 0-100% HSV value
std::vector<uint32_t> ImageUtils::convertRGBtoHSV(std::vector<uint32_t> rgb)
{
    if (rgb[0]>255 || rgb[1]>255 || rgb[2]>255)
    {
        qDebug() << "The given RGB values are not in valid range";
        throw;
    }

    std::vector<uint32_t> retValHSV;
    retValHSV.resize(3);

    double hue = 0.0, sat = 0.0;
    double maxval, minval;

    maxval=*max_element(rgb.begin(), rgb.end());
    minval=*min_element(rgb.begin(), rgb.end());

    double difference = maxval-minval;
    double red, green, blue;
    red = double(rgb[0]);
    green = double(rgb[1]);
    blue = double(rgb[2]);

    if(difference==0)
        hue=0;
    else if( red==maxval)
        hue = fmod(((60.0*((green-blue)/difference))+360.0), 360.0);
    else if (green==maxval)
        hue = fmod(((60.0*((blue-red)/difference))+120.0), 360.0);
    else if (blue==maxval)
        hue=  fmod(((60.0*((red-green)/difference))+240.0),360.0);

    retValHSV[0] = (uint32_t)round(hue);

    if(maxval==0)
        sat=0;
    else
        sat=100*(difference/maxval);

    retValHSV[1] = (uint32_t)round(sat);
    retValHSV[2] = (uint32_t)round(maxval*100.0/255.0);

    return retValHSV;
}

std::vector<uint32_t> ImageUtils::convertHSVtoRGB(std::vector<uint32_t> hsv)
{
    double H = hsv[0];
    double S = hsv[1];
    double V = hsv[2];

    if (H>360 || S>100 || V>100 )
    {
        qDebug() << "The givem HSV values are not in valid range";
        throw;
    }

    double s = S/100;
    double v = V/100;
    double C = s*v;
    double X = C*(1-abs(fmod(H/60.0, 2)-1));
    double m = v-C;
    double r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }

    std::vector<uint32_t> retValRGB;
    retValRGB.resize(3);

    retValRGB[0] = (uint32_t)round((r+m)*255);
    retValRGB[1] = (uint32_t)round((g+m)*255);
    retValRGB[2] = (uint32_t)round((b+m)*255);

    return retValRGB;
}

uint16_t ImageUtils::convertDepthPixelFromCamSpaceToRobotSpace2(const int x, const int y, const int _depthMin, const int _depthRange,  const QSharedPointer<uint16_t> _depthDatatoProcess, const QVector3D _robotToolOrientation_RPYdeg, const std::vector<double> _robot_actual_TCP_pose)
{
    constexpr int32_t w_frame = 640;
    constexpr int32_t h_frame = 480;
    const cv::Mat depthMap_mat(cv::Size(w_frame, h_frame), CV_16U, (void*)_depthDatatoProcess.data(), cv::Mat::AUTO_STEP);

    float distance = _depthDatatoProcess.data()[(y*depthMap_mat.cols) +  x];

    constexpr float fx = 614.571;
    constexpr float fy = 614.704;
    constexpr float ppx = 316.73;
    constexpr float ppy = 237.401;
    float i = (float(x) - ppx) / fx;
    float j = (float(y) - ppy) / fy;
    QVector3D point(distance*i/1000.0, distance*j/1000.0, distance/1000.0);

    // Rotation angle cam
    QMatrix4x4 myMat0;
    myMat0.rotate(-27, 1, 0, 0); // mesure laser : 53°+-1° en fct des mesures, et on releve jusqu'à 90° => 38° //26.54 -1 0 0
    QVector3D objectPos_toolref_noCamAngle = myMat0.map(point);
    QVector3D camTranslateToTool(-0.0378, -0.1028, -0.197); // first coord : line going to the right of the gripper, sec coord : to down of gripper, third : in line to the gripper
    QMatrix4x4 myMat1;
    myMat1.translate(camTranslateToTool);
    QVector3D objectPos_toolref_repCam = myMat1.map(objectPos_toolref_noCamAngle);
    QMatrix4x4 myMat2;
    myMat2.rotate(_robotToolOrientation_RPYdeg.x()-90.0, -1.0, 0.0, 0.0);
    QVector3D objectPos_toolref_noAngleTool = myMat2.map(objectPos_toolref_repCam);
    QVector3D objectPos_toolRef_refBase = QVector3D(-objectPos_toolref_noAngleTool.x(), -objectPos_toolref_noAngleTool.z(), -objectPos_toolref_noAngleTool.y());
    //On rajoute la rotation de la base sur l'axe Z :
    QMatrix4x4 myMat3;
    myMat3.rotate(_robotToolOrientation_RPYdeg.z(), 0.0, 0.0, 1.0);
    QVector3D objectPos_toolRef_refBase_RotZ = myMat3.map(objectPos_toolRef_refBase);
    //On rajoute la rotation de la base sur l'axe Z :
    QMatrix4x4 myMat4;
    myMat4.rotate(_robotToolOrientation_RPYdeg.y(), 0.0, 1.0, 0.0);
    QVector3D objectPos_toolRef_refBase_RotY = myMat4.map(objectPos_toolRef_refBase_RotZ);

    QVector3D pointPose = objectPos_toolRef_refBase_RotY;

    pointPose[0] = float(pointPose[0]) + float(_robot_actual_TCP_pose[0]);
    pointPose[1] = float(pointPose[1]) + float(_robot_actual_TCP_pose[1]);
    pointPose[2] = float(pointPose[2]) + float(_robot_actual_TCP_pose[2]);

    constexpr float zero_val_mm =-20.0;

    float zero_val_offset = _depthMin;
    float z_val_max_mm = _depthRange+_depthMin;

    float z_val = ((pointPose[2]*1000.0)-zero_val_mm)*(255.0/z_val_max_mm);

    if (z_val < (zero_val_offset)*(255.0/z_val_max_mm))
        z_val=0;
    else if(z_val > 250.0)
        z_val=0;

    return (uint16_t)z_val;
}


Utils3D::Utils3D()
{

}
void Utils3D::convertRotatedVectorRadToRPYDeg(QVector3D rotVector_rad, float &roll, float &pitch, float &yaw)
{
    double rotX = (rotVector_rad.x()*180)/M_PI;
    double rotY = (rotVector_rad.y()*180)/M_PI;
    double rotZ = (rotVector_rad.z()*180)/M_PI;

    QVector3D rotVector_deg(rotX, rotY, rotZ);

    qDebug() << "angle rad : " << rotVector_rad.x() << " " << rotVector_rad.y() << " " << rotVector_rad.z();
    qDebug() << "angle deg : " << rotVector_deg.x() << " " << rotVector_deg.y() << " " << rotVector_deg.z();

    QQuaternion convAxisToEuler;
    convAxisToEuler = QQuaternion::fromAxisAndAngle(rotVector_deg.normalized().y(),
                                                    rotVector_deg.normalized().z(),
                                                    rotVector_deg.normalized().x(),
                                                    rotVector_deg.length());
    convAxisToEuler.getEulerAngles(&pitch, &yaw, &roll);
    qDebug() << "angle rpy : " << pitch << " " <<  yaw << " " << roll;

}

void Utils3D::convertRotatedVectorDegToRPYDeg(QVector3D rotVector_deg, float &roll, float &pitch, float &yaw)
{
    QQuaternion convAxisToEuler;
    convAxisToEuler = QQuaternion::fromAxisAndAngle(rotVector_deg.normalized().y(),
                                                    rotVector_deg.normalized().z(),
                                                    rotVector_deg.normalized().x(),
                                                    rotVector_deg.length());
    convAxisToEuler.getEulerAngles(&pitch, &yaw, &roll);
}

QVector3D Utils3D::convertRPYDegToRotatedVectorDeg(double roll, double pitch, double yaw)
{
    //qDebug() << "From RPY : " << roll << "°, " << pitch << "°, " << yaw << "°";
    QQuaternion eulerToAxis;
    eulerToAxis = QQuaternion::fromEulerAngles(pitch, yaw, roll);
    QVector3D rotvector;
    float angle;
    eulerToAxis.getAxisAndAngle(&rotvector, &angle);

    rotvector *= angle;
    //qDebug() << "Result rot vector : " << rotvector;

    return QVector3D(rotvector.z(), rotvector.x(), rotvector.y());
}

QVector3D Utils3D::getRotatedVectorRadFromSpecificRPY(QVector3D rotVector_deg, double newRoll, double newPitch, double newYaw)
{
    //Convert rotated vector to RPY
    float roll, pitch, yaw;
    convertRotatedVectorDegToRPYDeg(rotVector_deg, roll, pitch, yaw);

    //Bring diff
    if(newRoll != 0) roll = newRoll;
    if(newPitch != 0) pitch = newPitch;
    if(newYaw != 0) yaw = newYaw;

    //Convert RPY to rotated vector
    QVector3D finalRotatedVectorDeg;
    finalRotatedVectorDeg = convertRPYDegToRotatedVectorDeg(roll, pitch, yaw);
    QVector3D finalRotatedVectorRad(finalRotatedVectorDeg.x()*M_PI/180, finalRotatedVectorDeg.y()*M_PI/180, finalRotatedVectorDeg.z()*M_PI/180);

    return finalRotatedVectorRad;
}

QVector3D Utils3D::getRotatedVectorDegFromSpecificRPY(QVector3D rotVector_deg, double newRoll, double newPitch, double newYaw)
{
    qDebug() << "rotVector_deg" << rotVector_deg;
    qDebug() << "newYaw" << newYaw;

    //Convert rotated vector to RPY
    float roll, pitch, yaw;
    convertRotatedVectorDegToRPYDeg(rotVector_deg, roll, pitch, yaw);

    //Bring diff
    if(newRoll != 0) roll = newRoll;
    if(newPitch != 0) pitch = newPitch;
    if(newYaw != 0) yaw = newYaw;

    //Convert RPY to rotated vector
    QVector3D finalRotatedVectorDeg;
    finalRotatedVectorDeg = convertRPYDegToRotatedVectorDeg(roll, pitch, yaw);
    QVector3D finalRotatedVectorRad(finalRotatedVectorDeg.x(), finalRotatedVectorDeg.y(), finalRotatedVectorDeg.z());

    return finalRotatedVectorRad;
}


Utils2D::Utils2D()
{

}

double Utils2D::findAngleRad(cv::Point2f p0, cv::Point2f p1, cv::Point2f p2)
{
    double a = pow(p1.x-p0.x,2) + pow(p1.y-p0.y,2);
    double b = pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2);
    double c = pow(p2.x-p0.x,2) + pow(p2.y-p0.y,2);
    double angleRad = acos( (a+b-c) / sqrt(4*a*b) );
    return angleRad;
}

double Utils2D::findAngleDeg(cv::Point2f p0, cv::Point2f p1, cv::Point2f p2)
{
    qDebug() << "debug points : ";
    qDebug() << p0.x << " " << p0.y;
    qDebug() << p1.x << " " << p1.y;
    qDebug() << p2.x << " " << p2.y;
    double a = pow(p1.x-p0.x,2) + pow(p1.y-p0.y,2);
    double b = pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2);
    double c = pow(p2.x-p0.x,2) + pow(p2.y-p0.y,2);
    double angleDeg = acos( (a+b-c) / sqrt(4*a*b) );
    angleDeg *= 180/M_PI;
    return angleDeg;
}

cv::Point2f Utils2D::find2DPointFromAngleDeg(cv::Point2f centre, cv::Point2f  p1, double angleDiff)
{
//    qDebug() << "find2DPointFromAngleDeg";

    double actualAngle = findAngleDeg(cv::Point2f(0.16, -0.613), centre, p1);
    qDebug() << centre.y << "  " << p1.y;
    if(centre.y > p1.y) actualAngle = -actualAngle;

//    qDebug() << "angle actual : " << actualAngle;

    QVector2D qCentre = QVector2D(centre.x, centre.y);
    QVector2D qp1 = QVector2D(p1.x, p1.y);

    double rayon = qCentre.distanceToPoint(qp1);
//    qDebug() << " rayon = " << rayon;

    double previousAngle = actualAngle - angleDiff;
//    qDebug() << " previousAngle deg = " << previousAngle;
    previousAngle *= M_PI/180;
//    qDebug() << " previousAngle rad = " << previousAngle;

    double newX = centre.x + rayon*cos(previousAngle);
    double newY = centre.y + rayon*sin(previousAngle);

//    qDebug() << "coord = " << newX << "   "  << newY;

    cv::Point2f thirdPoint(newX,newY);
    QVector2D qp2 = QVector2D(thirdPoint.x, thirdPoint.y);
//    qDebug() << " rayon 2 = " << qCentre.distanceToPoint(qp2);

    return thirdPoint;
}

DetectionUtils::DetectionUtils()
{

}

QVector3D DetectionUtils::getLastPose(double speed, int deltaTime, cv::Point2f tableCenter, QVector3D actualPose)
{
    double deltaTimeSec = (double)deltaTime/1000;
    double deltaAngle = speed*deltaTimeSec;
    qDebug() << "deltaTimeMSec : " << deltaTime;
    qDebug() << "deltaTimeSec : " << deltaTimeSec;
    qDebug() << "deltaAngle : " << deltaAngle;
    cv::Point2f previousPoint = Utils2D::find2DPointFromAngleDeg(tableCenter, cv::Point2f(actualPose.x(),actualPose.y()), deltaAngle);
    QVector3D lastPose(previousPoint.x, previousPoint.y, actualPose.z());

    return lastPose;
}

ConversionUtils::ConversionUtils()
{

}

std::vector<double> ConversionUtils::convertFloatTabToStdVectorDouble(float* floatTab, int size)
{
    std::vector<float> vectFloat;

    vectFloat.assign(floatTab, floatTab + size);
    std::vector<double> vectDouble(vectFloat.begin(), vectFloat.end());

    return vectDouble;
}

void ConversionUtils::convertStdVectorDoubleToFloatTab(std::vector<double> vectDouble, float* floatTab)
{
    std::vector<double> vectFloat(vectDouble.begin(), vectDouble.end());

    for(int i=0; i<vectFloat.size(); i++)
    {
        floatTab[i] = (float)vectFloat[i];
    }
}


void ConversionUtils::convertPathStdVectorDoubleToPathFloatTab(std::vector<std::vector<double>> pathVectDouble, float **pathFloatTab, int sizeTab, int nbCoords)
{
    for(int i=0; i<sizeTab; i++)
    {
        for(int j=0; j<nbCoords; j++)
        {
            pathFloatTab[i][j] = pathVectDouble[i][j];
        }
    }
}

