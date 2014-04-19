//============================================================================================-----
//== NaturalPoint Tracking Tools API Sample: Accessing Camera, Marker, and Trackable Information
//==
//== This command-line application loads a Tracking Tools Project, lists cameras, and 3d marker
//== count.
//============================================================================================-----

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "NPTrackingTools.h"

// Local function prototypes
void CheckResult( NPRESULT result );

// Local constants
const float kRadToDeg = 0.0174532925f;

// Local class definitions
class Point4
{
public:
    Point4( float x, float y, float z, float w );

    float           operator[]( int idx ) const { return mData[idx]; }
    const float*    Data() const { return mData; }

private:
    float           mData[4];
};

class TransformMatrix
{
public:
    TransformMatrix();

    TransformMatrix( float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44 );

    void            SetTranslation( float x, float y, float z );
    void            Invert();

    TransformMatrix operator*( const TransformMatrix &rhs );
    Point4          operator*( const Point4 &v );

    static TransformMatrix RotateX( float rads );
    static TransformMatrix RotateY( float rads );
    static TransformMatrix RotateZ( float rads );

private:
    float           mData[4][4];
};

// Main application
int main( int argc, char* argv[] )
{
    printf("== NaturalPoint Tracking Tools API Marker Sample =======---\n");
    printf("== (C) NaturalPoint, Inc.\n\n");

    printf("Initializing NaturalPoint Devices\n");
    TT_Initialize();

    // Do an update to pick up any recently-arrived cameras.
    TT_Update();

    // Load a project file from the executable directory.
    printf( "Loading Project: project.ttp\n\n" );
    CheckResult( TT_LoadProject("project.ttp") );

    // List all detected cameras.
    printf( "Cameras:\n" );
    for( int i = 0; i < TT_CameraCount(); i++)
    {
        printf( "\t%s\n", TT_CameraName(i) );
    }
    printf("\n");

    // List all defined rigid bodies.
    printf("Rigid Bodies:\n");
    for( int i = 0; i < TT_TrackableCount(); i++)
    {
        printf("\t%s\n", TT_TrackableName(i));
    }
    printf("\n");

    int frameCounter = 0;

    // Poll API data until the user hits a keyboard key.
    while( !_kbhit() )
    {
        if( TT_Update() == NPRESULT_SUCCESS )
        {
            frameCounter++;

            // Update tracking information every 100 frames (for example purposes).
            if( (frameCounter%100) == 0 )
            {
                float   yaw,pitch,roll;
                float   x,y,z;
                float   qx,qy,qz,qw;
                bool    tracked;

                printf( "Frame #%d: (Markers: %d)\n", frameCounter, TT_FrameMarkerCount() );

                for( int i = 0; i < TT_TrackableCount(); i++ )
                {
                    TT_TrackableLocation( i, &x,&y,&z, &qx,&qy,&qz,&qw, &yaw,&pitch,&roll );

                    if( TT_IsTrackableTracked( i ) )
                    {
                        printf( "\t%s: Pos (%.3f, %.3f, %.3f) Orient (%.1f, %.1f, %.1f)\n", TT_TrackableName( i ),
                            x, y, z, yaw, pitch, roll );

                        TransformMatrix xRot( TransformMatrix::RotateX( -roll * kRadToDeg ) );
                        TransformMatrix yRot( TransformMatrix::RotateY( -yaw * kRadToDeg ) );
                        TransformMatrix zRot( TransformMatrix::RotateZ( -pitch * kRadToDeg ) );

                        // Compose the local-to-world rotation matrix in XZY (roll, pitch, yaw) order.
                        TransformMatrix worldTransform = xRot * zRot * yRot;

                        // Inject world-space coordinates of the origin.
                        worldTransform.SetTranslation( x, y, z );

                        // Invert the transform matrix to convert from a local-to-world to a world-to-local.
                        worldTransform.Invert();

                        float   mx, my, mz;
                        int     markerCount = TT_TrackableMarkerCount( i );
                        for( int j = 0; j < markerCount; ++j )
                        {
                            // Get the world-space coordinates of each rigid body marker.
                            TT_TrackablePointCloudMarker( i, j, tracked, mx, my, mz );

                            // Transform the rigid body point from world coordinates to local rigid body coordinates.
                            // Any world-space point can be substituted here to transform it into the local space of
                            // the rigid body.
                            Point4  worldPnt( mx, my, mz, 1.0f );
                            Point4  localPnt = worldTransform * worldPnt;

                            printf( "\t\t%d: World (%.3f, %.3f, %.3f) Local (%.3f, %.3f, %.3f)\n", j + 1, 
                                mx, my, mz, localPnt[0], localPnt[1], localPnt[2] );
                        }
                    }
                    else
                    {
                        printf( "\t%s: Not Tracked\n", TT_TrackableName( i ) );
                    }
                }
            }
        }
        Sleep(2);
    }

    printf( "Shutting down NaturalPoint Tracking Tools\n" );
    CheckResult( TT_Shutdown() );

    printf( "Complete\n" );
    while( !_kbhit() )
    {
        Sleep(20);
    }

    TT_FinalCleanup();

	return 0;
}


