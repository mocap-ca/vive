
//====================================================================================-----
//== Motive API DLL
//== Copyright NaturalPoint, Inc.
//==
//== The Motive API is designed to be a simple yet full featured interface to Motive
//==
//====================================================================================-----

#ifndef NPTRACKINGTOOLS_H
#define NPTRACKINGTOOLS_H

//== Includes ========================================================================-----

#include "trackablesettings.h"

//== DLL EXPORT/IMPORT PREPROCESSOR DEFINES ==========================================-----

#ifdef NPTRACKINGTOOLS_EXPORTS
#define TTAPI __declspec(dllexport)
#else
#ifndef STATIC_TT_LINK
#define TTAPI __declspec(dllimport)
#else
#define TTAPI  
#endif
#endif

namespace CameraLibrary
{
    class Camera;
    class CameraManager;
    class cCameraModule;
}

#ifndef _CORE_UID_CLASS
#define _CORE_UID_CLASS

namespace Core
{
    /// <summary>
    /// A platform-neutral 128-bit universal identifier.
    /// </summary>
    class cUID
    {
    public:
        /// <summary>
        /// Create a default cUID. In order to create a cUID that has a valid unique identifier you
        /// must call Generate() after creating a cUID object.
        /// </summary>
        cUID() : mHighBits( 0 ), mLowBits( 0 ) { }

        cUID( long long high, long long low ) : mHighBits( high ), mLowBits( low ) { }
        cUID( const cUID & obj ) : mHighBits( obj.mHighBits ), mLowBits( obj.mLowBits ) { }
        cUID&            operator=( const cUID & rhs ) { mHighBits = rhs.mHighBits; mLowBits = rhs.mLowBits; return *this; }

        /// <summary>
        /// Set the value of the cUID from two long integer values. It is up to the caller to ensure that
        /// the resulting cUID is unique.
        /// </summary>
        void            SetValue( long long highBits, long long lowBits ) { mHighBits = highBits; mLowBits = lowBits; }

        /// <summary>Get the low 64 bits of the cUID.</summary>
        long long       LowBits() const { return mLowBits; }

        /// <summary>Get the high 64 bits of the cUID.</summary>
        long long       HighBits() const { return mHighBits; }

        //====================================================================================
        // Comparison operators
        //====================================================================================

        bool            operator<( const cUID & rhs ) const
        {
            return ( ( mHighBits < rhs.mHighBits ) ? true : ( mHighBits == rhs.mHighBits ? ( mLowBits < rhs.mLowBits ) : false ) );
        }
        bool            operator<=( const cUID & rhs ) const
        {
            return ( ( mHighBits < rhs.mHighBits ) ? true : ( mHighBits == rhs.mHighBits ? ( mLowBits <= rhs.mLowBits ) : false ) );
        }
        bool            operator>( const cUID & rhs ) const { return !( *this <= rhs ); }
        bool            operator>=( const cUID & rhs ) const { return !( *this < rhs ); }
        bool            operator==( const cUID & rhs ) const
        {
            return ( ( mHighBits == rhs.mHighBits ) && ( mLowBits == rhs.mLowBits ) );
        }
        bool            operator!=( const cUID & rhs ) const { return !( *this == rhs ); }

        //====================================================================================
        // Constants
        //====================================================================================

        static const cUID Invalid;

    private:
        long long       mHighBits;
        long long       mLowBits;
    };
}

#endif // _CORE_UID_CLASS

#ifndef _CORE_MARKER_CLASS
#define _CORE_MARKER_CLASS

namespace Core
{
    template <typename T>
    class cTMarker
    {
    public:
        cTMarker() : X( 0 ), Y( 0 ), Z(0), Size( 0 ), Residual( 0 ), ReconstructionID( 0 ), Synthetic( false )  { }
        cTMarker( T x, T y, T z ) : X( x ), Y( y ), Z( z ), Residual( 0 ), ReconstructionID( 0 ), Synthetic( false )  { }

        cTMarker&        operator=( const cTMarker &base )
        {
            X = base.X;
            Y = base.Y;
            Z = base.Z;

            Size = base.Size;
            ID = base.ID;
            Residual = base.Residual;
            ReconstructionID = base.ReconstructionID;
            Synthetic = base.Synthetic;

            return *this;
        }

