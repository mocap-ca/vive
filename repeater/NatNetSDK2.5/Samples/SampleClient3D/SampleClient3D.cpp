//=============================================================================
// Copyright © 2009 NaturalPoint, Inc. All Rights Reserved.
// 
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall NaturalPoint, Inc. or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//=============================================================================


// NatNetSample.cpp : Defines the entry point for the application.
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning( disable : 4996 )


#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include "resource.h"

#include <GL/gl.h>
#include <GL/glu.h>

//NatNet SDK
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include "natutils.h"
#include <winsock.h>

// constants
#define MAX_MARKER_COUNT 1000
#define MAX_RIGIDBODY_COUNT 200
#define PI 3.14159265

// globals
HINSTANCE g_hInst;
HGLRC g_hRenderContext = NULL;
NatNetClient natnetClient;
int markerCount = 0;
float markerPos[3*MAX_MARKER_COUNT];
int rigidbodyCount = 0;
float rigidbody[7*MAX_RIGIDBODY_COUNT];
int rigidbodyids[MAX_RIGIDBODY_COUNT];
bool render = true;
float g_unitConversion = 1.0f;
GLuint base;							///< display list id base for font
LOGFONT m_font;
int IPAddress[4] = {127,0,0,1};

int nLabeledMarkers;
sMarker labeledMarkers[MAX_LABELED_MARKERS];

char szTimecode[128] = "";

// functions
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK NatNetDlgProc(HWND, UINT, WPARAM, LPARAM);
void Render();
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData);			// receives data from the server
void __cdecl MessageHandler(int msgType, char* msg);		// receives NatNet error mesages
bool InitNatNet(LPSTR szIPAddress, LPSTR szServerIPAddress);
bool ParseRigidBodyDescription(sDataDescriptions* pDataDefs);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX); 
  wcex.style			= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= (WNDPROC)WndProc;
  wcex.cbClsExtra		= 0;
  wcex.cbWndExtra		= 0;
  wcex.hInstance		= hInstance;
  wcex.hIcon			= NULL;
  wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName	= (LPCSTR)IDC_NATNETSAMPLE;
  wcex.lpszClassName	= "NATNETSAMPLE";
  wcex.hIconSm		= NULL;
  RegisterClassEx(&wcex);

  if (!InitInstance (hInstance, nCmdShow)) 
    return FALSE;

  MSG msg;

  while(TRUE)
  {
    if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
      if(!GetMessage(&msg, NULL, 0, 0))
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      if(render)
      {
        HDC hDC = GetDC(msg.hwnd);
        if(hDC)
        {
          wglMakeCurrent(hDC, g_hRenderContext);
          Render();
          SwapBuffers(hDC);
          wglMakeCurrent(0, 0);
        }
        ReleaseDC(msg.hwnd, hDC);
      }
    }
  }

  return (int)msg.wParam;
}

void BuildGLFont(HDC hDC)
{

    HFONT	font;										
    HFONT	oldfont;								

    base = glGenLists(96);								// Storage For 96 Characters

    m_font.lfHeight          = -18;     
    m_font.lfWidth           = 0; 
    m_font.lfEscapement      = 0; 
    m_font.lfOrientation     = 0; 
    m_font.lfWeight          = FW_MEDIUM; //FW_NORMAL; 
    m_font.lfItalic          = FALSE; 
    m_font.lfUnderline       = FALSE; 
    m_font.lfStrikeOut       = FALSE; 
    m_font.lfCharSet         = ANSI_CHARSET; 
    m_font.lfOutPrecision    = OUT_TT_PRECIS; 
    m_font.lfClipPrecision   = CLIP_DEFAULT_PRECIS; 
    m_font.lfQuality         = ANTIALIASED_QUALITY; 
    m_font.lfPitchAndFamily  = FF_DONTCARE | DEFAULT_PITCH; 
    lstrcpy(m_font.lfFaceName, TEXT("Arial"));
    font = CreateFontIndirect( &m_font );
    oldfont = (HFONT)SelectObject(hDC, font);           

    /* wgl bitmap fonts
    - pre-rendered into display list
    - no rotation
    - scale indepenent
    */
    BOOL bSuccess = wglUseFontBitmaps(hDC, 32, 96, base);

    // we're done with the font object so release it
    SelectObject(hDC, oldfont);
    DeleteObject(font);

}

