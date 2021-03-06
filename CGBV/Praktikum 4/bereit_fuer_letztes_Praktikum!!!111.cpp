// Ausgangssoftware des 3. Praktikumsversuchs 
// zur Vorlesung Echtzeit-3D-Computergrahpik
// von Prof. Dr. Alfred Nischwitz
// Programm umgesetzt mit der GLTools Library
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include <math.h>
#include <math3d.h>
#include <GL/freeglut.h>
#include <AntTweakBar.h>

#define GL_PI 3.1415

GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeline;
GLFrustum viewFrustum;
GLBatch geometryBatch;
GLuint shaders;
GLBatch boden;
GLBatch boden2;
GLBatch zylrand;

/// View space light position
float light_pos[4] = { 0.5f,0.1f,-5.0f,1.0f };
/// Lichtfarben
float light_ambient[4] = { 1.0, 1.0, 1.0, 1.0 };
float light_diffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
float light_specular[4] = { 1.0f,1.0f,1.0f,1.0f };

//Materialeigenschaften
float mat_emissive[4] = { 0.0, 0.0, 0.0, 1.0 };
float mat_ambient[4] = { 0.1, 0.1, 0.0, 1.0 };
float mat_diffuse[4] = { 0.6, 0.0, 0.0, 1.0 };
float mat_specular[4] = { 0.8, 0.8, 0.8, 1.0 };
float specular_power = 10;
// Rotationsgroessen
float rotation[] = { 0, 0, 0, 0 };
//GUI
TwBar *bar;