        /// <summary>Set the position.</summary>
        void            Set( T x, T y, T z ) { X = x; Y = y; Z = z; }

        T               DistanceSquared( const cTMarker *other ) const
        {
            T           x = X - other->X;
            T           y = Y - other->Y;
            T           z = Z - other->Z;

            return x * x + y * y + z * z;
        }

        T               Distance( const cTMarker *marker ) const
        {
            return sqrt( DistanceSquared( marker ) );
        }

        T               X;        //== Position in meters ==
        T               Y;        //== Position in meters ==
        T               Z;        //== Position in meters ==
        T               Size;     //== Size in meters ======
        cUID            ID;       //== Marker Label ========
        T               Residual; //== Residual in mm/ray ==
        int             ReconstructionID; //== ID assigned during reconstruction ==
        bool            Synthetic;
    };

    typedef cTMarker<float> cMarker;
    typedef cTMarker<float> cMarkerf;
    typedef cTMarker<double> cMarkerd;
}

#endif // _CORE_MARKER_CLASS

//== PUBLIC INTERFACE ===============================================================-----

#define NPRESULT int                  //== NPRESULT Defines Call Success/Failure ====-----

//== STARTUP / SHUTDOWN =============================================================-----

TTAPI   NPRESULT TT_Initialize();      //== Initialize Library ======================-----
TTAPI   NPRESULT TT_Shutdown();        //== Shutdown Library ========================-----
TTAPI   NPRESULT TT_FinalCleanup();    //== This shuts down device driver. Call =====-----
                                       //== this before exiting your application. ===-----

//== RIGID BODY INTERFACE ===========================================================-----

TTAPI   NPRESULT TT_LoadCalibration(const char *filename); //== Load Calibration =====----
TTAPI   NPRESULT TT_LoadTrackables (const char *filename); //== Load Trackables ======----
TTAPI   NPRESULT TT_SaveTrackables (const char *filename); //== Save Trackables ======----
TTAPI   NPRESULT TT_AddTrackables  (const char *filename); //== Add  Trackables ======----
TTAPI   NPRESULT TT_Update();                          //== Process incoming camera data -
TTAPI   NPRESULT TT_UpdateSingleFrame();               //== Process incoming camera data -
TTAPI   NPRESULT TT_LoadProject(const char *filename); //== Load Project File ==========--
TTAPI   NPRESULT TT_SaveProject(const char *filename); //== Save Project File ==========--

TTAPI   NPRESULT TT_LoadCalibrationFromMemory(unsigned char* buffer, int bufferSize);

//== DATA STREAMING =================================================================-----

TTAPI   NPRESULT TT_StreamTrackd(bool enabled);        //== Start/stop Trackd Stream =----
TTAPI   NPRESULT TT_StreamVRPN(bool enabled, int port);//== Start/stop VRPN Stream ===----
TTAPI   NPRESULT TT_StreamNP(bool enabled);            //== Start/stop NaturalPoint Stream 

//== FRAME ==========================================================================-----

TTAPI   int      TT_FrameMarkerCount();               //== Returns Frame Markers Count ---
TTAPI   float    TT_FrameMarkerX(int index);          //== Returns X Coord of cMarker -----
TTAPI   float    TT_FrameMarkerY(int index);          //== Returns Y Coord of cMarker -----
TTAPI   float    TT_FrameMarkerZ(int index);          //== Returns Z Coord of cMarker -----
TTAPI   int      TT_FrameMarkerLabel(int index);      //== Returns Label of cMarker -------
TTAPI   double   TT_FrameTimeStamp();                 //== Time Stamp of Frame (seconds) -

                 //== TT_FrameCameraCentroid returns true if the camera is contributing
                 //== to this 3D marker.  It also returns the location of the 2D centroid
                 //== that is reconstructing to this 3D marker.