void DrawGrid()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    float halfSize = 2000.0f;      // world is in mms - set to 2 cubic meters
    float step = 100.0f;           // line every .1 meter
    float major = 200.0f;          // major every .2 meters
    float yloc = 0.0f; 

    glEnable (GL_LINE_STIPPLE);
    glLineWidth (0.25);
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_COLOR_MATERIAL);

    float r,g,b,a;
    r = g = b = a = 1.0f;

    for(float x=-halfSize; x<=halfSize; x+=step)
    {
        if( (x==0) || (x==-halfSize) || (x==halfSize) )         // edge or center line
        {
            glColor4f(.76f*r,.76f*g,.76f*b,.76f*a);         
        }
        else
        {
            float ff = fmod(x,major);                           
            if(ff==0.0f)                                        // major line
            {
                glColor4f(.55f*r,0.55f*g,0.55f*b,0.55f*a);  
            }
            else                                                // minor line
            {
                glColor4f(0.3f*r,0.3f*g,0.3f*b,0.3f*a);     
            }
        }

        glBegin(GL_LINES);				
        glVertex3f(x, 0, halfSize);	    // vertical
        glVertex3f(x, 0, -halfSize);
        glVertex3f(halfSize, 0, x);     // horizontal
        glVertex3f(-halfSize, 0, x);
        glEnd();						

    }    

    glPopAttrib();
    glPopMatrix();

}

float RadiansToDegrees(float fRadians)
{
    return (float)(fRadians * (180.0f / PI));
}