void InitGUI()
{
	bar = TwNewBar("TweakBar");
	TwDefine(" TweakBar size='200 400'");
	TwAddVarRW(bar, "Model Rotation", TW_TYPE_QUAT4F, &rotation, "");
	TwAddVarRW(bar, "Light Position", TW_TYPE_DIR3F, &light_pos, "group=Light axisx=-x axisy=-y axisz=-z");
	//Hier weitere GUI Variablen anlegen. Für Farbe z.B. den Typ TW_TYPE_COLOR4F benutzen
}
void CreateGeometry()
{

	auto bodenVertices = new M3DVector3f[32];
	auto bodenColors = new M3DVector4f[8];	//der rote ist boden
	auto boden2Vertices = new M3DVector3f[8];
	auto boden2Colors = new M3DVector4f[8];	//der grüne ist boden2
	auto randVertices = new M3DVector3f[8];

	m3dLoadVector3(bodenVertices[0], 0, 0, 0);

	int i = 0;
	for (float angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8))
	{
		// Berechne x und y Positionen des naechsten Vertex
		float x = .5f*sin(angle);
		float y = .5f*cos(angle);

		// Spezifiziere den naechsten Vertex des Triangle_Fans
		m3dLoadVector3(bodenVertices[i], x, y, 0);

		//printf("Schleife i: %d, Wert x: %f, Wert y: %f\n\n", i, x, y);

		i++;
	}


	//Dreieck
	geometryBatch.Begin(GL_TRIANGLES, 48);

	//geometryBatch.Normal3f(0, 0, 1);
	//geometryBatch.Vertex3f(0, 0, 0);

	geometryBatch.Vertex3f(0.0, 0.0, 0.0);
	geometryBatch.Vertex3f(bodenVertices[0][0], bodenVertices[0][1], bodenVertices[0][2]);

	for (i = 1; i < 15; i++) {
		geometryBatch.Vertex3f(bodenVertices[i][0], bodenVertices[i][1], bodenVertices[i][2]);
		geometryBatch.Vertex3f(0.0, 0.0, 0.0);
		geometryBatch.Vertex3f(bodenVertices[i][0], bodenVertices[i][1], bodenVertices[i][2]);
	}
	//geometryBatch.Vertex3f(bodenVertices[14][0], bodenVertices[14][1], bodenVertices[14][2]);


	//geometryBatch.Vertex3f(0.0, 0.0, 0.0);
	//geometryBatch.Vertex3f(bodenVertices[15][0], bodenVertices[15][1], bodenVertices[15][2]);
	geometryBatch.Vertex3f(bodenVertices[0][0], bodenVertices[0][1], bodenVertices[0][2]);

	

	geometryBatch.End();



	for (i = 0; i < 16; i++) {
		printf("DREIECK #%d\n", i);
		printf("0   0   0\n");
		printf("%f   %f   %f\n", bodenVertices[i][0], bodenVertices[i][1], bodenVertices[i][2]);
		printf("%f   %f   %f\n", bodenVertices[i+1][0], bodenVertices[i+1][1], bodenVertices[i+1][2]);
		printf("\n");
	}

	printf("DREIECK #%d\n", i);
	printf("0   0   0\n");
	printf("%f   %f   %f\n", bodenVertices[15][0], bodenVertices[15][1], bodenVertices[15][2]);
	printf("%f   %f   %f\n", bodenVertices[0][0], bodenVertices[0][1], bodenVertices[0][2]);



	/*auto bodenVertices = new M3DVector3f[vertSizeCyl];
	auto bodenColors = new M3DVector4f[vertSizeCyl];	//der rote ist boden
	auto boden2Vertices = new M3DVector3f[vertSizeCyl];
	auto boden2Colors = new M3DVector4f[vertSizeCyl];	//der grüne ist boden2
	auto randVertices = new M3DVector3f[vertSizeRand];
	//auto randVertices = new M3DVector3f[10];

	// Das Zentrum des Triangle_Fans ist im Ursprung
	m3dLoadVector3(bodenVertices[0], 0, 0, -75.0);
	m3dLoadVector4(bodenColors[0], 1, 0, 0, 1);
	int i = 1;
	for (float angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / (tesselationCylinder * 2)))
	{
		// Berechne x und y Positionen des naechsten Vertex
		float x = 50.0f*sin(angle);
		float y = 50.0f*cos(angle);

		// Spezifiziere den naechsten Vertex des Triangle_Fans
		m3dLoadVector3(bodenVertices[vertSizeCyl - i], x, y, -75.0);
		m3dLoadVector3(boden2Vertices[i], x, y, 75.0);

		m3dLoadVector3(randVertices[2 * (vertSizeCyl - i) - 2], x, y, -75.0);
		m3dLoadVector3(randVertices[2 * (vertSizeCyl - i) - 1], x, y, 75.0);

		//printf("Schleife i: %d, Wert x: %f, Wert y: %f\n\n", i, x, y);

		i++;
	}


	for (int i = 0; i < vertSizeCyl; i++) {
		m3dLoadVector4(bodenColors[i], 0.2*i, 0.2, 0.2, 1);
		m3dLoadVector4(boden2Colors[i], 0.2, 0.2*i, 0.2, 1);
	}

	m3dLoadVector3(bodenVertices[1], bodenVertices[vertSizeCyl - 1][0], bodenVertices[vertSizeCyl - 1][1], bodenVertices[vertSizeCyl - 1][2]);

	boden.~GLBatch();
	boden = GLBatch();
	boden.Begin(GL_TRIANGLE_FAN, vertSizeCyl);
	boden.CopyVertexData3f(bodenVertices);
	boden.CopyColorData4f(bodenColors);
	boden.End();


	// Das Zentrum des Triangle_Fans ist im Ursprung
	m3dLoadVector3(boden2Vertices[0], 0, 0, 75.0);
	m3dLoadVector4(boden2Colors[0], 1, 0, 0, 1);
	i = 1;

	//m3dLoadVector3(randVertices[0], 0, 0, 0);

	m3dLoadVector3(boden2Vertices[vertSizeCyl - 1], boden2Vertices[1][0], boden2Vertices[1][1], boden2Vertices[1][2]);


	m3dLoadVector3(randVertices[0], randVertices[vertSizeRand - 2][0], randVertices[vertSizeRand - 2][1], randVertices[vertSizeRand - 2][2]);
	m3dLoadVector3(randVertices[1], randVertices[vertSizeRand - 1][0], randVertices[vertSizeRand - 1][1], randVertices[vertSizeRand - 1][2]);

	for (int i = 0; i < vertSizeRand; i++) {
		//printf("Schleife i: %d, Wert x: %fd, Wert y: %fd, Wert z: %fd\n", i, randVertices[i][0], randVertices[i][1], randVertices[i][2]);
	}

	boden2.~GLBatch();
	boden2 = GLBatch();
	boden2.Begin(GL_TRIANGLE_FAN, vertSizeCyl);
	boden2.CopyVertexData3f(boden2Vertices);
	boden2.CopyColorData4f(boden2Colors);
	boden2.End();

	auto randColors = new M3DVector4f[vertSizeRand];
	for (int i = 0; i < vertSizeRand; i++) {
		m3dLoadVector4(randColors[i], 0.7, 0.5, 0.2, 1);
	}

	zylrand.~GLBatch();
	zylrand = GLBatch();
	zylrand.Begin(GL_QUAD_STRIP, vertSizeRand);
	zylrand.CopyVertexData3f(randVertices);
	zylrand.CopyColorData4f(randColors);
	zylrand.End(); */








	//Shader Programme laden. Die letzen Argumente geben die Shader-Attribute an. Hier wird Vertex und Normale gebraucht.
	shaders = gltLoadShaderPairWithAttributes("VertexShader.glsl", "FragmentShader.glsl", 2,
		GLT_ATTRIBUTE_VERTEX, "vVertex",
		GLT_ATTRIBUTE_NORMAL, "vNormal");

	gltCheckErrors(shaders);
}

