/* Start Header -------------------------------------------------------
File Name: main.cpp
Purpose: Contains program entry point
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#include "Utilities.h"

#include "graphics/base/Rasterizer.hpp"
#include "Plane.hpp"
#include "Box.hpp"
#include "type/FileStream.hpp"

#include <locale>

int winID;
static graphics::Rasterizer gRasterizer ;
static PlaneModel *mod ;
enum {BOX_COUNT = 60};
static Box *gBoxes[BOX_COUNT] ;

static float32 CAM_DIST = 100.0F ;
static float32 CAM_HEIGHT = 15.0F ;
static float32 const BOX_SPREAD = 40 ;

static float32 const MOVE_SPEED = 5.0F ;
static float32 const TURN_SPEED = 3.14159F ;

static bool gFirstPerson = false ;

int32 RANDOM(int32 lo, int32 hi) {
	return (rand() % hi) + lo ;
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		mod->Roll(-0.016F * TURN_SPEED) ;
		break ;

	case 'd':
		mod->Roll(0.016F * TURN_SPEED) ;
		break ;
		
	case 'w':
		mod->Pitch(0.016F * TURN_SPEED) ;
		break ;

	case 's':
		mod->Pitch(-0.016F * TURN_SPEED) ;
		break ;
		
	case 'q':
		mod->Yaw(0.016F * TURN_SPEED) ;
		break ;

	case 'e':
		mod->Yaw(-0.016F * TURN_SPEED) ;
		break ;

	case ' ':
		mod->body.position += mod->direction.xyz() * mod->speed ;
		break ;

	case '1':
		gRasterizer.SetState(graphics::Rasterizer::State::WireFrame) ;
		break ;

	case '2':
		gRasterizer.SetState(graphics::Rasterizer::State::Fill) ;
		break ;
		case VK_ESCAPE:
			delete [] FrameBuffer::buffer;
			glutDestroyWindow(winID);
			exit(0);
			break;

		case '3':
			gFirstPerson = true ;
			break ;

		case '4':
			gFirstPerson = false ;
			break ;
	}
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
		default : break ;
	}
}

void mouseMove(int x, int y) {
}

void loop() {
	glutPostRedisplay();
}

void render() {
	FrameBuffer::Clear(255, 255, 255);
	DepthBuffer::Clear(1.0F) ;
	//Put your rendering code here

	if (!gFirstPerson) {
		gRasterizer.camera.position = mod->body.position - mod->direction * CAM_DIST + mod->up * CAM_HEIGHT ;
		gRasterizer.camera.view = mod->body.position - gRasterizer.camera.position ;
		gRasterizer.camera.right = mod->right ;
		gRasterizer.camera.up = math::Cross(gRasterizer.camera.right, gRasterizer.camera.view) ;
	} else {
		gRasterizer.camera.position = mod->body.position ;
		gRasterizer.camera.view = mod->direction ;
		gRasterizer.camera.right = mod->right ;
		gRasterizer.camera.up = math::Cross(gRasterizer.camera.right, gRasterizer.camera.view) ;
	}
	
	gRasterizer.Rasterize() ;

	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, FrameBuffer::buffer);
	glutSwapBuffers();
}

void init() {
	FrameBuffer::Init(WIDTH, HEIGHT);
	DepthBuffer::Init(WIDTH, HEIGHT) ;
	//Initialize everything here
	mod = new PlaneModel ;
	
	for (uint32 i = 0; i < BOX_COUNT; ++i)
		gBoxes[i] = new Box ;

	graphics::Mesh &m = mod->mesh ;

	FileStream fstream ;
	fstream.Parse("input.txt") ;

	fstream.GetVarBlock("camera", "focal",  gRasterizer.camera.focal) ;
	fstream.GetVarBlock("camera", "left",   gRasterizer.camera.l) ;
	fstream.GetVarBlock("camera", "right",  gRasterizer.camera.r) ;
	fstream.GetVarBlock("camera", "top",    gRasterizer.camera.t) ;
	fstream.GetVarBlock("camera", "bottom", gRasterizer.camera.b) ;
	fstream.GetVarBlock("camera", "near",   gRasterizer.camera.near) ;
	fstream.GetVarBlock("camera", "far",    gRasterizer.camera.far) ;
	

	float32 vertex_count ;
	fstream.GetVarBlock("vertexes", "count", vertex_count) ;
	// adjust for "count" var
	for (uint32 i = 2; i < vertex_count + 2; ++i) {
		Vec4 pos ;
		fstream.ReadSet("vertexes", i, pos.x, pos.y, pos.z, pos.w) ;
		m.verts.push_back(graphics::Vertex(pos)) ;
	}

	float32 face_count ;
	fstream.GetVarBlock("faces", "count", face_count) ;

	for (uint32 i = 2; i < face_count + 2; ++i) {
		uint32 i1, i2, i3 ;
		fstream.ReadSet("faces", i, i1, i2, i3) ;
		m.inds.push_back(i1) ;
		m.inds.push_back(i2) ;
		m.inds.push_back(i3) ;
	}

	
	// build vertices
	Array<graphics::Vertex> vertices ;
	for (uint32 i = 0; i < m.inds.Size(); ++i) {
		vertices.push_back(m.verts[m.inds[i]]) ;
	}

	m.inds.Clear() ;
	for (uint32 i = 0; i < vertices.Size(); ++i) {
		m.inds.push_back(i) ;
	}


	for (uint32 i = 0; i < vertices.Size(); ++i) {
		float32 u, v ;
		fstream.ReadSet("texturecoordinates", i, u, v) ;
		vertices[i].texCoord.u = u ;
		vertices[i].texCoord.v = v ;
	}

	m.verts = vertices ;

	uint32 i = 0, j = 0 ;
	for (Box *b : gBoxes) {
		b->mesh = m ;

		for (uint32 ijk = 0; ijk < b->mesh.verts.Size(); ++ijk) {
			b->mesh.verts[ijk].color.r = RANDOM(0, 255) ;
			b->mesh.verts[ijk].color.g = RANDOM(0, 255) ;
			b->mesh.verts[ijk].color.b = RANDOM(0, 255) ;
		}

		if (i && i % 4 == 0) ++j ;
		++i ;

		b->transform.position += Vec3((i % (BOX_COUNT / 4)) * BOX_SPREAD, 0, j * BOX_SPREAD) ;
		b->transform.scale = Vec4(RANDOM(5, 30), RANDOM(5, 30), RANDOM(5, 30), 1) ;
	}

	/*
	Body Sx=15,Sy=12.5, Sz=4
	
	Tail
	scale values: Sx=5, Sy=	7.5, Sz=10
	position: (0, 10,-15)

	LeftWing
	scale values: Sx=20, Sy=5, Sz=10
	position: (-17.5, 0, 0)
	
	Right
	scale values: Sx=20, Sy=5, Sz=10
	position: (17.5, 0, 0
	*/

	mod->body.position = Vec3(0, 0, -200) ;
	mod->body.scale = Vec4(15, 12.5, 40, 1) ;

	mod->tail.position = Vec3(0, 10, -15) ;
	mod->tail.scale = Vec4(5, 7.5, 10, 1) ;

	mod->leftWing.position = Vec3(-17.5, 0, 0) ;
	mod->leftWing.scale = Vec4(20, 5, 10, 1) ;

	mod->rightWing.position = Vec3(17.5, 0, 0) ;
	mod->rightWing.scale = Vec4(20, 5, 10, 1) ;

	gRasterizer.Add(mod) ;
	
	for (Box *b : gBoxes)
		gRasterizer.Add(b) ;

	gRasterizer.Perspective(gRasterizer.camera.yFOV, float(WIDTH)/HEIGHT, gRasterizer.camera.near, gRasterizer.camera.far) ;
}

int main(int argc, char **argv) {
	srand(time(0)) ;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	winID = glutCreateWindow("CS250");

	glClearColor(0, 0, 0, 1);

	glutKeyboardFunc(keyboard);
	glutDisplayFunc(render);
	glutIdleFunc(loop);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);

	init();

	glutMainLoop();
	return 0;
}