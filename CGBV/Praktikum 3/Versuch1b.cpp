// Versuch1b.cpp
// Ausgangssoftware des 1. Praktikumsversuchs 
// zur Vorlesung Echtzeit-3D-Computergrahpik
// von Prof. Dr. Alfred Nischwitz
// Programm umgesetzt mit der GLTools Library und Vertex-Arrays
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



#include <GL/glut.h>



void   Keyboard(unsigned char key, int x, int y);
void   Motion(int x, int y);
void   Mouse(int button, int state, int x, int y);
void   Special(int key, int x, int y);
void ChangeSize(int w, int h);




GLShaderManager shaderManager;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeline;
GLFrustum viewFrustum;

GLBatch quad1;
GLBatch quad2;
GLBatch quad3;
GLBatch quad4;
GLBatch quad5;
GLBatch quad6;
GLBatch boden;
GLBatch boden2;
GLBatch zylrand;
GLBatch circleSlice;
GLBatch circleSlice2;
GLBatch sphere;
GLBatch apfel;

static float dummy = 0.0f; // delete

//Vektor f�r Augenpunkt
M3DVector3f augenpunkt[1];
bool rotateNick = false;
bool rotateGier = false;
M3DVector4f rotateY[1];		//Nick
M3DVector4f rotateX[1];		//Gier
bool up = false;
bool down = false;
bool right = false;
bool left = false;
int start = 0;
int stop = 0;

M3DVector3f scaletrain[1] = { 1, 1, 1 };

int gH = 0;
int gW = 0;

// Definition der Kreiszahl 
#define GL_PI 3.1415f

// Rotationsgroessen
static float rotation[] = { 0, 0, 0, 0 };

// Flags fuer Schalter
bool bCull = false;
bool bOutline = false;
bool bDepth = true;
bool bPersp = false;

//TESSELATION
unsigned int tesselation = 1;
float scsize = 1;

void CreateGeometry();
void timer(int);
void RenderScene(void);

//Set Funktion fuer GUI, wird aufgerufen wenn Variable im GUI geaendert wird
void TW_CALL SetTesselation(const void *value, void *clientData)
{
	//printf("SETTESS CALLED\n");
	//Pointer auf gesetzten Typ casten (der Typ der bei TwAddVarCB angegeben wurde)
	const unsigned int* uintptr = static_cast<const unsigned int*>(value);

	//Setzen der Variable auf neuen Wert
	if (*uintptr > 0 && *uintptr < 33) {
		tesselation = *uintptr;
	}

	//Hier kann nun der Aufruf gemacht werden um die Geometrie mit neuem Tesselationsfaktor zu erzeugen
	//RenderScene();
	CreateGeometry();
	//RenderScene();

}

//Get Funktion fuer GUI, damit GUI Variablen Wert zum anzeigen erhaelt
void TW_CALL GetTesselation(void *value, void *clientData)
{
	//printf("GETTESS CALLED\n");
	//Pointer auf gesetzten Typ casten (der Typ der bei TwAddVarCB angegeben wurde)
	unsigned int* uintptr = static_cast<unsigned int*>(value);

	//	//Variablen Wert and GUI weiterreichen
	*uintptr = tesselation;
}



//GUI
TwBar *bar;
void InitGUI()
{
	bar = TwNewBar("TweakBar");
	TwDefine(" TweakBar size='200 400'");
	TwAddVarRW(bar, "Model Rotation", TW_TYPE_QUAT4F, &rotation, "");
	TwAddVarRW(bar, "Depth Test?", TW_TYPE_BOOLCPP, &bDepth, "");
	TwAddVarRW(bar, "Culling?", TW_TYPE_BOOLCPP, &bCull, "");
	TwAddVarRW(bar, "Backface Wireframe?", TW_TYPE_BOOLCPP, &bOutline, "");
	//Hier weitere GUI Variablen anlegen. Fuer Farbe z.B. den Typ TW_TYPE_COLOR4F benutzen
	//Tesselation Faktor als unsigned 32 bit integer definiert
	TwAddVarRW(bar, "Size", TW_TYPE_FLOAT, &scsize, "");
	TwAddVarCB(bar, "Tesselation", TW_TYPE_UINT32, SetTesselation, GetTesselation, NULL, "");

	TwAddVarRW(bar, "Perspective", TW_TYPE_BOOLCPP, &bPersp, "");

}