void GetEulers(float qx, float qy, float qz, float qw, float *angle1,float *angle2, float *angle3)
{
    float &heading = *angle1;
    float &attitude = *angle2;
    float &bank = *angle3;

    double test = qx*qy + qz*qw;
    if (test > 0.499)   // singularity at north pole
    { 
        heading = (float) 2.0f * atan2(qx,qw);
        attitude = (float) PI/2.0f;
        bank = 0;
    }
    else if (test < -0.499)  // singularity at south pole
    { 
        heading = (float) -2.0f * atan2(qx,qw);
        attitude = (float)  - PI/2.0f;
        bank = 0;
    }
    else
    {
        double sqx = qx*qx;
        double sqy = qy*qy;
        double sqz = qz*qz;
        heading = (float) atan2((double)2.0*qy*qw-2.0*qx*qz , (double)1 - 2.0*sqy - 2.0*sqz);
        attitude = (float)asin(2.0*test);
        bank = (float) atan2((double)2.0*qx*qw-2.0*qy*qz , (double)1.0 - 2.0*sqx - 2.0*sqz);
    }

    heading = RadiansToDegrees(heading);
    attitude = RadiansToDegrees(attitude);
    bank = RadiansToDegrees(bank);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  g_hInst = hInstance;

  HWND hWnd = CreateWindow("NATNETSAMPLE", "NatNet Sample", WS_OVERLAPPEDWINDOW,
  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

  if (!hWnd)
    return FALSE;

  // Define pixel format
  PIXELFORMATDESCRIPTOR pfd;
  int nPixelFormat;	
  memset(&pfd, NULL, sizeof(pfd));    
  pfd.nSize      = sizeof(pfd);
  pfd.nVersion   = 1;
  pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;

  // Set pixel format
  HDC hDC = GetDC(hWnd);
  nPixelFormat = ChoosePixelFormat(hDC, &pfd);
  SetPixelFormat(hDC, nPixelFormat, &pfd);

  g_hRenderContext = wglCreateContext(hDC);
  wglMakeCurrent(hDC, g_hRenderContext);

  glClearColor(0.400f, 0.400f, 0.400f, 1.0f);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  BuildGLFont(hDC);

#if 0
  static GLfloat glfMatAmbient[] = {0.000f, 0.450f, 1.000f, 1.0f};
  static GLfloat glfMatDiffuse[] = {0.000f, 0.000f, 0.580f, 1.0f};
  static GLfloat glfMatSpecular[]= {1.000f, 1.000f, 1.000f, 1.0f};
  static GLfloat glfMatEmission[]= {0.000f, 0.000f, 0.000f, 1.0f};
  static GLfloat fShininess = 128.000f;

  glMaterialfv(GL_FRONT, GL_AMBIENT,  glfMatAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,  glfMatDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glfMatSpecular);
  glMaterialfv(GL_FRONT, GL_EMISSION, glfMatEmission);
  glMaterialf(GL_FRONT, GL_SHININESS, fShininess);
#endif

  wglMakeCurrent(0, 0);
  ReleaseDC(hWnd, hDC);

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  in_addr MyAddress[10];
  int nAddresses = NATUtils::GetLocalIPAddresses((unsigned long *)&MyAddress, 10);
  IPAddress[0] = MyAddress[0].S_un.S_un_b.s_b1;
  IPAddress[1] = MyAddress[0].S_un.S_un_b.s_b2;
  IPAddress[2] = MyAddress[0].S_un.S_un_b.s_b3;
  IPAddress[3] = MyAddress[0].S_un.S_un_b.s_b4;

  return TRUE;
}

void _setupView()
{



}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int wmId, wmEvent;
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message) 
  {
  case WM_COMMAND:
    wmId    = LOWORD(wParam); 
    wmEvent = HIWORD(wParam); 
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_CONNECT:
      DialogBox(g_hInst, (LPCTSTR)IDD_NATNET, hWnd, (DLGPROC)NatNetDlgProc);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    wglMakeCurrent(hdc, g_hRenderContext);
    Render();
    SwapBuffers(hdc);
    wglMakeCurrent(0, 0);
    EndPaint(hWnd, &ps);
    break;
  case WM_SIZE:
    {
    int cx = LOWORD(lParam), cy = HIWORD(lParam);
    if(cx==0 || cy ==0 || hWnd==NULL)
      break;

    GLfloat fFovy  = 40.0f; // Field-of-view
    GLfloat fZNear = 1.0f;  // Near clipping plane
    GLfloat fZFar = 10000.0f;  // Far clipping plane

    HDC hDC = GetDC(hWnd);
    wglMakeCurrent(hDC, g_hRenderContext);

    // Calculate viewport aspect
    RECT rv;
    GetClientRect(hWnd, &rv);
    GLfloat fAspect = (GLfloat)(rv.right-rv.left) / (GLfloat)(rv.bottom-rv.top);

    // Define viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fFovy, fAspect, fZNear, fZFar);
    glViewport(rv.left, rv.top, rv.right-rv.left, rv.bottom-rv.top);
    glMatrixMode(GL_MODELVIEW);

    wglMakeCurrent(0, 0);
    ReleaseDC(hWnd, hDC);
    }
    break;
  case WM_DESTROY:
    {
    HDC hDC = GetDC(hWnd);
    wglMakeCurrent(hDC, g_hRenderContext);
  	natnetClient.Uninitialize();
    wglMakeCurrent(0, 0);
    wglDeleteContext(g_hRenderContext);
    ReleaseDC(hWnd, hDC);
    PostQuitMessage(0);
    }
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

#define X1 .5257311f 
#define Z1 .8506508f

static GLfloat vdata[12][3] = {    
    {-X1, 0.0, Z1}, {X1, 0.0, Z1}, {-X1, 0.0, -Z1}, {X1, 0.0, -Z1},    
    {0.0, Z1, X1}, {0.0, Z1, -X1}, {0.0, -Z1, X1}, {0.0, -Z1, -X1},    
    {Z1, X1, 0.0}, {-Z1, X1, 0.0}, {Z1, -X1, 0.0}, {-Z1, -X1, 0.0} 
};
static GLuint tindices[20][3] = { 
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

void normalize(GLfloat *a) {
    GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d; a[1]/=d; a[2]/=d;
}

void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r) {
    if (div<=0) {
        glNormal3fv(a); glVertex3f(a[0]*r, a[1]*r, a[2]*r);
        glNormal3fv(b); glVertex3f(b[0]*r, b[1]*r, b[2]*r);
        glNormal3fv(c); glVertex3f(c[0]*r, c[1]*r, c[2]*r);
    } else {
        GLfloat ab[3], ac[3], bc[3];
        for (int i=0;i<3;i++) {
            ab[i]=(a[i]+b[i])/2;
            ac[i]=(a[i]+c[i])/2;
            bc[i]=(b[i]+c[i])/2;
        }
        normalize(ab); normalize(ac); normalize(bc);
        drawtri(a, ab, ac, div-1, r);
        drawtri(b, bc, ab, div-1, r);
        drawtri(c, ac, bc, div-1, r);
        drawtri(ab, bc, ac, div-1, r);  
    }  
}

void drawsphere(int ndiv, float radius=1.0) {
    glBegin(GL_TRIANGLES);
    for (int i=0;i<20;i++)
        drawtri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius);
    glEnd();
}