TTAPI   bool     TT_FrameCameraCentroid(int index, int cameraIndex, float &x, float &y);

                 //== In the event that you are tracking a very high number of 2D and/or
                 //== 3D markers (hundreds of 3D markers), and you find that the data
                 //== you're getting out has sufficient latency you can call
                 //== TT_FlushCameraQueues() to catch up before calling TT_Update().
                 //== Ideally, after calling TT_FlushCameraQueues() you'll want to
                 //== not call it again until after TT_Update() returns NPRESULT_SUCCESS

TTAPI   void     TT_FlushCameraQueues();

//== RIGID BODY CONTROL =============================================================-----

TTAPI   bool     TT_IsTrackableTracked(int index); //== Is trackable currently tracked ---
TTAPI   void     TT_TrackableLocation(int index,      //== Trackable Index ============---
                        float *x, float *y, float *z,                  //== Position ==---
                        float *qx, float *qy, float *qz, float *qw,    //== Orientation -- 
                        float *yaw, float *pitch, float *roll);        //== Orientation --

TTAPI   void     TT_ClearTrackableList();             //== Clear all trackables   =====---
TTAPI   NPRESULT TT_RemoveTrackable(int Index);       //== Remove single trackable ====---
TTAPI   int      TT_TrackableCount();                 //== Returns number of trackables  -

TTAPI   int      TT_TrackableID(int index);           //== Get Trackables ID ==========---
TTAPI   void     TT_SetTrackableID(int index,int ID); //== Set Trackables ID ==========---
TTAPI   const char* TT_TrackableName(int index);      //== Returns Trackable Name =====---

TTAPI   void     TT_SetTrackableEnabled(int index, bool enabled);    //== Set Tracking   ====---
TTAPI   bool     TT_TrackableEnabled(int index);                     //== Get Tracking   ====---

TTAPI   NPRESULT TT_TrackableTranslatePivot(int index, float x, float y, float z);

TTAPI   int      TT_TrackableMarkerCount(int index);             //== Get marker count   ====---
TTAPI   void     TT_TrackableMarker(int rigidIndex,              //== Get Trackable mrkr ====---
                        int markerIndex, float *x, float *y, float *z);
TTAPI   void     TT_TrackablePointCloudMarker(int rigidIndex,    //== Get corresponding point cloud marker ======---
                        int markerIndex, bool &tracked,          //== If tracked is false, there is no
                        float &x, float &y, float &z);           //== corresponding point cloud marker.

                 //== TT_CreateTrackable.  This creates a rigid body based on the marker
                 //== count and marker list provided.  The MarkerList is a expected to
                 //== contain of list of marker coordinates in the order: x1,y1,z1,x2,
                 //== y2,z2,...xN,yN,zN.
TTAPI   NPRESULT TT_CreateTrackable(const char* name, int id, int markerCount,
                                    float *markerList);

TTAPI   NPRESULT TT_TrackableSettings   (int index, cTrackableSettings &settings);  //== Get Trackable Settings =---
TTAPI   NPRESULT TT_SetTrackableSettings(int index, cTrackableSettings &settings);  //== Set Trackable Settings =---

//== CAMERA MANAGER ACCESS ====================================================================================-----

TTAPI   CameraLibrary::CameraManager* TT_GetCameraManager();    //== Returns a pointer to the Camera SDK's  =====---
                                                                //== CameraManager ==============================---

TTAPI   int	     TT_BuildNumber();                    //== Software Release Build # =============================---

//== CAMERA GROUP SUPPORT =======================================================================================---

TTAPI   int      TT_CameraGroupCount();               //== Returns number of camera groups ======================---
TTAPI   bool     TT_CreateCameraGroup();              //== Add an additional group ==============================---
TTAPI   bool     TT_RemoveCameraGroup(int index);     //== Remove a camera group (must be empty) ================---
TTAPI   int      TT_CamerasGroup(int index);          //== Returns Camera's camera group index ==================---

TTAPI   void     TT_SetGroupShutterDelay(int groupIndex, int microseconds); //== Set camera group's shutter delay --
TTAPI   void     TT_SetCameraGroup(int cameraIndex, int cameraGroupIndex);  //== Move camera to camera group ====---

//== CAMERA GROUP FILTER SETTINGS ===============================================================================---

class TTAPI cCameraGroupFilterSettings
{
public:
    cCameraGroupFilterSettings();
    ~cCameraGroupFilterSettings();