void CheckResult( NPRESULT result )   //== CheckResult function will display errors and ---
                                      //== exit application after a key is pressed =====---
{
    if( result!= NPRESULT_SUCCESS)
    {
        // Treat all errors as failure conditions.
        printf( "Error: %s\n\n(Press any key to continue)\n", TT_GetResultString(result) );

        Sleep(20);
        exit(1);
    }
}

//
// Point4
//

Point4::Point4( float x, float y, float z, float w )
{
    mData[0] = x;
    mData[1] = y;
    mData[2] = z;
    mData[3] = w;
}

//
// TransformMatrix
//

TransformMatrix::TransformMatrix()
{
    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            if( i == j )
            {
                mData[i][j] = 1.0f;
            }
            else
            {
                mData[i][j] = 0.0f;
            }
        }
    }
}

TransformMatrix::TransformMatrix( float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44 )
{
    mData[0][0] = m11;
    mData[0][1] = m12;
    mData[0][2] = m13;
    mData[0][3] = m14;
    mData[1][0] = m21;
    mData[1][1] = m22;
    mData[1][2] = m23;
    mData[1][3] = m24;
    mData[2][0] = m31;
    mData[2][1] = m32;
    mData[2][2] = m33;
    mData[2][3] = m34;
    mData[3][0] = m41;
    mData[3][1] = m42;
    mData[3][2] = m43;
    mData[3][3] = m44;
}

void TransformMatrix::SetTranslation( float x, float y, float z )
{
    mData[0][3] = x;
    mData[1][3] = y;
    mData[2][3] = z;
}

void TransformMatrix::Invert()
{
    // Exploit the fact that we are dealing with a rotation matrix + translation component.
    // http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform

    float   tmp;
    float   vals[3];

    // Transpose left-upper 3x3 (rotation) sub-matrix
    tmp = mData[0][1]; mData[0][1] = mData[1][0]; mData[1][0] = tmp;
    tmp = mData[0][2]; mData[0][2] = mData[2][0]; mData[2][0] = tmp;
    tmp = mData[1][2]; mData[1][2] = mData[2][1]; mData[2][1] = tmp;

    // Multiply translation component (last column) by negative inverse of upper-left 3x3.
    for( int i = 0; i < 3; ++i )
    {
        vals[i] = 0.0f;
        for( int j = 0; j < 3; ++j )
        {
            vals[i] += -mData[i][j] * mData[j][3];
        }
    }
    for( int i = 0; i < 3; ++i )
    {
        mData[i][3] = vals[i];
    }
}

TransformMatrix TransformMatrix::RotateX( float rads )
{
    return TransformMatrix( 1.0, 0.0, 0.0, 0.0,
        0.0, cos( rads ), -sin( rads ), 0.0,
        0.0, sin( rads ), cos( rads ), 0.0,
        0.0, 0.0, 0.0, 1.0 );
}

TransformMatrix TransformMatrix::RotateY( float rads )
{
    return TransformMatrix( cos( rads ), 0.0, sin( rads ), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin( rads ), 0.0, cos( rads ), 0.0,
        0.0, 0.0, 0.0, 1.0 );
}

TransformMatrix TransformMatrix::RotateZ( float rads )
{
    return TransformMatrix( cos( rads ), -sin( rads ), 0.0, 0.0,
        sin( rads ), cos( rads ), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0 );
}

TransformMatrix TransformMatrix::operator*( const TransformMatrix &rhs )
{
    TransformMatrix result;

    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            float rowCol = 0.0;
            for( int k = 0; k < 4; ++k )
            {
                rowCol += mData[i][k] * rhs.mData[k][j];
            }
            result.mData[i][j] = rowCol;
        }
    }
    return result;
}

Point4 TransformMatrix::operator*( const Point4 &v )
{
    const float *pnt = v.Data();
    float   result[4];

    for( int i = 0; i < 4; ++i )
    {
        float rowCol = 0.0;
        for( int k = 0; k < 4; ++k )
        {
            rowCol += mData[i][k] * pnt[k];
        }
        result[i] = rowCol;
    }
    return Point4( result[0], result[1], result[2], result[3] );
}