void quatToMatrix(GLfloat *q, GLfloat *m)
{
  m[0] = 1-2*q[1]*q[1]-2*q[2]*q[2]; m[1] = 2*q[0]*q[1]-2*q[3]*q[2];   m[2] = 2*q[0]*q[2]+2*q[3]*q[1];
  m[3] = 2*q[0]*q[1]+2*q[3]*q[2];   m[4] = 1-2*q[0]*q[0]-2*q[2]*q[2]; m[5] = 2*q[1]*q[2]-2*q[3]*q[0];
  m[6] = 2*q[0]*q[2]-2*q[3]*q[1];   m[7] = 2*q[1]*q[2]+2*q[3]*q[0];   m[8] = 1-2*q[0]*q[0]-2*q[1]*q[1];
}

void vec3MatrixMult(GLfloat *v, GLfloat *m)
{
  GLfloat x = v[0]*m[0]+v[1]*m[3]+v[2]*m[6];
  GLfloat y = v[0]*m[1]+v[1]*m[4]+v[2]*m[7];
  GLfloat z = v[0]*m[2]+v[1]*m[5]+v[2]*m[8];
  v[0] = x;
  v[1] = y;
  v[2] = z;
}

void drawbox(GLfloat x, GLfloat y, GLfloat z, GLfloat qx, GLfloat qy, GLfloat qz, GLfloat qw)
{
  GLfloat s = 50;
  GLfloat m[9], q[] = {qx,qy,qz,qw};
  quatToMatrix(q, m);
  GLfloat p1[] = {s,s,s}, p2[] = {-s,s,s}, p3[] = {-s,-s,s}, p4[] = {s,-s,s};
  GLfloat p5[] = {s,s,-s}, p6[] = {-s,s,-s}, p7[] = {-s,-s,-s}, p8[] = {s,-s,-s};
  vec3MatrixMult(p1, m);
  vec3MatrixMult(p2, m);
  vec3MatrixMult(p3, m);
  vec3MatrixMult(p4, m);
  vec3MatrixMult(p5, m);
  vec3MatrixMult(p6, m);
  vec3MatrixMult(p7, m);
  vec3MatrixMult(p8, m);
  p1[0] += x; p1[1] += y; p1[2] += z;
  p2[0] += x; p2[1] += y; p2[2] += z;
  p3[0] += x; p3[1] += y; p3[2] += z;
  p4[0] += x; p4[1] += y; p4[2] += z;
  p5[0] += x; p5[1] += y; p5[2] += z;
  p6[0] += x; p6[1] += y; p6[2] += z;
  p7[0] += x; p7[1] += y; p7[2] += z;
  p8[0] += x; p8[1] += y; p8[2] += z;

  GLfloat n[3];

  glBegin(GL_POLYGON);
  n[0] = 0; n[1] = 0; n[2] = 1;
  vec3MatrixMult(n, m);
  glNormal3f(n[0], n[1], n[2]);
  glVertex3f(p1[0],p1[1],p1[2]);
  glVertex3f(p2[0],p2[1],p2[2]);
  glVertex3f(p3[0],p3[1],p3[2]);
  glVertex3f(p4[0],p4[1],p4[2]);
  glEnd();

  glBegin(GL_POLYGON);
  n[0] = 0; n[1] = 0; n[2] = -1;
  vec3MatrixMult(n, m);
  glNormal3f(n[0], n[1], n[2]);
  glVertex3f(p5[0],p5[1],p5[2]);
  glVertex3f(p8[0],p8[1],p8[2]);
  glVertex3f(p7[0],p7[1],p7[2]);
  glVertex3f(p6[0],p6[1],p6[2]);
  glEnd();

  glBegin(GL_POLYGON);
  n[0] = -1; n[1] = 0; n[2] = 0;
  vec3MatrixMult(n, m);
  glNormal3f(n[0], n[1], n[2]);
  glVertex3f(p2[0],p2[1],p2[2]);
  glVertex3f(p6[0],p6[1],p6[2]);
  glVertex3f(p7[0],p7[1],p7[2]);
  glVertex3f(p3[0],p3[1],p3[2]);
  glEnd();

  glBegin(GL_POLYGON);
  n[0] = 1; n[1] = 0; n[2] = 0;
  vec3MatrixMult(n, m);
  glNormal3f(n[0], n[1], n[2]);
  glVertex3f(p1[0],p1[1],p1[2]);
  glVertex3f(p4[0],p4[1],p4[2]);
  glVertex3f(p8[0],p8[1],p8[2]);
  glVertex3f(p5[0],p5[1],p5[2]);
  glEnd();

  glBegin(GL_POLYGON);
  n[0] = 0; n[1] = 1; n[2] = 0;
  vec3MatrixMult(n, m);
  glNormal3f(n[0], n[1], n[2]);
  glVertex3f(p6[0],p6[1],p6[2]);
  glVertex3f(p2[0],p2[1],p2[2]);
  glVertex3f(p1[0],p1[1],p1[2]);
  glVertex3f(p5[0],p5[1],p5[2]);
  glEnd();

  glBegin(GL_POLYGON);
  n[0] = 0; n[1] = -1; n[2] = 0;
  vec3MatrixMult(n, m);
  glNormal3f(n[0], n[1], n[2]);
  glVertex3f(p7[0],p7[1],p7[2]);
  glVertex3f(p8[0],p8[1],p8[2]);
  glVertex3f(p4[0],p4[1],p4[2]);
  glVertex3f(p3[0],p3[1],p3[2]);
  glEnd();
}