    enum eFilterType
    {
        FilterNone,
        FilterSizeRoundness,
        FilterCount
    };

    eFilterType  FilterType;
    int          MinMarkerSize;
    int          MaxMarkerSize;
    float        MinRoundness;
};

TTAPI   NPRESULT TT_CameraGroupFilterSettings   (int groupIndex, cCameraGroupFilterSettings &settings);
TTAPI   NPRESULT TT_SetCameraGroupFilterSettings(int groupIndex, cCameraGroupFilterSettings &settings);

//== Point Cloud Reconstruction Settings ==---
class TTAPI cCameraGroupPointCloudSettings
{
public:
    enum Setting : unsigned long long
    {
        eResolvePointCloud = 1LL,               // bool
        eShowCameras = 1LL << 1,                // bool
        eVisibleMarkerSize = 1LL << 3,          // double
        ePCResidual = 1LL << 4,                 // double
        ePCMinSize = 1LL << 5,                  // double
        ePCMaxSize = 1LL << 6,                  // double
        ePCMinAngle = 1LL << 7,                 // double
        ePCMinRays = 1LL << 8,                  // long
        eShutterDelay = 1LL << 9,               // long
        ePrecisionPacketCap = 1LL << 10,        // long
        ePCMinRayLength = 1LL << 11,            // double
        ePCMaxRayLength = 1LL << 12,            // double
        ePCReconstructMinX = 1LL << 13,         // double
        ePCReconstructMaxX = 1LL << 14,         // double
        ePCReconstructMinY = 1LL << 15,         // double
        ePCReconstructMaxY = 1LL << 16,         // double
        ePCReconstructMinZ = 1LL << 17,         // double
        ePCReconstructMaxZ = 1LL << 18,         // double
        ePCObjectFilterLevel = 1LL << 19,       // long
        ePCObjectFilterMinSize = 1LL << 20,     // long
        ePCObjectFilterMaxSize = 1LL << 21,     // long
        ePCObjectFilterCircularity = 1LL << 22, // double
        ePCObjectFilterGrayscaleFloor = 1LL << 23, // long
        ePCObjectFilterAspectTolerance = 1LL << 24, // long
        ePCObjectFilterObjectMargin = 1LL << 25, // long
        eShowReconstructionBounds = 1LL << 26,  // bool
        eBoundReconstruction = 1LL << 27,       // bool
        eShowCaptureVolume = 1LL << 28,         // bool
        eShow3DMarkers = 1LL << 29,             // bool
        eShowCameraFOV = 1LL << 30,             // bool
        eCameraOverlap = 1LL << 31,             // double
        eVolumeResolution = 1LL << 32,          // double
        eWireframe = 1LL << 33,                 // double
        eFOVIntensity = 1LL << 34,              // double
        eRankRays = 1LL << 35,                  // bool
        eMinimumRankRayCount = 1LL << 36,       // long
        ePCPixelGutter = 1LL << 37,             // long
        ePCMaximum2DPoints = 1LL << 38,         // long
        ePCCalculationTime = 1LL << 39,         // long
        ePCThreadCount = 1LL << 40,             // long
        ePCCalculateDiameter = 1LL << 41,       // bool
        ePCBoost = 1LL << 42,                   // bool
        ePCSmallMarkerOptimization = 1LL << 43, // long
        eBlockWidth = 1LL << 44,                // double
        eBlockHeight = 1LL << 45,               // double
        ePointCloudEngine = 1LL << 46,          // long 1=v1.0  2=v2.0
        eSynchronizerEngine = 1LL << 47,        // long 1=v1.0  2=v2.0
        eMarkerDiameterType = 1LL << 48,        // long
        eMarkerDiameterForceSize = 1LL << 49,   // double
        eSynchronizerControl = 1LL << 50,       // long
        eSettingsCount
    };

    cCameraGroupPointCloudSettings();
    ~cCameraGroupPointCloudSettings();

