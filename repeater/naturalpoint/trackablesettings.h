
//==========================================================================----
//== Trackable Settings
//== Copyright NaturalPoint ==--
//==========================================================================----

#ifndef __TRACKABLESETTINGS_H__
#define __TRACKABLESETTINGS_H__

//==================================================================================-----

namespace Core
{
    const int kTrackableNameMaxLen = 32;
    const int kTrackableModelNameMaxLen = 256;
    class cSerializer;
}

class cTrackableSettings
{
public:
    cTrackableSettings();

    void  Save(Core::cSerializer *Serial);
    void  Load(Core::cSerializer *Serial);

    char  mName     [Core::kTrackableNameMaxLen];
    char  mModelName[Core::kTrackableModelNameMaxLen];

    int   TrackableID;
    float ColorR;
    float ColorG;
    float ColorB;

    float MaxFrameRotation;               //== depricated
    float MaxFrameTranslation;            //== depricated

    bool  DynamicRotationConstraint;      //== depricated
    bool  DynamicTranslationConstraint;   //== depricated
    bool  StaticYawRotationConstraint;    //== depricated
    bool  StaticPitchRotationConstraint;  //== depricated
    bool  StaticRollRotationConstraint;   //== depricated

    float YawGreaterThan;                 //== depricated
    float YawLessThan;                    //== depricated
    float PitchGreaterThan;               //== depricated
    float PitchLessThan;                  //== depricated
    float RollGreaterThan;                //== depricated
    float RollLessThan;                   //== depricated

    double Smoothing;                     

    bool   DisplayUntracked;
    bool   DisplayPivot;
    bool   DisplayUntrackedMarkers;
    bool   DisplayMarkerQuality;
    bool   DisplayQuality;
    bool   DisplayTracked;
    bool   DisplayLabel;
    bool   DisplayOrientation;
    bool   DisplayModelReplace;

    float  ModelYaw;
    float  ModelPitch;
    float  ModelRoll;
    float  ModelX;
    float  ModelY;
    float  ModelZ;
    float  ModelScale;

    bool   DisplayPositionHistory;
    bool   DisplayOrientationHistory;
    int    DisplayHistoryLength;
    int    DisplayOrientationSpread;
    int    DisplayOrientationSize;

    // Solver settings
    bool    Enabled;
    float   MaxMarkerDeflection;
    int     MinimumMarkerCount;
    int     MinimumHitCount;         //== depricated
    float   Flexibility;
    bool    ShareMarkers;
    int     DynamicOverride;         //== depricated
    int     StaticOverride;          //== depricated
    bool    PreciseOrientation;      //== depricated
    bool    ForceExhaustive;         //== depricated
    double  MaxCalculationTime;
    int     AcquisitionFrames;
};


#endif