// Aufruf draw scene
void RenderScene(void)
{
	// Clearbefehle für den color buffer und den depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	modelViewMatrix.LoadIdentity();
	modelViewMatrix.Translate(0, 0, -5);
	// Speichere den matrix state und führe die Rotation durch
	modelViewMatrix.PushMatrix();

	M3DMatrix44f rot;
	m3dQuatToRotationMatrix(rot, rotation);
	modelViewMatrix.MultMatrix(rot);

	//setze den Shader für das Rendern
	glUseProgram(shaders);
	// Matrizen an den Shader übergeben
	glUniformMatrix4fv(glGetUniformLocation(shaders, "mvpMatrix"), 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(glGetUniformLocation(shaders, "mvMatrix"), 1, GL_FALSE, transformPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(glGetUniformLocation(shaders, "normalMatrix"), 1, GL_FALSE, transformPipeline.GetNormalMatrix(true));
	// Lichteigenschaften übergeben
	glUniform4fv(glGetUniformLocation(shaders, "light_pos_vs"), 1, light_pos);
	glUniform4fv(glGetUniformLocation(shaders, "light_ambient"), 1, light_ambient);
	glUniform4fv(glGetUniformLocation(shaders, "light_diffuse"), 1, light_diffuse);
	glUniform4fv(glGetUniformLocation(shaders, "light_specular"), 1, light_specular);
	glUniform1f(glGetUniformLocation(shaders, "spec_power"), specular_power);
	//Materialeigenschaften übergeben
	glUniform4fv(glGetUniformLocation(shaders, "mat_emissive"), 1, mat_emissive);
	glUniform4fv(glGetUniformLocation(shaders, "mat_ambient"), 1, mat_ambient);
	glUniform4fv(glGetUniformLocation(shaders, "mat_diffuse"), 1, mat_diffuse);
	glUniform4fv(glGetUniformLocation(shaders, "mat_specular"), 1, mat_specular);
	//Zeichne Model
	geometryBatch.Draw();

	// Hole die im Stack gespeicherten Transformationsmatrizen wieder zurück
	modelViewMatrix.PopMatrix();
	// Draw tweak bars
	TwDraw();
	gltCheckErrors(shaders);
	// Vertausche Front- und Backbuffer
	glutSwapBuffers();
	glutPostRedisplay();
}

// Initialisierung des Rendering Kontextes
void SetupRC()
{
	// Schwarzer Hintergrund
	glClearColor(0.12f, 0.35f, 0.674f, 0.0f);

	// In Uhrzeigerrichtung zeigende Polygone sind die Vorderseiten.
	// Dies ist umgekehrt als bei der Default-Einstellung weil wir Triangle_Fans benützen
	glFrontFace(GL_CCW);

	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	//erzeuge die geometrie
	CreateGeometry();
	InitGUI();
}

void ShutDownRC()
{
	//Aufräumen
	glDeleteProgram(shaders);

	//GUI aufräumen
	TwTerminate();
}



void ChangeSize(int w, int h)
{
	// Verhindere eine Division durch Null
	if (h == 0)
		h = 1;
	// Setze den Viewport gemaess der Window-Groesse
	glViewport(0, 0, w, h);
	// Ruecksetzung des Projection matrix stack
	projectionMatrix.LoadIdentity();
	viewFrustum.SetPerspective(45, float(w) / float(h), 1, 100);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	// Ruecksetzung des Model view matrix stack
	modelViewMatrix.LoadIdentity();

	// Send the new window size to AntTweakBar
	TwWindowSize(w, h);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("A3 Normalenvektoren");
	glutCloseFunc(ShutDownRC);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//Veralteter Treiber etc.
		std::cerr << "Error: " << glewGetErrorString(err) << "\n";
		return 1;
	}
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); // same as MouseMotion
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventKeyboardGLUT);

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	TwInit(TW_OPENGL_CORE, NULL);
	SetupRC();
	glutMainLoop();

	return 0;
}