    //== Set individual parameter values. Only values that are set will be changed when submitting
    //== the settings block to TT_SetCameraGroupPointCloudSettings. These methods will return false 
    //== if there is a mismatch between the requested parameter and its expected type.
    bool            SetBoolParameter( Setting which, bool val );
    bool            SetDoubleParameter( Setting which, double val );
    bool            SetLongParameter( Setting which, long val );

    //== Retrieve individual parameter settings from the parameter block. These methods will return false 
    //== if there is a mismatch between the requested parameter and its expected type.
    bool            BoolParameter( Setting which, bool &val ) const;
    bool            DoubleParameter( Setting which, double &val ) const;
    bool            LongParameter( Setting which, long &val ) const;

private:
    unsigned long long mWhichSet;
    void            *mSettings;

    friend TTAPI NPRESULT TT_CameraGroupPointCloudSettings(int groupIndex, cCameraGroupPointCloudSettings &settings);
    friend TTAPI NPRESULT TT_SetCameraGroupPointCloudSettings(int groupIndex, cCameraGroupPointCloudSettings &settings);
};

TTAPI   NPRESULT TT_CameraGroupPointCloudSettings   (int groupIndex, cCameraGroupPointCloudSettings &settings);
TTAPI   NPRESULT TT_SetCameraGroupPointCloudSettings(int groupIndex, cCameraGroupPointCloudSettings &settings);

//== cMarker Size Settings ====----

class TTAPI cCameraGroupMarkerSizeSettings
{
public:
    cCameraGroupMarkerSizeSettings();
    ~cCameraGroupMarkerSizeSettings();

    enum eMarkerSizeType
    {
        MarkerSizeCalculated,
        MarkerSizeFixed,
        MarkerSizeCount
    };

    eMarkerSizeType  MarkerSizeType;
    float            MarkerSize;
};

TTAPI   NPRESULT TT_CameraGroupMarkerSize   (int groupIndex, cCameraGroupMarkerSizeSettings &settings);
TTAPI   NPRESULT TT_SetCameraGroupMarkerSize(int groupIndex, cCameraGroupMarkerSizeSettings &settings);

TTAPI   NPRESULT TT_SetCameraGroupReconstruction(int groupIndex, bool enable);

TTAPI   NPRESULT TT_SetEnabledFilterSwitch(bool enabled);  //== Enabled by default ==--
TTAPI   bool     TT_IsFilterSwitchEnabled();

//== POINT CLOUD INTERFACE ==========================================================-----

TTAPI   int      TT_CameraCount();                    //== Returns Camera Count =====-----
TTAPI   float    TT_CameraXLocation(int index);       //== Returns Camera's X Coord =-----
TTAPI   float    TT_CameraYLocation(int index);       //== Returns Camera's Y Coord =-----
TTAPI   float    TT_CameraZLocation(int index);       //== Returns Camera's Z Coord =-----
TTAPI   float    TT_CameraOrientationMatrix(int camera, int index); //== Orientation -----

TTAPI   const char* TT_CameraName(int index);         //== Returns Camera Name ======-----

TTAPI   int      TT_CameraMarkerCount(int cameraIndex); //== Camera's 2D cMarker Count =---

                 //== CameraMarker fetches the 2D centroid location of the marker as seen
                 //== by the camera.

TTAPI   bool     TT_CameraMarker(int cameraIndex, int markerIndex, float &x, float &y);

                 //== Fetch predistorted marker location.  This is basically where the
                 //== camera would see the marker if there was no lens distortion.
                 //== For most of our cameras/lenses, this location is only a few pixels
                 //== from the distorted (TT_CameraMarker) position.

TTAPI   bool     TT_CameraMarkerPredistorted(int cameraIndex, int markerIndex, float &x, float &y);

                 //== Set camera settings.  This function allows you to set the camera's
                 //== video mode, exposure, threshold, and illumination settings.
                 
                 //== VideoType:  
                 //==     0 = Segment Mode   
                 //==     1 = Grayscale Mode 
                 //==     2 = Object Mode    
                 //==     4 = Precision Mode
                 //==     6 = MJPEG Mode     (V100R2 only)

                 //== Exposure: Valid values are:  1-480
                 //== Threshold: Valid values are: 0-255
                 //== Intensity: Valid values are: 0-15  (This should be set to 15 for all most all
                 //==                                     situations)
                  