void CreateGeometry()
{

	/*glColor3f(1.0f, 0.0f, 0.0f); // sets color to black.
	glBegin(GL_TRIANGLE_STRIP); // draw in triangle strips
	glVertex2f(0.0f, 0.75f); // top of the roof
	glVertex2f(-0.5f, 0.25f); // left corner of the roof
	glVertex2f(0.5f, 0.25f); // right corner of the roof
	glVertex2f(-0.5f, -0.5f); // bottom left corner of the house
	glVertex2f(0.5f, -0.5f); //bottom right corner of the house
	glEnd();*/

	//printf("createGeo called with TESS=%d\n", tesselation);


	M3DVector3f quadVertices[4];
	M3DVector4f quadColors[4];

	// red
	m3dLoadVector4(quadColors[3], 0.635, 0.235, 0.235, 1);
	m3dLoadVector4(quadColors[2], 0.635, 0.235, 0.235, 1);
	m3dLoadVector4(quadColors[1], 0.635, 0.235, 0.235, 1);
	m3dLoadVector4(quadColors[0], 0.635, 0.235, 0.235, 1);

	m3dLoadVector3(quadVertices[0], 0, 100, 0);
	m3dLoadVector3(quadVertices[1], 100, 100, 0);
	m3dLoadVector3(quadVertices[2], 0, 0, 0);
	m3dLoadVector3(quadVertices[3], 100, 0, 0);

	quad1.Begin(GL_TRIANGLE_STRIP, 4);
	quad1.CopyVertexData3f(quadVertices);
	quad1.CopyColorData4f(quadColors);
	quad1.End();

	// blue
	m3dLoadVector4(quadColors[0], 0.235, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[1], 0.235, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[2], 0.235, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[3], 0.235, 0.235, 0.635, 1);

	m3dLoadVector3(quadVertices[0], 0, 0, 0);
	m3dLoadVector3(quadVertices[1], 0, 100, 0);
	m3dLoadVector3(quadVertices[2], 0, 0, 100);
	m3dLoadVector3(quadVertices[3], 0, 100, 100);

	quad2.Begin(GL_TRIANGLE_STRIP, 4);
	quad2.CopyVertexData3f(quadVertices);
	quad2.CopyColorData4f(quadColors);
	quad2.End();

	// cyan
	m3dLoadVector4(quadColors[3], 0.235, 0.635, 0.635, 1);
	m3dLoadVector4(quadColors[2], 0.235, 0.635, 0.635, 1);
	m3dLoadVector4(quadColors[1], 0.235, 0.635, 0.635, 1);
	m3dLoadVector4(quadColors[0], 0.235, 0.635, 0.635, 1);

	m3dLoadVector3(quadVertices[0], 100, 0, 0);
	m3dLoadVector3(quadVertices[1], 100, 100, 0);
	m3dLoadVector3(quadVertices[2], 100, 0, 100);
	m3dLoadVector3(quadVertices[3], 100, 100, 100);

	quad4.Begin(GL_TRIANGLE_STRIP, 4);
	quad4.CopyVertexData3f(quadVertices);
	quad4.CopyColorData4f(quadColors);
	quad4.End();

	// green
	m3dLoadVector4(quadColors[0], 0.235, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[1], 0.235, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[2], 0.235, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[3], 0.235, 0.635, 0.235, 1);

	m3dLoadVector3(quadVertices[0], 0, 100, 100);
	m3dLoadVector3(quadVertices[1], 100, 100, 100);
	m3dLoadVector3(quadVertices[2], 0, 0, 100);
	m3dLoadVector3(quadVertices[3], 100, 0, 100);

	quad3.Begin(GL_TRIANGLE_STRIP, 4);
	quad3.CopyVertexData3f(quadVertices);
	quad3.CopyColorData4f(quadColors);
	quad3.End();

	// yellow
	m3dLoadVector4(quadColors[0], 0.635, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[1], 0.635, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[2], 0.635, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[3], 0.635, 0.635, 0.235, 1);

	m3dLoadVector3(quadVertices[0], 0, 100, 0);
	m3dLoadVector3(quadVertices[1], 100, 100, 0);
	m3dLoadVector3(quadVertices[2], 0, 100, 100);
	m3dLoadVector3(quadVertices[3], 100, 100, 100);

	quad5.Begin(GL_TRIANGLE_STRIP, 4);
	quad5.CopyVertexData3f(quadVertices);
	quad5.CopyColorData4f(quadColors);
	quad5.End();

	// magenta
	m3dLoadVector4(quadColors[0], 0.635, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[1], 0.635, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[2], 0.635, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[3], 0.635, 0.235, 0.635, 1);

	m3dLoadVector3(quadVertices[0], 0, 0, 0);
	m3dLoadVector3(quadVertices[1], 100, 0, 0);
	m3dLoadVector3(quadVertices[2], 0, 0, 100);
	m3dLoadVector3(quadVertices[3], 100, 0, 100);

	quad6.Begin(GL_TRIANGLE_STRIP, 4);
	quad6.CopyVertexData3f(quadVertices);
	quad6.CopyColorData4f(quadColors);
	quad6.End();

	int tesselationCylinder = tesselation * 2;
	int vertSizeCyl = 4 * tesselationCylinder + 2;
	int vertSizeRand = 2 * vertSizeCyl - 2;

	// Erzeuge einen weiteren Triangle_Fan um den Boden zu bedecken
	auto bodenVertices = new M3DVector3f[vertSizeCyl];
	auto bodenColors = new M3DVector4f[vertSizeCyl];
	auto boden2Vertices = new M3DVector3f[vertSizeCyl];
	auto boden2Colors = new M3DVector4f[vertSizeCyl];
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
		m3dLoadVector3(bodenVertices[i], x, y, -75.0);
		m3dLoadVector3(boden2Vertices[i], x, y, 75.0);

		m3dLoadVector3(randVertices[2 * i - 2], x, y, -75.0);
		m3dLoadVector3(randVertices[2 * i - 1], x, y, 75.0);

		//printf("Schleife i: %d, Wert x: %f, Wert y: %f\n\n", i, x, y);

		i++;
	}

	for (int i = 0; i < vertSizeCyl; i++) {
		m3dLoadVector4(bodenColors[i], 0.2*i, 0.2, 0.2, 1);
		m3dLoadVector4(boden2Colors[i], 0.2, 0.2*i, 0.2, 1);
	}

	m3dLoadVector3(bodenVertices[vertSizeCyl - 1], bodenVertices[1][0], bodenVertices[1][1], bodenVertices[1][2]);

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
	/*for (float angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// Berechne x und y Positionen des naechsten Vertex
		float x = 50.0f*sin(angle);
		float y = 50.0f*cos(angle);
		// Alterniere die Farbe zwischen Rot und Gruen
		m3dLoadVector4(boden2Colors[i], 0.8, 0.2, 0.2, 1);
		// Spezifiziere den naechsten Vertex des Triangle_Fans
		m3dLoadVector3(boden2Vertices[i], x, y, 75.0);
		m3dLoadVector3(randVertices[2 * i], x, y, 75.0);
		i++;
	}*/
	//m3dLoadVector3(randVertices[0], 0, 0, 0);

	m3dLoadVector3(boden2Vertices[vertSizeCyl - 1], boden2Vertices[1][0], boden2Vertices[1][1], boden2Vertices[1][2]);


	m3dLoadVector3(randVertices[vertSizeRand - 2], randVertices[0][0], randVertices[0][1], randVertices[0][2]);
	m3dLoadVector3(randVertices[vertSizeRand - 1], randVertices[1][0], randVertices[1][1], randVertices[1][2]);

	for (int i = 0; i < vertSizeCyl; i++) {
		//printf("Schleife i: %d, Wert x: %f, Wert y: %f, Wert z: %f\n", i, bodenVertices[i][0], bodenVertices[i][1], bodenVertices[i][2]);
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
	zylrand.End();


	float radius = 50.0;
	float fixedHeight = 10.0 / tesselation; //Anfang. 11 Ringe mit 10x Abstand
	float currentheight = 10.0 / tesselation;

	//			#Vertices Pole					#Vertices Ringe Mantel						#Vertices pro Ring
	int arr = 2 * 4 * (2 + 16 * tesselation) + (8 * tesselation + 2 * (tesselation - 1)) * (2 + 16 * tesselation) * 4;

	auto sphericalVertices = new M3DVector3f[arr];
	auto sphericalColors = new M3DVector4f[arr];
	float sphericalDepth = 100;

	auto apfelVertices = new M3DVector3f[arr];
	auto apfelColors = new M3DVector4f[arr];

	//Laufvariable ueber alle Elemente der Kugel
	i = 0;
	float currRadius = 0;
	float xFirst = 0;
	float yFirst = 0;
	float xSecond = 0;
	float ySecond = 0;

	//Huetchen vorne, Farben abwechselnd 
	for (int k = 1; k <= 2 + 16 * tesselation; k++) { //nur 2 Startpunkte und 16 werden bei Tessilierung hochskaliert
		currRadius = sqrt((pow(radius, 2) - (pow((radius - currentheight), 2)))); //aktueller Radius 
		xFirst = currRadius * sin((GL_PI / (8.0f * tesselation)) * k);
		yFirst = currRadius * cos((GL_PI / (8.0f * tesselation)) * k);
		xSecond = currRadius * sin((GL_PI / (8.0f * tesselation)) * (k + 1));
		ySecond = currRadius * cos((GL_PI / (8.0f * tesselation)) * (k + 1));

		m3dLoadVector3(sphericalVertices[i], 0, 0, sphericalDepth); //Startpunkt
		m3dLoadVector3(sphericalVertices[i + 1], xFirst, yFirst, (sphericalDepth - currentheight));
		m3dLoadVector3(sphericalVertices[i + 2], 0, 0, sphericalDepth);
		m3dLoadVector3(sphericalVertices[i + 3], xSecond, ySecond, (sphericalDepth - currentheight));

		m3dLoadVector3(apfelVertices[i], 0, 0, sphericalDepth);
		m3dLoadVector3(apfelVertices[i + 1], xFirst, yFirst, (sphericalDepth - currentheight));
		m3dLoadVector3(apfelVertices[i + 2], 0, 0, sphericalDepth);
		m3dLoadVector3(apfelVertices[i + 3], xSecond, ySecond, (sphericalDepth - currentheight));

		m3dLoadVector4(apfelColors[i], 0.635, 0.235, 0.235, 1);
		m3dLoadVector4(apfelColors[i + 1], 0.635, 0.235, 0.235, 1);
		m3dLoadVector4(apfelColors[i + 2], 0.635, 0.235, 0.235, 1);
		m3dLoadVector4(apfelColors[i + 3], 0.635, 0.235, 0.235, 1);

		//Farben setzen
		if ((k % 2) == 0) {
			m3dLoadVector4(sphericalColors[i], 0.396, 0.810, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 1], 0.396, 0.810, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 2], 0.396, 0.810, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 3], 0.396, 0.810, 0.810, 1);
		}
		else {
			m3dLoadVector4(sphericalColors[i], 0.635, 0.235, 0.635, 1);
			m3dLoadVector4(sphericalColors[i + 1], 0.635, 0.235, 0.635, 1);
			m3dLoadVector4(sphericalColors[i + 2], 0.635, 0.235, 0.635, 1);
			m3dLoadVector4(sphericalColors[i + 3], 0.635, 0.235, 0.635, 1);
		}

		i += 4;
	}
	int p = 0;

	//Mantel einfarbig
	for (int l = 1; l <= 8 * tesselation + 2 * (tesselation - 1); l++) { //Ring fuer Ring durchgehen
		for (int k = 1; k <= 2 + 16 * tesselation; k++) { //innerhalb eines Rings
			currRadius = sqrt((pow(radius, 2) - (pow((radius - currentheight), 2))));
			xFirst = currRadius * sin((GL_PI / (8.0f * tesselation)) * k);
			yFirst = currRadius * cos((GL_PI / (8.0f * tesselation)) * k);
			xSecond = currRadius * sin((GL_PI / (8.0f * tesselation)) * (k + 1));
			ySecond = currRadius * cos((GL_PI / (8.0f * tesselation)) * (k + 1));

			float secRadius = sqrt((pow(radius, 2) - (pow((radius - (currentheight + (10.0 / tesselation))), 2))));
			float xNew = secRadius * sin((GL_PI / (8.0f * tesselation)) * k);
			float yNew = secRadius * cos((GL_PI / (8.0f * tesselation)) * k);
			float x1New = secRadius * sin((GL_PI / (8.0f * tesselation)) * (k + 1));
			float y1New = secRadius * cos((GL_PI / (8.0f * tesselation)) * (k + 1));

			//aktuelle Hoehe des aktuellen Rings (z-Wert)
			float secHeightFirst = (sphericalDepth - (fixedHeight * l));
			float secHeightSecond = (sphericalDepth - (fixedHeight * (l + 1)));

			m3dLoadVector3(sphericalVertices[i], xFirst, yFirst, secHeightFirst); //fixedHeight * l
			m3dLoadVector3(sphericalVertices[i + 1], xNew, yNew, secHeightSecond); //fixedHeight * (l + 1)
			m3dLoadVector3(sphericalVertices[i + 2], xSecond, ySecond, secHeightFirst);	//fixedHeight * l
			m3dLoadVector3(sphericalVertices[i + 3], x1New, y1New, secHeightSecond);	//fixedHeight * (l + 1)

			m3dLoadVector3(apfelVertices[i], xFirst, yFirst, secHeightFirst); //fixedHeight * l
			m3dLoadVector3(apfelVertices[i + 1], xNew, yNew, secHeightSecond); //fixedHeight * (l + 1)
			m3dLoadVector3(apfelVertices[i + 2], xSecond, ySecond, secHeightFirst);	//fixedHeight * l
			m3dLoadVector3(apfelVertices[i + 3], x1New, y1New, secHeightSecond);	//fixedHeight * (l + 1)

			m3dLoadVector4(apfelColors[i], 0.635, 0.235, 0.235, 1);
			m3dLoadVector4(apfelColors[i + 1], 0.635, 0.235, 0.235, 1);
			m3dLoadVector4(apfelColors[i + 2], 0.635, 0.235, 0.235, 1);
			m3dLoadVector4(apfelColors[i + 3], 0.635, 0.235, 0.235, 1);

			//Farben setzen
			m3dLoadVector4(sphericalColors[i], 0.396, 0.810, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 1], 0.396, 0.810, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 2], 0.396, 0.810, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 3], 0.396, 0.810, 0.810, 1);

			i += 4;
		}
		p++;
		currentheight += (10.0 / tesselation);
	}

	//Huetchen hinten, Farben abwechselnd
	for (int k = 1; k <= 2 + 16 * tesselation; k++) {
		currRadius = sqrt((pow(radius, 2) - (pow((radius - currentheight), 2))));
		xFirst = currRadius * sin((GL_PI / (8.0f * tesselation)) * k);
		yFirst = currRadius * cos((GL_PI / (8.0f * tesselation)) * k);
		xSecond = currRadius * sin((GL_PI / (8.0f * tesselation)) * (k + 1));
		ySecond = currRadius * cos((GL_PI / (8.0f * tesselation)) * (k + 1));

		m3dLoadVector3(sphericalVertices[i], xFirst, yFirst, fixedHeight);
		m3dLoadVector3(sphericalVertices[i + 1], 0, 0, 0);
		m3dLoadVector3(sphericalVertices[i + 2], xSecond, ySecond, fixedHeight);
		m3dLoadVector3(sphericalVertices[i + 3], 0, 0, 0);

		m3dLoadVector3(apfelVertices[i], xFirst, yFirst, fixedHeight);
		m3dLoadVector3(apfelVertices[i + 1], 0, 0, 0);
		m3dLoadVector3(apfelVertices[i + 2], xSecond, ySecond, fixedHeight);
		m3dLoadVector3(apfelVertices[i + 3], 0, 0, 0);

		m3dLoadVector4(apfelColors[i], 0.635, 0.235, 0.235, 1);
		m3dLoadVector4(apfelColors[i + 1], 0.635, 0.235, 0.235, 1);
		m3dLoadVector4(apfelColors[i + 2], 0.635, 0.235, 0.235, 1);
		m3dLoadVector4(apfelColors[i + 3], 0.635, 0.235, 0.235, 1);

		//Farben setzen 
		if ((k % 2) == 0) {
			m3dLoadVector4(sphericalColors[i], 0.396, 1, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 1], 0.396, 0.810, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 2], 0.396, 1, 0.810, 1);
			m3dLoadVector4(sphericalColors[i + 3], 0.396, 0.810, 0.810, 1);
		}
		else {
			m3dLoadVector4(sphericalColors[i], 0.635, 0.235, 0.635, 1);
			m3dLoadVector4(sphericalColors[i + 1], 0.635, 0.235, 0.635, 1);
			m3dLoadVector4(sphericalColors[i + 2], 0.635, 0.235, 0.635, 1);
			m3dLoadVector4(sphericalColors[i + 3], 0.635, 0.235, 0.635, 1);
		}
		i += 4;
	}

	apfel.~GLBatch();
	apfel = GLBatch();
	apfel.Begin(GL_QUAD_STRIP, arr);
	apfel.CopyVertexData3f(apfelVertices);
	apfel.CopyColorData4f(apfelColors);
	apfel.End();

	sphere.~GLBatch();
	sphere = GLBatch();
	sphere.Begin(GL_QUAD_STRIP, arr);
	sphere.CopyVertexData3f(sphericalVertices);
	sphere.CopyColorData4f(sphericalColors);
	sphere.End();
}

