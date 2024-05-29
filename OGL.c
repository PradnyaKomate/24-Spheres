/******************************
 * Batch         : RTR 5.0
 * Group Name    : Mesh
 * Developer     : Pradnya Komate
 * Platform      : Windows
 *
 * Template Used : Diffuse Light
 *******************************/


 // Common Header Files
// Windows Header Files
#include<windows.h>
#include<stdio.h> // FileIO
#include<stdlib.h> // Exit()

// OpenGL Header Files
#include<gl/GL.h>
#include<gl/GLU.h>

#include"OGL.h" 

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable declarations
FILE *gpFile = NULL;
HWND ghwnd = NULL;
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullScreen = FALSE;

// Light Variable
BOOL bLight = FALSE;

GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // last wala 1 sangtoy ki ha positional light ahe

GLUquadric* quadric = NULL;

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;
GLuint keyPressed = 0;


// OpenGL Related Global Variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;


// Entry-point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function declarations
	int initialize_phk(void);
	void uninitialize_phk(void);
	void display_phk(void);
	void update_phk(void);

	// Local Variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("PHKWindow");

	int iResult = 0;
	BOOL bDone = FALSE;

	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int xScreenWidth = (ScreenWidth) / 2;
	int yScreenHeight = (ScreenHeight) / 2;

	int WindowWidth = 800;
	int WindowHeight = 600;
	int wWidth = (WindowWidth) / 2;
	int wHeight = (WindowHeight) / 2;

	int x = xScreenWidth - wWidth;
	int y = yScreenHeight - wHeight;

	

	// Code
	gpFile = fopen("Log.txt", "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFile, "Prgram Started Successfully\n");

	// WNDCLASSEX initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, 
					MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, 
					MAKEINTRESOURCE(MYICON));

	// Register WNDCLASSEX Class EX
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Twenty Four Spheres Light:Pradnya Hemreddy Komate(Mesh Group RTR 5.0)"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x,
		y,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// Initialization
	iResult = initialize_phk();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}

	// Show the window
	ShowWindow(hwnd, iCmdShow);

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// GameLoop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActive == TRUE)
			{
				// Render
				display_phk();

				// Update
				update_phk();

			}
		}
	}

	// Uninitialization
	uninitialize_phk();

	return((int)msg.wParam);
}

	// Callback Function
	LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		// Function declarations
		void ToggleFullscreen(void);
		void resize_phk(int, int);

		// Code
		switch (iMsg)
		{
		case WM_SETFOCUS:
			gbActive = TRUE;
			break;
		case WM_KILLFOCUS:
			gbActive = FALSE;
			break;
		case WM_SIZE:
			resize_phk(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_ERASEBKGND:
			return(0);
		case WM_KEYDOWN:
			switch (LOWORD(wParam))
			{
			case VK_ESCAPE:
				DestroyWindow(hwnd);
				break;
			}
			break;
		case WM_CHAR:
			switch (LOWORD(wParam))
			{
			case 'F':
			case 'f':
				if (gbFullScreen == FALSE)
				{
					ToggleFullscreen();
					gbFullScreen = TRUE;
				}
				else
				{
					ToggleFullscreen();
					gbFullScreen = FALSE;
				}
				break;
			case 'L':
			case 'l':                                 
				if (bLight == FALSE)
				{
					glEnable(GL_LIGHTING);
					bLight = TRUE;
				}
				else
				{
					glDisable(GL_LIGHTING);
					bLight = FALSE;
				}
				break;
			case 'X':
			case 'x':
				keyPressed = 1;
				angleForXRotation = 0.0f; // Reset
				break;
			case 'Y':
			case 'y':
				keyPressed = 2;
				angleForYRotation = 0.0f; // Reset
				break;
			case 'Z':
			case 'z':
				keyPressed = 3;
				angleForZRotation = 0.0f; // Reset
				break;
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		return(DefWindowProc(hwnd, iMsg, wParam, lParam));
	}

	void ToggleFullscreen(void)
	{
		// Local Variable declarations
		MONITORINFO mi = { sizeof(MONITORINFO) };

		// Code
		if (gbFullScreen == FALSE)
		{
			dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
			if (dwStyle & WS_OVERLAPPEDWINDOW)
			{
				if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
				{
					SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
				}
			}
			ShowCursor(FALSE);
		}
		else
		{
			SetWindowPlacement(ghwnd, &wpPrev);
			SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
			ShowCursor(TRUE);
		}
	}


	int initialize_phk(void)
	{
		// Function declarations
		void resize_phk(int, int);
		// Local Variable declarations
		PIXELFORMATDESCRIPTOR pfd;
		int iPixelFormatIndex = 0;

		ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

		// Code
		// Initialize the PIXELFORMATDESCRIPTOR
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cRedBits = 8;
		pfd.cGreenBits = 8;
		pfd.cBlueBits = 8;
		pfd.cAlphaBits = 8;

		// First change
		pfd.cDepthBits = 32;

		// Get the Device Context
		ghdc = GetDC(ghwnd);
		if (ghdc == NULL)
		{
			fprintf(gpFile, "GetDC() Failed\n");
			return(-1);
		}

		iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
		if (iPixelFormatIndex == 0)
		{
			fprintf(gpFile, "ChoosePixelFormat() Failed\n");
			return(-2);
		}

		if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		{
			fprintf(gpFile, "SetPixelFormat() Failed\n");
			return(-3);
		}

		// Create OpenGL Context from Device Context
		ghrc = wglCreateContext(ghdc);
		if (ghrc == NULL)
		{
			fprintf(gpFile, "wglCreateContext() Failed\n");
			return(-4);
		}

		// Make Rendering Context current
		if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		{
			fprintf(gpFile, "wglMakeCurrent() Failed\n");
			return(-5);
		}

		// Enabling depth
		glShadeModel(GL_SMOOTH); // ajun tr lighting apn kel nahi coloring kel nahi jevha coloring krel tevha te color shade smooth thev GL_FLAT navacha pn ahe. he optional ahe compulsory nahi

		// compulsory lines 3 function
		glClearDepth(1.0f); // frame buffer mdhla color buffer clear krayla apn glClearColor vaprla. depth buffer la clear karayla ha vapara depth buffer mdhlya saglya bits la 1 kr.
		// gl enable state machine 
		glEnable(GL_DEPTH_TEST); // 8 test paiki enable kr depth test by default disable aste
		glDepthFunc(GL_LEQUAL); // depth sathi kont function vapru varch test enable zali pn test kru ks kru tr Less than or equal to as compare kr ki tya pixel la pass kr je less than or equal to what tr z-coordinate clear depth chya value sobt mhnje 1.0 

		// Optional
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // jevha depth enable krto tevha opengl mdhe ek artifact yeto corner chya goshti depth enable kelyamul tya gol n dista elliptical distat tyamul apn sangto ki depth mul perspective mule zalel elliptical nicest kr 
		
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glEnable(GL_LIGHT0);


		// Initialize Quadric
		quadric = gluNewQuadric();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Set the Clear Color Of Window to Blue
		// Here OpenGL Starts
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f); // Dark Gray Color
		resize_phk(WIN_WIDTH, WIN_HEIGHT);
		return(0);
	}

	void resize_phk(int width, int height)
	{
		// Code

		if (height <= 0)
			height = 1;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (width <= height)
		{
			glOrtho(0.0f,
					15.5f,
					0.0f * ((GLfloat)height / (GLfloat)width),
					15.5f * (GLfloat)height / (GLfloat)width,
					-10.0f,
					10.0f);
		}
		else
		{
			glOrtho(0.0f * ((GLfloat)width / (GLfloat)height),
					15.5f * ((GLfloat)width / (GLfloat)height),
					0.0f,
					15.5f,
					-10.0f,
					10.0f);
		}
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		
	}

	void display_phk(void)
	{

		// Code

		// Variable Declarations

		GLfloat materialAmbient[4];
		GLfloat materialDiffuse[4];
		GLfloat materialSpecular[4];
		GLfloat materialShininess;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //gl clear he func initialize mdhe dilelya color ni clean krel. depth buffer chya saglya value 1 honar

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Animation
		if (keyPressed == 1)
		{
			glRotatef(angleForXRotation, 1.0f, 0.0f, 0.0f);
			lightPosition[2] = angleForXRotation;
		}
		if (keyPressed == 2)
		{
			glRotatef(angleForYRotation, 0.0f, 1.0f, 0.0f);
			lightPosition[0] = angleForYRotation;
		}
		if (keyPressed == 3)
		{
			glRotatef(angleForZRotation, 0.0f, 0.0f, 1.0f);
			lightPosition[1] = angleForZRotation;
		}
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		
		// First Column of Precious Stones
		// 1st Sphere on 1st Column, emerald
		materialAmbient[0] = 0.0215f;
		materialAmbient[1] = 0.1745f;
		materialAmbient[2] = 0.0215f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		materialDiffuse[0] = 0.07568f;
		materialDiffuse[1] = 0.61424f;
		materialDiffuse[2] = 0.07568f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.633f;
		materialSpecular[1] = 0.727811f;
		materialSpecular[2] = 0.633f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.6f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);


		glLoadIdentity();
		glTranslatef(1.5f, 14.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 2nd Sphere on 1st Column, Jade
		// Ambient Material
		materialAmbient[0] = 0.135f;
		materialAmbient[1] = 0.2225f;
		materialAmbient[2] = 0.1575f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.54f;
		materialDiffuse[1] = 0.89f;
		materialDiffuse[2] = 0.63f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.316228f;
		materialSpecular[1] = 0.316228f;
		materialSpecular[2] = 0.316228f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.1f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(1.5f, 11.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 3rd Sphere on 1st Column, obsidian
		// Ambient Material
		materialAmbient[0] = 0.05375f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.06625f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.18275f;
		materialDiffuse[1] = 0.17f;
		materialDiffuse[2] = 0.22525f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.332741f;
		materialSpecular[1] = 0.328634f;
		materialSpecular[2] = 0.346435f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.3f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(1.5f, 9.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 4th Sphere on 1st Column, Pearl
		// Ambient Material
		materialAmbient[0] = 0.25f;
		materialAmbient[1] = 0.20725f;
		materialAmbient[2] = 0.20725f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 1.0f;
		materialDiffuse[1] = 0.829f;
		materialDiffuse[2] = 0.829f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.296648f;
		materialSpecular[1] = 0.296648f;
		materialSpecular[2] = 0.296648f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.088f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(1.5f, 6.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 5th Sphere on 1st Column, Ruby
		// Ambient Material
		materialAmbient[0] = 0.1745f;
		materialAmbient[1] = 0.01175f;
		materialAmbient[2] = 0.01175f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.61424f;
		materialDiffuse[1] = 0.04136f;
		materialDiffuse[2] = 0.04136f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.727811f;
		materialSpecular[1] = 0.626959f;
		materialSpecular[2] = 0.626959f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.6f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(1.5f, 4.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 6th Sphere on 1st Column, Turquoise
		// Ambient Material
		materialAmbient[0] = 0.1f;
		materialAmbient[1] = 0.18725f;
		materialAmbient[2] = 0.1745f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.396f;
		materialDiffuse[1] = 0.74151f;
		materialDiffuse[2] = 0.69102f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.297254f;
		materialSpecular[1] = 0.30829f;
		materialSpecular[2] = 0.306678f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.1f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(1.5f, 1.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// *******************************************************
		// 1st Sphere on 2nd Column, Brass
		// Ambient Material
		materialAmbient[0] = 0.329412f;
		materialAmbient[1] = 0.223529f;
		materialAmbient[2] = 0.027451f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.780392f;
		materialDiffuse[1] = 0.568627f;
		materialDiffuse[2] = 0.113725f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.992157f;
		materialSpecular[1] = 0.941176f;
		materialSpecular[2] = 0.807843f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.21794872f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(7.5f, 14.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 2nd Sphere on 2nd Column, Bronze
		// Ambient Material
		materialAmbient[0] = 0.2125f;
		materialAmbient[1] = 0.1275f;
		materialAmbient[2] = 0.054f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.714f;
		materialDiffuse[1] = 0.4284f;
		materialDiffuse[2] = 0.18144f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.393548f;
		materialSpecular[1] = 0.271906f;
		materialSpecular[2] = 0.166721f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.2f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(7.5f, 11.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 3rd Sphere on 2nd Column, Bronze
		// Ambient Material
		materialAmbient[0] = 0.25f;
		materialAmbient[1] = 0.25f;
		materialAmbient[2] = 0.25f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.4f;
		materialDiffuse[1] = 0.4f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.774597f;
		materialSpecular[1] = 0.774597f;
		materialSpecular[2] = 0.774597f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.6f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(7.5f, 9.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 4th Sphere on 2nd Column, Copper
		// Ambient Material
		materialAmbient[0] = 0.19125f;
		materialAmbient[1] = 0.0735f;
		materialAmbient[2] = 0.0225f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.7038f;
		materialDiffuse[1] = 0.27048f;
		materialDiffuse[2] = 0.0828f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.256777f;
		materialSpecular[1] = 0.137622f;
		materialSpecular[2] = 0.086014f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.1f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(7.5f, 6.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 5th Sphere on 2nd Column, Gold
		// Ambient Material
		materialAmbient[0] = 0.24725f;
		materialAmbient[1] = 0.1995f;
		materialAmbient[2] = 0.0745f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.75164f;
		materialDiffuse[1] = 0.60648f;
		materialDiffuse[2] = 0.22648f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.628281f;
		materialSpecular[1] = 0.555802f;
		materialSpecular[2] = 0.366065f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.4f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(7.5f, 4.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 6th Sphere on 2nd Column, Silver
		// Ambient Material
		materialAmbient[0] = 0.19225f;
		materialAmbient[1] = 0.19225f;
		materialAmbient[2] = 0.19225f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.50754f;
		materialDiffuse[1] = 0.50754f;
		materialDiffuse[2] = 0.50754f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.508273f;
		materialSpecular[1] = 0.508273f;
		materialSpecular[2] = 0.508273f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.4f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(7.5f, 1.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// *******************************************
		// 1st Sphere on 3rd Column, Black
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.01f;
		materialDiffuse[1] = 0.01f;
		materialDiffuse[2] = 0.01f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.50f;
		materialSpecular[1] = 0.50f;
		materialSpecular[2] = 0.50f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.25f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(13.5f, 14.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 2nd Sphere on 3rd Column, Cyan
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.1f;
		materialAmbient[2] = 0.06f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.0f;
		materialDiffuse[1] = 0.50980392f;
		materialDiffuse[2] = 0.50980392f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.50196078f;
		materialSpecular[1] = 0.50196078f;
		materialSpecular[2] = 0.50196078f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.25f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(13.5f, 11.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 3rd Sphere on 3rd Column, Green
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.1f;
		materialDiffuse[1] = 0.35f;
		materialDiffuse[2] = 0.1f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.45f;
		materialSpecular[1] = 0.55f;
		materialSpecular[2] = 0.45f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.25f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(13.5f, 9.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 4th Sphere on 3rd Column, Red
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.0f;
		materialDiffuse[2] = 0.0f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.6f;
		materialSpecular[2] = 0.6f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.25f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(13.5f, 6.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 5th Sphere on 3rd Column, Black
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.55f;
		materialDiffuse[1] = 0.55f;
		materialDiffuse[2] = 0.55f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.70f;
		materialSpecular[1] = 0.70f;
		materialSpecular[2] = 0.70f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.25f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(13.5f, 4.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 6th Sphere on 3rd Column, Yellow Plastic
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.0f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.60f;
		materialSpecular[1] = 0.60f;
		materialSpecular[2] = 0.50f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.25f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(13.5f, 1.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// ************************************************
		// 1st Sphere on 4th Column, Black
		// Ambient Material
		materialAmbient[0] = 0.02f;
		materialAmbient[1] = 0.02f;
		materialAmbient[2] = 0.02f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.01f;
		materialDiffuse[1] = 0.01f;
		materialDiffuse[2] = 0.01f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.4f;
		materialSpecular[1] = 0.4f;
		materialSpecular[2] = 0.4f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.078125f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(19.5f, 14.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 2nd Sphere on 4th Column, Cyan
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.05f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.4f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.5f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.04f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.7f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.078125f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(19.5f, 11.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 3rd Sphere on 4th Column, Black
		// Ambient Material
		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.4f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.04f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.04f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.078125f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(19.5f, 9.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 4th Sphere on 4th Column, Red
		// Ambient Material
		materialAmbient[0] = 0.05f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.4f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.04f;
		materialSpecular[2] = 0.04f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.078125f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(19.5f, 6.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 5th Sphere on 4th Column, White
		// Ambient Material
		materialAmbient[0] = 0.05f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.05f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.5f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.7f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.078125f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(19.5f, 4.0f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		// 6th Sphere on 4th Column, Yellow Rubber
		// Ambient Material
		materialAmbient[0] = 0.05f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

		// Diffuse Material
		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		// Material Specular
		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.04f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		materialShininess = 0.078125f * 128.0f;
		glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

		glLoadIdentity();
		glTranslatef(19.5f, 1.5f, 0.0f);
		gluSphere(quadric, 1.0f, 30, 30);

		SwapBuffers(ghdc);

	}

	void update_phk(void)
	{
		// Code
		if (keyPressed == 1)
		{
			angleForXRotation = angleForXRotation + 1.0f;
			if (angleForXRotation >= 360.0f)
			{
				angleForXRotation = angleForXRotation - 360.0f;
			}
		}
		if (keyPressed == 2)
		{
			angleForYRotation = angleForYRotation + 1.0f;
			if (angleForYRotation >= 360.0f)
			{
				angleForYRotation = angleForYRotation - 360.0f;

			}
		}
		if (keyPressed == 3)
		{
			angleForZRotation = angleForZRotation + 1.0f;
			if (angleForZRotation >= 360.0f)
			{
				angleForZRotation = angleForZRotation - 360.0f;
			}
		}
	}

	void uninitialize_phk(void)
	{
		// Function Declaration
		void ToggleFullscreen(void);

		// Code
		// If application is exiting full screen then this will be followed
		if (gbFullScreen == TRUE)
		{
			ToggleFullscreen();
			gbFullScreen = FALSE;
		}

		if (gbFullScreen == FALSE)
		{
			// Make the hdc as current DC
			if (wglGetCurrentContext() == ghrc)
			{
				wglMakeCurrent(NULL, NULL);
			}

			// Destroy/ Delete Rendering Context
			if (ghrc)
			{
				wglDeleteContext(ghrc);
				ghrc = NULL;
			}

			// Release hdc
			if (ghdc)
			{
				ReleaseDC(ghwnd, ghdc);
				ghdc = NULL;
			}

			// Destroy Window
			if (ghwnd)
			{
				DestroyWindow(ghwnd);
				ghwnd = NULL;
			}

			if (quadric)
			{
				gluDeleteQuadric(quadric);
				quadric = NULL;
			}

			// Close the Log File
			if (gpFile)
			{
				fprintf(gpFile, "Program Ended Successfully\n");
				fclose(gpFile);
				gpFile = NULL;
			}

			
		}
	}