TTAPI   bool     TT_SetCameraSettings(int cameraIndex, int videoType, int exposure, int threshold, int intensity);

TTAPI   int      TT_CameraGrayscaleDecimation(int cameraIndex); //== Camera's Full Frame Grayscale Decimation =---
TTAPI   bool     TT_SetCameraGrayscaleDecimation(int cameraIndex, int value);

                 //== Toggle camera extended options

TTAPI   bool     TT_SetCameraFilterSwitch(int cameraIndex, bool enableIRFilter);
TTAPI   bool     TT_SetCameraAGC(int cameraIndex, bool enableAutomaticGainControl);
TTAPI   bool     TT_SetCameraAEC(int cameraIndex, bool enableAutomaticExposureControl);
TTAPI   bool     TT_SetCameraHighPower(int cameraIndex, bool enableHighPowerMode);
TTAPI   bool     TT_SetCameraMJPEGHighQuality(int cameraIndex, int mjpegQuality);

                 //== Camera Imager Gain ==--

TTAPI   int      TT_CameraImagerGain(int cameraIndex);
TTAPI   int      TT_CameraImagerGainLevels(int cameraIndex);
TTAPI   void     TT_SetCameraImagerGain(int cameraIndex, int value);

                 //== Camera Illumination ==--

TTAPI   bool     TT_IsContinuousIRAvailable(int cameraIndex);
TTAPI   void     TT_SetContinuousTT_SetCameraMJPEGHighQualityIR(int cameraIndex, bool Enable);
TTAPI   bool     TT_ContinuousIR(int cameraIndex);

                 //== Camera ID ==--

TTAPI   int      TT_CameraID(int cameraIndex);

                 //== Fetch the cameras frame buffer.  This function fills the provided
                 //== buffer with an image of what is in the camera view.  The resulting
                 //== Image depends on what video mode the camera is in.  If the camera
                 //== is in grayscale mode, for example, a grayscale image is returned
                 //== from this call.

TTAPI   bool     TT_CameraFrameBuffer(int cameraIndex, int bufferPixelWidth, int bufferPixelHeight,
                                      int bufferByteSpan, int bufferPixelBitDepth, unsigned char *buffer);

                 //== Save camera's frame buffer as a BMP file

TTAPI   bool     TT_CameraFrameBufferSaveAsBMP(int cameraIndex, const char *filename);

                 //== Backproject from 3D space to 2D space.  If you give this function a 3D
                 //== location and select a camera, it will return where the point would land
                 //== on the imager of that camera in to 2D space.  This basically locates
                 //== where in the cameras FOV a 3D point would be located.

TTAPI   void     TT_CameraBackproject(int cameraIndex, float x, float y, float z, float &cameraX, float &cameraY);

                 //== The 2D centroids the camera reports are distorted by the lens.  To remove
                 //== the distortion call CameraUndistort2DPoint.  Also if you have a 2D undistorted point
                 //== that you'd like to convert back to a distorted point call CameraDistort2DPoint.

TTAPI   void     TT_CameraUndistort2DPoint(int cameraIndex, float &x, float &y);
TTAPI   void     TT_CameraDistort2DPoint  (int cameraIndex, float &x, float &y);

                 //== TT_CameraRay will take an undistorted 2D centroid and return a camera ray in the world
                 //== coordinate system.

TTAPI   bool     TT_CameraRay(int cameraIndex, float x, float y, float &rayStartX, float &rayStartY, float &rayStartZ,
                                                                 float &rayEndX,   float &rayEndY,   float &rayEndZ);

                 //== Set camera's extrinsic (position & orientation) and intrinsic (lens distortion) parameters with
                 //== parameters compatible with the OpenCV intrinsic model.

TTAPI   bool     TT_CameraModel(int cameraIndex, float x, float y, float z, //== Camera Position ===============----
                                float *orientation,                         //== Orientation (3x3 matrix) ======----
                                float principleX, float principleY,         //== Lens center (in pixels)  ======----
                                float focalLengthX, float focalLengthY,     //== Lens focal  (in pixels)  ======----
                                float kc1, float kc2, float kc3,            //== Barrel distortion coefficients ----
                                float tangential0, float tangential1);      //== Tangential distortion =========----

