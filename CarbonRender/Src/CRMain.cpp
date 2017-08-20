#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRObject.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRSkyRenderPass.h"
#include "..\Inc\CRFinalPass.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRWeatherSystem.h"

SkyRenderPass skyPass;
FinalPass finalPass;

void MainDisplay()
{
	PassOutput* output;
	output = skyPass.Draw(NULL);
	finalPass.Draw(output);

	glutSwapBuffers();
}

void ReSizeCallback(int w, int h)
{
	WindowManager::Instance()->ReSize((unsigned int)w, (unsigned int)h);
	Camera* cam = CameraManager::Instance()->GetCurrentCamera();
	if (cam != nullptr)
		cam->UpdateProjectionMatrix();
}


void Init(int argc, char** argv)
{
	FbxImportManager::Instance();
	ShaderManager::Instance();
	CameraManager::Instance();
	WindowManager::Instance();
	WeatherSystem::Instance();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	WindowManager::Instance()->CreateWindow(1600, 900, "CarbonRender");

	if (glewInit())
		cout << "GLEW init fail" << endl;

	string str = "@carbonsunsu";
	cout << str << endl;
	str = string("OpenGL Version: ") + string((GLchar *)glGetString(GL_VERSION));
	cout << str << endl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//test code
	Camera cam;
	cam.SetPerspectiveCamera(90.0f, 0.01f, 1000.0f);
	cam.SetPosition(float3(0.0f, 0.0f, 0.0f));
	cam.SetRotation(float3(0.0f, 90.0f, 0.0f));
	CameraManager::Instance()->Push(cam);

	skyPass.Init();
	finalPass.Init();
	//test code
}

void FixedUpdate(int value)
{
	WeatherSystem::Instance()->Update();

	glutPostRedisplay();

	glutTimerFunc((unsigned int)(FIXEDUPDATE_TIME * 1000.0f), FixedUpdate, 0);
}

void main(int argc, char** argv)
{
	Init(argc, argv);
	
	glutDisplayFunc(MainDisplay);
	glutIdleFunc(MainDisplay);
	glutReshapeFunc(ReSizeCallback);
	glutTimerFunc((unsigned int)(FIXEDUPDATE_TIME * 1000.0f), FixedUpdate, 0);
	glutMainLoop();
}