void glPrint(double x, double y, const char *fmt, ...)
{
    char		text[256];								
    va_list		ap;										
    if (fmt == NULL)									
        return;											

    // parse formatted string/args into text string
    va_start(ap, fmt);									
    vsprintf(text, fmt, ap);					
    va_end(ap);											

    // wgl text
    glPushMatrix();
    glTranslated(x,y,0.0f);
    glRasterPos2d(0.0,0.0);
    // draw the text
    glListBase(base - 32);								
    glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);	
    glPopMatrix();
}

void Render()
{
  GLfloat m[9];
  GLfloat v[3];
  float fRadius = 5.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
  glLoadIdentity(); // Load identity matrix

  GLfloat glfLight[] = {-4.0f, 4.0f, 4.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, glfLight);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  
  glPushMatrix();

  // draw timecode
  glPushMatrix();
  glTranslatef(2400.f, -1750.f, -5000.0f);	
  glPrint(0.0f,0.0f,szTimecode);
  glPopMatrix();


  // Position and rotate the camera
#if 1
  glTranslatef(0.0f, -1000.0f, -5000.0f);	
#else
  // ori
  float quat[4];
  quat[0] = rigidbody[3];
  quat[1] = rigidbody[4];
  quat[2] = rigidbody[5];
  quat[3] = rigidbody[6];
  float yaw, pitch, roll;
  GetEulers(quat[0], quat[1], quat[2], quat[3], &yaw, &pitch, &roll);
  glRotatef(-roll, 1.0f, 0.0f, 0.0f);
  glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
  glRotatef(-pitch, 0.0f, 0.0f, 1.0f);
  
  /*
  EulerAngles eulers = Eul_FromQuat(q, order);
  eulers.x = RadiansToDegrees(eulers.x);
  eulers.y = RadiansToDegrees(eulers.y);
  eulers.z = RadiansToDegrees(eulers.z);
  */
  // pos
  float x = rigidbody[0];
  float y = rigidbody[1];
  float z = rigidbody[2];
  x *= g_unitConversion;	// convert to mm
  y *= g_unitConversion;
  z *= g_unitConversion;
  //glTranslatef(0.0f, -100.0f, -5000.0f); // origin offset	
  glTranslatef(-x, -y, -z);	

  
  

#endif


  //draw axis
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0,0,0);
  glVertex3f(300,0,0);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0,0,0);
  glVertex3f(0,300,0);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0,0,0);
  glVertex3f(0,0,300);
  glEnd();

  // draw grid
  DrawGrid();

  int i;