//== Additional Functionality =================================================================================-----

//== This function will return the Camera SDK's camera pointer.  While the Tracking Tools API takes over the
//== data path which prohibits fetching the frames directly from the camera, it is still very usefuL to be
//== able to communicate with the camera directly for setting camera settings or attaching modules.

TTAPI   CameraLibrary::Camera * TT_GetCamera(int index);  //== Returns Camera SDK Camera

TTAPI   void TT_AttachCameraModule(int index, CameraLibrary::cCameraModule *module);
TTAPI   void TT_DetachCameraModule(int index, CameraLibrary::cCameraModule *module);

//== Rigid Body Solver Callback Hook ==========================================================================-----

//== Inherit cTrackableSolutionTest and overload the TrackableSolutionTest method to have the ability to reject
//== potential rigid body solutions during the rigid body solving process.  You must attach your cTrackableSolutionTest
//== class to a rigid body via TT_AttachTrackableSolutionTest.  Return false if the presented solution should be
//== rejected.

class TTAPI cTrackableSolutionTest
{
public:
    cTrackableSolutionTest()  {}
    virtual ~cTrackableSolutionTest() {}

    //== Trackable Solution Test ==--

    virtual bool TrackableSolutionTest(int markerCount, Core::cMarker *markers, bool *markerExists) { return true; }
};

TTAPI   void     TT_AttachTrackableSolutionTest(int index, cTrackableSolutionTest* test);
TTAPI   void     TT_DetachTrackableSolutionTest(int index, cTrackableSolutionTest* test);

//== API Callbacks =============================================================================-----

//== Inherit cTTAPIListener and override it's methods to receive callbacks from the TTAPI.  You must attach your
//== listening class to the TTAPI via TT_AttachListener.

class TTAPI cTTAPIListener
{
public:
    cTTAPIListener();
    virtual ~cTTAPIListener();

    //== TTAPIFrameAvailable callback is called when a new synchronized group of camera frames has been delivered
    //== to the TTAPI and is ready for processing.  You can use this notification to then call TT_Update() without
    //== having to poll blindly for new data.

    virtual void TTAPIFrameAvailable();

    //== InitialPointCloud is called when the initial point cloud is calculated from the connected cameras. During
    //== this callback 3D markers can be added (up to MaxMarkers) or removed by modifying the Markers list as well
    //== as the MarkerCount variable.  After this callback the marker list is passed onto the rigid body solver.

    virtual void InitialPointCloud(Core::cMarker* markers, int &markerCount, int maxMarkers);
};

TTAPI   void     TT_AttachListener(cTTAPIListener* listener);
TTAPI   void     TT_DetachListener(cTTAPIListener* listener);

//== RESULT PROCESSING ========================================================================================-----

TTAPI   const char *TT_GetResultString(NPRESULT result); //== Return Plain Text Message =======================-----

#define NPRESULT_SUCCESS                0             //== Successful Result ================================-------
#define NPRESULT_FILENOTFOUND           1             //== File Not Found ===================================-------
#define NPRESULT_LOADFAILED             2             //== Load Failed ======================================-------
#define NPRESULT_FAILED                 3             //== Failed ===========================================-------
#define NPRESULT_INVALIDFILE            8             //== Invalid File =====================================-------
#define NPRESULT_INVALIDCALFILE         9             //== Invalid Calibration File =========================-------
#define NPRESULT_UNABLETOINITIALIZE     10            //== Unable To Initialize =============================-------
#define NPRESULT_INVALIDLICENSE         11            //== Invalid License ==================================-------
#define NPRESULT_NOFRAMEAVAILABLE       14            //== No Frames Available ==============================-------

//== CAMERA VIDEO TYPE DEFINITIONS ==========================================================================-------

#define NPVIDEOTYPE_SEGMENT   0
#define NPVIDEOTYPE_GRAYSCALE 1
#define NPVIDEOTYPE_OBJECT    2
#define NPVIDEOTYPE_PRECISION 4
#define NPVIDEOTYPE_MJPEG     6

//===============================================================================================================---

#endif