void DrawSquare() {
	quad1.Draw();
	quad2.Draw();
	quad3.Draw();
	quad4.Draw();
	quad5.Draw();
	quad6.Draw();
}

void DrawCylinder() {
	boden.Draw();
	zylrand.Draw();
	boden2.Draw();
}

void DrawSphere() {
	sphere.Draw();
}

void DrawApple() {
	apfel.Draw();
}

void DrawWheels() {
	static float rotWh = 0.0f;
	rotWh += 01.0f;

	//erstes Set Raeder
	modelViewMatrix.Scale(0.5, 0.5, 0.1);
	modelViewMatrix.Translate(40, -50, 150);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(rotWh, 0, 0, 150);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();


	modelViewMatrix.Translate(0, 0, 700);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(rotWh, 0, 0, 150);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();


	//zweites Set Raeder
	modelViewMatrix.Translate(160, 0, -700);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(rotWh, 0, 0, 150);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();


	modelViewMatrix.Translate(0, 0, 700);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(rotWh, 0, 0, 150);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();

	//drittes Set Raeder
	modelViewMatrix.Translate(160, 0, -700);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(rotWh, 0, 0, 150);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.Translate(0, 0, 700);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(rotWh, 0, 0, 150);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.Translate(0, 0, 100);

	//Rad-Antrieb
	//vorne
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Scale(1.75, 0.1, 0.01);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(-137 + 10 * cos(rotWh*0.0174), 45 + 130 * sin(rotWh*0.0174), 100);
	modelViewMatrix.Rotate(180, 180, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(0, 0, -900);
	modelViewMatrix.Scale(1.75, 0.1, 0.01);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(-137 + 10 * cos(rotWh*0.0174), 45 + 130 * sin(rotWh*0.0174), 0);
	modelViewMatrix.Rotate(180, 180, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();

	//hinten
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Scale(1.75, 0.1, 0.01);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(-43 + 10 * cos(rotWh*0.0174), 45 + 130 * sin(rotWh*0.0174), 100);
	modelViewMatrix.Rotate(180, 180, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(0, 0, -900);
	modelViewMatrix.Scale(1.75, 0.1, 0.01);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(-43 + 10 * cos(rotWh*0.0174), 45 + 130 * sin(rotWh*0.0174), 0);
	modelViewMatrix.Rotate(180, 180, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();


	modelViewMatrix.PopMatrix();
}

void DrawChimney() {
	static float translateCh = 0.0f;
	static bool directionChange = false;
	if (!directionChange) {
		translateCh += 0.00025f;
		if (translateCh > 0.1)
			directionChange = true;
	}
	else {
		translateCh -= 0.00025f;
		if (translateCh < 0.01)
			directionChange = false;
	}

	modelViewMatrix.Translate(30, 120, 50);
	modelViewMatrix.Scale(0.4 - translateCh, 0.3 + translateCh, 0.4 - translateCh);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(90, 90, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();
}

void DrawSmoke() {
	static float translateSmoke = 0.0f;
	static int rotateSmoke = 0.0f;
	rotateSmoke = (rotateSmoke + 1) % 360;
	static bool directionChange = false;
	if (!directionChange) {
		translateSmoke += 0.5f;
		if (translateSmoke > 100.0)
			directionChange = true;
	}
	else {
		translateSmoke -= 0.5f;
		if (translateSmoke < 0.0)
			directionChange = false;
	}

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(100 + translateSmoke, 200, -230);
	modelViewMatrix.Rotate(rotateSmoke, 0, rotateSmoke, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawSphere();

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Scale(1.2, 1.2, 1.2);
	modelViewMatrix.Translate(80 + translateSmoke, 50, -100);
	modelViewMatrix.Rotate(rotateSmoke, 0, rotateSmoke, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawSphere();

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Scale(1.2, 1.2, 1.2);
	modelViewMatrix.Translate(80 + translateSmoke, 80, -100);
	modelViewMatrix.Rotate(rotateSmoke, 0, rotateSmoke, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawSphere();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();



}

void DrawLocomotive() {
	DrawSquare();

	//zweiter Quader
	modelViewMatrix.Translate(100, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	modelViewMatrix.PushMatrix();
	DrawSquare();

	//Fahrerhaeuschen
	modelViewMatrix.Translate(0, 100, 0);
	modelViewMatrix.Scale(1, 0.5, 1);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawSquare();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();

	// Raeder
	DrawWheels();

	//Schornstein
	DrawChimney();

	//Rauch
	DrawSmoke();

	modelViewMatrix.PopMatrix();
}

void ScaledApple() {
	modelViewMatrix.Scale(0.1, 0.1, 0.1);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawApple();
}

void DrawTree() {

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Scale(0.3, 1, 0.3);
	modelViewMatrix.Rotate(90, 90, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawCylinder();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(0, 170, 0);
	modelViewMatrix.Scale(1.2, 1, 0.5);
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate(90, 90, 0, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	DrawSphere();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.Translate(0, 150, 20);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	modelViewMatrix.PushMatrix();
	ScaledApple();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.Translate(40, -25, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	modelViewMatrix.PushMatrix();
	ScaledApple();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.Translate(-35, -15, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	modelViewMatrix.PushMatrix();
	ScaledApple();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.Translate(-35, 30, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	modelViewMatrix.PushMatrix();
	ScaledApple();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.Translate(-10, -20, 0);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	modelViewMatrix.PushMatrix();
	ScaledApple();

	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();



	DrawSquare();
}

void DrawLandscape() {

	modelViewMatrix.PushMatrix();
	DrawTree();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.Translate(120, 15, 0);
	modelViewMatrix.Scale(1.2, 1.2, 1.2);
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	modelViewMatrix.PushMatrix();
	DrawTree();
	modelViewMatrix.PopMatrix();





	//DrawSquare();
}


// Aufruf draw scene
void RenderScene(void)
{
	glutTimerFunc(1000.0 / 60.0, timer, 0);
	modelViewMatrix.Translate(augenpunkt[0][0], augenpunkt[0][1], 0);
}


void timer(int)
{
	/* update animation */
	//glutPostRedisplay();

	// Clearbefehle fuer den color buffer und den depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Schalte culling ein falls das Flag gesetzt ist
	if (bCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	// Schalte depth testing ein falls das Flag gesetzt ist
	if (bDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	/*if (bPersp)
		glFrustum();
	else
		glPerspe();*/

		/*int w;
		int h;
		glViewport(0, 0, w, h);
		ChangeSize(w, h);*/




		// Zeichne die Rueckseite von Polygonen als Drahtgitter falls das Flag gesetzt ist
	if (bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);

	// Speichere den matrix state und fuehre die Rotation durch
	modelViewMatrix.PushMatrix();

	modelViewMatrix.LoadIdentity();
	modelViewMatrix.Scale(scsize, scsize, scsize);

	if (bPersp) {
		//modelViewMatrix.PushMatrix();
		ChangeSize(gW, gH);
		modelViewMatrix.Translate(0.0, 0.0, -500.0);
		modelViewMatrix.Scale(1.3, 1.3, 1.3);
		//modelViewMatrix.Rotate(180.0, 0.0, 1.0, 0.0);
		//modelViewMatrix.Rotate(180.0, 0.0, 0.0, 1.0);
		//modelViewMatrix.PopMatrix();
	}
	else {

		ChangeSize(gW, gH);
	}

	M3DMatrix44f rot;
	m3dQuatToRotationMatrix(rot, rotation);
	modelViewMatrix.MultMatrix(rot);
	//modelViewMatrix.Translate(0.0, 0.0, -200.0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///Maus und Tastatur abfangen
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	modelViewMatrix.Translate(augenpunkt[0][0], augenpunkt[0][1], augenpunkt[0][2]);

	modelViewMatrix.Rotate(rotateX[0][0], rotateX[0][1], rotateX[0][2], rotateX[0][3]);
	modelViewMatrix.Rotate(rotateY[0][0], rotateY[0][1], rotateY[0][2], rotateY[0][3]);

	//modelViewMatrix.Scale(scaletrain[0][0], scaletrain[0][1], scaletrain[0][2]);

	modelViewMatrix.Translate(0, 0, dummy);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/*if (left) {
		printf("rotieren um Y nach links\n");
		modelViewMatrix.Translate(0, 100, 0);
		left = false;
	}
	if (right) {
		printf("rotieren um Y nach rechts\n");
		modelViewMatrix.Rotate(100, 0, 100, 0);
		right = false;
	}*/

	//setze den Shader fuer das Rendern
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());

	modelViewMatrix.PushMatrix();
	modelViewMatrix.PushMatrix();

	DrawLocomotive();
	//DrawLandscape();


	//Auf fehler ueberpruefen
	gltCheckErrors(0);
	// Hole die im Stack gespeicherten Transformationsmatrizen wieder zurueck
	//modelViewMatrix.PopMatrix();

	TwDraw();
	// Vertausche Front- und Backbuffer
	glutSwapBuffers();
	glutPostRedisplay();

	//glutTimerFunc(1000.0 / 60.0, timer, 0);
}


// Initialisierung des Rendering Kontextes
void SetupRC()
{
	// Schwarzer Hintergrund
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// In Uhrzeigerrichtung zeigende Polygone sind die Vorderseiten.
	// Dies ist umgekehrt als bei der Default-Einstellung weil wir Triangle_Fans benuetzen
	glFrontFace(GL_CW);

	//initialisiert die standard shader
	shaderManager.InitializeStockShaders();
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	//erzeuge die geometrie
	CreateGeometry();
	InitGUI();
}

void SpecialKeys(int key, int x, int y)
{
	printf("Special-Function\n");
	printf("Key: %d", key);

	switch (key)
	{
	case GLUT_KEY_UP:
		//printf("Pfeil nach oben was pressed - SpecialKeys\n");
		//printf("Maus x: %d, Maus y: %d\n", x, y);
		m3dLoadVector3(augenpunkt[0], augenpunkt[0][0], augenpunkt[0][1] + 10, 0);
		break;
	case GLUT_KEY_RIGHT:
		//printf("Pfeil nach rechts was pressed - SpecialKeys\n");
		//printf("Maus x: %d, Maus y: %d\n", x, y);
		m3dLoadVector3(augenpunkt[0], augenpunkt[0][0] + 10, augenpunkt[0][1], 0);
		break;
	case GLUT_KEY_DOWN:
		//printf("Pfeil nach unten was pressed - SpecialKeys\n");
		//printf("Maus x: %d, Maus y: %d\n", x, y);
		m3dLoadVector3(augenpunkt[0], augenpunkt[0][0], augenpunkt[0][1] - 10, 0);
		break;
	case GLUT_KEY_LEFT:
		//printf("Pfeil nach links was pressed - SpecialKeys\n");
		//printf("Maus x: %d, Maus y: %d\n", x, y);
		m3dLoadVector3(augenpunkt[0], augenpunkt[0][0] - 10, augenpunkt[0][1], 0);
		break;
	}


	TwEventKeyboardGLUT(key, x, y);
	// Zeichne das Window neu
	glutPostRedisplay();
}


void ChangeSize(int w, int h)
{

	gH = h; gW = w;
	GLfloat nRange = 300.0f;

	// Verhindere eine Division durch Null
	if (h == 0)
		h = 1;
	// Setze den Viewport gemaess der Window-Groesse
	glViewport(0, 0, w, h);
	// Ruecksetzung des Projection matrix stack
	projectionMatrix.LoadIdentity();

	// Definiere das viewing volume (left, right, bottom, top, near, far)
	if (w <= h) {
		//viewFrustum.SetOrthographic(-nRange, nRange, -nRange * float(h) / float(w), nRange * float(h) / float(w), nRange, 5*nRange);
		if (!bPersp) {
			viewFrustum.SetOrthographic(-nRange, nRange, -nRange * float(h) / float(w), nRange * float(h) / float(w), -nRange, nRange);
		}
		else {
			viewFrustum.SetFrustum(-nRange * float(w) / float(h), nRange * float(w) / float(h), -nRange, nRange, 250, -5 * nRange);
		}
	}
	else {
		//viewFrustum.SetOrthographic(-nRange * float(w) / float(h), nRange * float(w) / float(h), -nRange, nRange, nRange, 5 * nRange);
		if (!bPersp) {
			viewFrustum.SetOrthographic(-nRange * float(w) / float(h), nRange * float(w) / float(h), -nRange, nRange, -nRange, nRange);
		}
		else {
			viewFrustum.SetFrustum(-nRange * float(w) / float(h), nRange * float(w) / float(h), -nRange, nRange, 250, -5 * nRange);
		}

	}
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	// Ruecksetzung des Model view matrix stack
	modelViewMatrix.LoadIdentity();

	TwWindowSize(w, h);
}

void ShutDownRC()
{
	//GUI aufraeumen
	TwTerminate();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Versuch1");
	glutCloseFunc(ShutDownRC);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Veralteter Treiber etc.
		std::cerr << "Error: " << glewGetErrorString(err) << "\n";
		return 1;
	}

	glutSpecialFunc(SpecialKeys);
	glutMouseFunc(Mouse);

	//glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);
	//glutReshapeFunc(Resize);
	//glutSpecialFunc(Special);

	//glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);	//Rotate
	//glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); // same as MouseMotion
	//glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	TwInit(TW_OPENGL_CORE, NULL);
	SetupRC();



	glutMainLoop();



	//glutDisplayFunc(Redraw);



	return 0;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void
Keyboard(unsigned char key, /* I - Key that was pressed */
	int           x,   /* I - Mouse X position */
	int           y)   /* I - Mouse Y position */
{

	printf("Keyboard\n");
	switch (key)
	{
	case 0x1b:
		puts("");
		exit(0);
		break;
	case GLUT_KEY_UP:
		printf("Pfeil nach oben was pressed\n");
		printf("Maus x: %d, Maus y: %d\n", x, y);
		glutSpecialFunc(SpecialKeys);
		break;
	case 'd':
		printf("Pfeil nach rechts was pressed\n");

		break;
	case 's':
		printf("Pfeil nach unten was pressed\n");

		break;
	case 'a':
		printf("Pfeil nach links was pressed\n");

		break;
	}

	glutPostRedisplay();
}


/*
 * 'Motion()' - Handle mouse pointer motion.
 */

void
Motion(int x, /* I - Current mouse X position */
	int y) /* I - Current mouse Y position */
{

	//glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);


	if (rotateGier) {
		if (x > start) { //mehr links sehen L->R ziehen
			rotateX[0][0] = rotateX[0][0] + 5;
			rotateX[0][2] = 1;
			start = x;
		}
		else {		//mehr rechts sehen R->L ziehen
			rotateX[0][0] = rotateX[0][0] - 5;
			rotateX[0][2] = 1;
			start = x;
		}

	}
	if (rotateNick) {
		if (y > start) {
			rotateY[0][0] = rotateY[0][0] + 5;
			rotateY[0][1] = 1;
			start = y;
		}
		else {
			rotateY[0][0] = rotateY[0][0] - 5;
			rotateY[0][1] = 1;
			start = y;
		}
	}
}


/*
 * 'Mouse()' - Handle mouse button events.
 */

void
Mouse(int button, /* I - Button that changed */
	int state,  /* I - Current button states */
	int x,      /* I - Current mouse X position */
	int y)      /* I - Current mouse Y position */
{
	//bool pressed = false;

	//Sidebar weiterhin verwendbar
	TwMouseButton((state == GLUT_DOWN) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED, TW_MOUSE_LEFT);

	printf("Mausbutton: %d\n", button);
	//printf("Position: %d,%d\n", x, y);

	if (state == GLUT_DOWN) {
		if (button == 0) { //Gierbewegung
			//printf("Gierbewegung\n");
			start = x;
			rotateGier = true;
		}
		else if (button == 1) { //middle mouse button

		}
		else if (button == 2) { //Nickbewegung
			//printf("Nickbewegung");
			start = y;
			rotateNick = true;
		}
		else if (button == 3) { //zoom in 
			printf("setze augenpunkt zoom in\n");
			//glutReshapeFunc(ChangeSize);
			scaletrain[0][0] = scaletrain[0][0] + 0.1;
			scaletrain[0][1] = scaletrain[0][1] + 0.1;
			scaletrain[0][2] = scaletrain[0][2] + 0.1;
			dummy += 5.0f;
			//augenpunkt[0][2] = augenpunkt[0][2] + 10;
		}
		else if (button == 4) { //zoom out
			printf("setze augenpunkt zoom out\n");
			scaletrain[0][0] = scaletrain[0][0] - 0.1;
			scaletrain[0][1] = scaletrain[0][1] - 0.1;
			scaletrain[0][2] = scaletrain[0][2] - 0.1;
			//augenpunkt[0][2] = augenpunkt[0][2] - 10;
			dummy -= 5.0f;
		}
	}
	else {
		right = false;
		left = false;
		up = false;
		down = false;
		rotateGier = false;
		rotateNick = false;
	}

}



/*
 * 'Special()' - Handle special keys like left, right, up, and down.
 */

void
Special(int key, /* I - Key that was pressed */
	int x,   /* I - X location of the mouse pointer */
	int y)   /* I - Y location of the mouse pointer */
{
	int ViewAngle = 0;
	switch (key)
	{
	case GLUT_KEY_UP:
		ViewAngle = 0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		ViewAngle = 180;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		ViewAngle = (ViewAngle + 315) % 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		ViewAngle = (ViewAngle + 45) % 360;
		glutPostRedisplay();
		break;
	}
}