#if 1
  // draw rigid bodies
  for (i=0;i<rigidbodyCount;i++)
  {
    int index = 7*i;    // a rigid body pos/ori consists of 7 values : [x,y,x,qx,qy,qz,qw]
    GLfloat x = rigidbody[index], y = rigidbody[index+1], z = rigidbody[index+2];
	x *= g_unitConversion;	// convert to mm
	y *= g_unitConversion;
	z *= g_unitConversion;
    GLfloat qx = rigidbody[index+3], qy = rigidbody[index+4], qz = rigidbody[index+5], qw = rigidbody[index+6];
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    drawbox(x,y,z,qx,qy,qz,qw);
    glColor4f(0.0f,0.0f,0.0f,1.0f);

    // Convert quaternion to eulers.  Motive coordinate conventions: X(Pitch), Y(Yaw), Z(Roll), Relative, RHS
    Quat q;
    q.x = qx; q.y = qy; q.z = qz; q.w = qw;
    int order = EulOrdXYZr;
    EulerAngles ea = Eul_FromQuat(q, order);
    ea.x = RadiansToDegrees(ea.x);
    ea.y = RadiansToDegrees(ea.y);
    ea.z = RadiansToDegrees(ea.z);
    float angle1, angle2, angle3;
    GetEulers(qx, qy, qz, qw, &angle1, &angle2, &angle3);
    glPrint(x,y,"RB %d (yaw:%3.1f, pitch:%3.1f, roll:%3.1f) (yaw:%3.1f, pitch:%3.1f, roll:%3.1f)", rigidbodyids[i], ea.x, ea.y, ea.z, angle1, angle2, angle3 );
  }
#endif

#if 1
  // draw markers
  // [optional] local coordinate support : get first rb's pos/ori (face support only- assume 1st is root)
  if(rigidbodyCount==1)
  {
      GLfloat q[4] = {rigidbody[3], rigidbody[4], rigidbody[5], rigidbody[6]};
      quatToMatrix(q, m);
  }

  for (i=0;i<markerCount;i++)
  {
    int index = 3*i;
    v[0] = markerPos[index];
    v[1] = markerPos[index+1];
    v[2] = markerPos[index+2];

    // [optional] local coordinate support : inherit (accumulate) parent's RB pos/ori ("root") if using local marker position
    if(rigidbodyCount==1)
    {
        vec3MatrixMult(v,m);
        v[0] += rigidbody[0];
        v[1] += rigidbody[1];
        v[2] += rigidbody[2];
    }
    glPushMatrix();
	glTranslatef(v[0],v[1],v[2]);
    drawsphere(1,fRadius);
    glPopMatrix();
  }
#endif


#if 1	//draw labeled markers
  for (i=0;i<nLabeledMarkers;i++)
  {
	  v[0] = labeledMarkers[i].x * g_unitConversion;
	  v[1] = labeledMarkers[i].y * g_unitConversion;
	  v[2] = labeledMarkers[i].z * g_unitConversion;
	  fRadius = labeledMarkers[i].size * g_unitConversion;
	  glPushMatrix();
	  glTranslatef(v[0],v[1],v[2]);
	  glColor4f(1.0f,0.0f,0.0f,1.0f);
	  drawsphere(1,fRadius);
	 /*
	 glPrint(v[0],v[1], "Marker: %d (%3.1f, %3.1f, %3.1f)",
			  labeledMarkers[i].ID, labeledMarkers[i].x,labeledMarkers[i].y,labeledMarkers[i].z );
			  */
	  glPopMatrix();
  }

#endif


  glPopMatrix();



  glFlush();
  render = false;

}

LRESULT CALLBACK NatNetDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    char szBuf[512];
  switch (message)
  {
  case WM_INITDIALOG:
    SetDlgItemText(hDlg, IDC_EDIT1, _itoa(IPAddress[0], szBuf,10));
    SetDlgItemText(hDlg, IDC_EDIT2, _itoa(IPAddress[1], szBuf,10));
    SetDlgItemText(hDlg, IDC_EDIT3, _itoa(IPAddress[2], szBuf,10));
    SetDlgItemText(hDlg, IDC_EDIT4, _itoa(IPAddress[3], szBuf,10));
    SetDlgItemText(hDlg, IDC_EDIT5, _itoa(IPAddress[0], szBuf,10));
    SetDlgItemText(hDlg, IDC_EDIT6, _itoa(IPAddress[1], szBuf,10));
    SetDlgItemText(hDlg, IDC_EDIT7, _itoa(IPAddress[2], szBuf,10));
    SetDlgItemText(hDlg, IDC_EDIT8, _itoa(IPAddress[3], szBuf,10));
    return TRUE;

  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDC_CONNECT:
      {
      char szMyIPAddress[30], szServerIPAddress[30];
      char ip1[5], ip2[5], ip3[5], ip4[5];
      GetDlgItemText(hDlg, IDC_EDIT1, ip1, 4);
      GetDlgItemText(hDlg, IDC_EDIT2, ip2, 4);
      GetDlgItemText(hDlg, IDC_EDIT3, ip3, 4);
      GetDlgItemText(hDlg, IDC_EDIT4, ip4, 4);
      sprintf_s(szMyIPAddress, 30, "%s.%s.%s.%s", ip1,ip2,ip3,ip4);

      GetDlgItemText(hDlg, IDC_EDIT5, ip1, 4);
      GetDlgItemText(hDlg, IDC_EDIT6, ip2, 4);
      GetDlgItemText(hDlg, IDC_EDIT7, ip3, 4);
      GetDlgItemText(hDlg, IDC_EDIT8, ip4, 4);
      sprintf_s(szServerIPAddress, 30, "%s.%s.%s.%s", ip1,ip2,ip3,ip4);

      if (InitNatNet(szMyIPAddress, szServerIPAddress) == FALSE)
      {
        natnetClient.Uninitialize();
        MessageBox(hDlg, "Failed to connect", "", MB_OK);
      }
      }
    case IDOK:
    case IDCANCEL:
      EndDialog(hDlg, LOWORD(wParam));
      return TRUE;
    }
  }
  return FALSE;
}


bool InitNatNet(LPSTR szIPAddress, LPSTR szServerIPAddress)
{
  unsigned char ver[4];
  natnetClient.NatNetVersion(ver);


  // Set callback handlers
  natnetClient.SetMessageCallback(MessageHandler);
  natnetClient.SetVerbosityLevel(Verbosity_None);
  natnetClient.SetDataCallback( DataHandler );	// this function will receive data from the server

  int retCode = natnetClient.Initialize(szIPAddress, szServerIPAddress);
  if (retCode != ErrorCode_OK)
  {
    //Unable to connect to server.
    return FALSE;
  }
  else
  {
    // print server info
    sServerDescription ServerDescription;
    memset(&ServerDescription, 0, sizeof(ServerDescription));
    natnetClient.GetServerDescription(&ServerDescription);
    if(!ServerDescription.HostPresent)
    {
      //Unable to connect to server. Host not present
      return FALSE;
    }
  }

  // Retrieve RigidBody description from server
  sDataDescriptions* pDataDefs = NULL;
  int nBodies = natnetClient.GetDataDescriptions(&pDataDefs);
  if (ParseRigidBodyDescription(pDataDefs) == false)
  {
    //Unable to retrieve RigidBody description
    //return FALSE;
  }

  // example of NatNet general message passing
  void* response;
  int nBytes;
  retCode = natnetClient.SendMessageAndWait("UnitsToMillimeters", &response, &nBytes);
  if (retCode == ErrorCode_OK)
  {
    g_unitConversion = *(float*)response;
  }

  return TRUE;
}

bool ParseRigidBodyDescription(sDataDescriptions* pDataDefs)
{
  if (pDataDefs == NULL || pDataDefs->nDataDescriptions <= 0)
    return false;

  for (int i=0,j=0;i<pDataDefs->nDataDescriptions;i++)
  {
    if (pDataDefs->arrDataDescriptions[i].type != Descriptor_RigidBody)
      continue;
    
    sRigidBodyDescription *pRB = pDataDefs->arrDataDescriptions[i].Data.RigidBodyDescription;
    //construct skeleton hierarchy
    int ID        = pRB->ID;
    int parentID  = pRB->parentID;
    float offsetx = pRB->offsetx;
    float offsety = pRB->offsety;
    float offsetz = pRB->offsetz;
  }

  return true;
}

void MessageHandler(int msgType, char* msg)
{
  //	printf("\n[SampleClient] Message received: %s\n", msg);
}

void DataHandler(sFrameOfMocapData* data, void* pUserData)
{
  int i;
  int mcount = min(MAX_MARKER_COUNT,data->MocapData->nMarkers);
 	for(i=0;i<mcount;i++)
	{
    markerPos[3*i]   = data->MocapData->Markers[i][0]; //x pos
    markerPos[3*i+1] = data->MocapData->Markers[i][1]; //y pos
    markerPos[3*i+2] = data->MocapData->Markers[i][2]; //z pos
	}
  markerCount = mcount;

  // unidentified markers
  mcount = min(MAX_MARKER_COUNT,data->nOtherMarkers);
  for(i=0;i<mcount;i++)
  {
    markerPos[markerCount + (3*i)]   = data->OtherMarkers[i][0] * g_unitConversion; //x pos
    markerPos[markerCount + (3*i)+1] = data->OtherMarkers[i][1] * g_unitConversion; //y pos
    markerPos[markerCount + (3*i)+2] = data->OtherMarkers[i][2] * g_unitConversion; //z pos
  }
  markerCount += mcount;

  int rbcount = min(MAX_RIGIDBODY_COUNT, data->nRigidBodies);
  for (i=0;i<rbcount;i++)
  {
    rigidbodyids[i] = data->RigidBodies[i].ID;
    rigidbody[7*i]   = data->RigidBodies[i].x;  //x pos
    rigidbody[7*i+1] = data->RigidBodies[i].y;  //y pos
    rigidbody[7*i+2] = data->RigidBodies[i].z;  //z pos
    rigidbody[7*i+3] = data->RigidBodies[i].qx; //quaternion x
    rigidbody[7*i+4] = data->RigidBodies[i].qy; //quaternion y
    rigidbody[7*i+5] = data->RigidBodies[i].qz; //quaternion z
    rigidbody[7*i+6] = data->RigidBodies[i].qw; //quaternion w
  }
  rigidbodyCount = rbcount;

  int sklRigidbodyCount = 0;
  for (int s=0;s<data->nSkeletons;s++)
  {
    for (int r=0; r<data->Skeletons[s].nRigidBodies; r++)
    {
      rigidbodyids[i] = data->Skeletons[s].RigidBodyData[r].ID;
      rigidbody[7*i]   = data->Skeletons[s].RigidBodyData[r].x;  //x pos
      rigidbody[7*i+1] = data->Skeletons[s].RigidBodyData[r].y;  //y pos
      rigidbody[7*i+2] = data->Skeletons[s].RigidBodyData[r].z;  //z pos
      rigidbody[7*i+3] = data->Skeletons[s].RigidBodyData[r].qx; //quaternion x
      rigidbody[7*i+4] = data->Skeletons[s].RigidBodyData[r].qy; //quaternion y
      rigidbody[7*i+5] = data->Skeletons[s].RigidBodyData[r].qz; //quaternion z
      rigidbody[7*i+6] = data->Skeletons[s].RigidBodyData[r].qw; //quaternion w
      i++;
      sklRigidbodyCount++;
    }
  }
  rigidbodyCount += sklRigidbodyCount;

  nLabeledMarkers = data->nLabeledMarkers;
  for(int i=0;i<nLabeledMarkers;i++)
  {
	  labeledMarkers[i].ID = data->LabeledMarkers[i].ID;
	  labeledMarkers[i].x = data->LabeledMarkers[i].x;
	  labeledMarkers[i].y = data->LabeledMarkers[i].y;
	  labeledMarkers[i].z = data->LabeledMarkers[i].z;
	  labeledMarkers[i].size = data->LabeledMarkers[i].size;
  }

  // timecode
  NatNetClient* pClient = (NatNetClient*)pUserData;
  // decode to values
  int hour, minute, second, frame, subframe;
  bool bValid = pClient->DecodeTimecode(data->Timecode, data->TimecodeSubframe, &hour, &minute, &second, &frame, &subframe);
  // decode to friendly string
  pClient->TimecodeStringify(data->Timecode, data->TimecodeSubframe, szTimecode, 128);

  render = true;
}
