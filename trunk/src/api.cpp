// C++ source
// This file is part of RGL.
//
// $Id$

#include "lib.hpp"

extern "C" {

#include "api.h"

} // extern C

#include "DeviceManager.hpp"
#include "rglview.h"

//
// GLOBAL: deviceManager pointer
//

DeviceManager* deviceManager = NULL;

//
// FUNCTION
//   rgl_init
//

#include "lib.hpp"

void rgl_init(int* successptr)
{
  bool success = false;

  if ( lib::init() ) {
    deviceManager = new DeviceManager();
    success = true;
  }

  *successptr = (int) success;
}

//
// FUNCTION
//   rgl_quit
//
// DESCRIPTION
//   Gets called by .Last.lib ( R function )
//

void rgl_quit(int* successptr)
{
  if (deviceManager) {
    delete deviceManager;
    deviceManager = NULL;
  }

  lib::quit();

  *successptr = (int) true;
}

//
// FUNCTION
//   rgl_dev_open
//

void rgl_dev_open(int* successptr)
{
  bool success;

  success = deviceManager->openDevice();

  *successptr = (int) success;
}


//
// FUNCTION
//   rgl_dev_close
//

void rgl_dev_close(int* successptr)
{
  bool success = false;

  Device* device;

  device = deviceManager->getCurrentDevice();

  if (device) {

    device->close();
    success = true;

  }

  *successptr = (int) success;
}

#ifdef _WIN32
void rgl_dev_bringtotop(int* successptr, int* stay)
{
  bool success = false;

  Device* device;

  device = deviceManager->getCurrentDevice();

  if (device) {

    device->bringToTop(*stay);
    success = true;

  }

  *successptr = (int) success;
}
#endif

//
// FUNCTION
//   rgl_dev_getcurrent
//
// RETURNS
//   device id
//

void rgl_dev_getcurrent(int* successptr, int* idptr)
{
  int id;

  id  = deviceManager->getCurrent();

  *idptr = id;
  *successptr = true;
}

//
// FUNCTION
//   rgl_dev_setcurrent
//
// PARAMETERS
//   idata
//     [0]  device id
//

void rgl_dev_setcurrent(int* successptr, int* idata)
{
  bool success = false;

  int id = idata[0];

  success = deviceManager->setCurrent(id);

  *successptr = (int) success;
}


//
// SCENE MANAGEMENT
//

static Material currentMaterial(Color(1.0f,1.0f,1.0f),Color(1.0f,0.0f,0.0f));

//
// FUNCTION
//   rgl_clear ( successPtr, idata(type) )
//
// PARAMETERS
//   idata
//     [0]  TypeID
//
//

void rgl_clear(int* successptr, int *idata)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if (device) {

    TypeID stackTypeID = (TypeID) idata[0];

    success = device->clear( stackTypeID );

  }

  *successptr = success;
}


//
// FUNCTION
//   rgl_pop  ( successPtr, idata )
//
// PARAMETERS
//   idata
//     [0]  stack TypeID
//
//


void rgl_pop(int* successptr, int* idata)
{
  bool success = false;
  Device* device = deviceManager->getCurrentDevice();

  if (device) {

    TypeID stackTypeID = (TypeID) idata[0];

    success = device->pop( stackTypeID );

  }

  *successptr = success;
}


//
// FUNCTION
//   rgl_bg   ( successPtr, idata )
//
// PARAMETERS
//   idata
//     [0]  bool environment sphere
//     [1]  int  fogtype
//


void rgl_bg(int* successptr, int* idata)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();
  if (device) {
    bool sphere    = (bool) idata[0];
    int  fogtype   = idata[1];

    success = device->add( new Background(currentMaterial, sphere, fogtype) );
  }

  *successptr = success;
}


//
// FUNCTION
//   rgl_light   ( successPtr, idata, cdata, ddata )
//

void rgl_light ( int* successptr, int* idata, double* ddata )
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if ( device ) {

    bool  viewpoint_rel = (bool)  idata[0];

    Color ambient;
    Color diffuse;
    Color specular;

    ambient.set3iv ( &idata[1] );
    diffuse.set3iv ( &idata[4] );
    specular.set3iv( &idata[7] );

    float theta         = (float) ddata[0];
    float phi           = (float) ddata[1];

    success = device->add( new Light( PolarCoord(theta, phi), (bool) viewpoint_rel, ambient, diffuse, specular ) );

  }

  *successptr = success;
}


void rgl_viewpoint(int* successptr, int* idata, double* ddata)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if (device) {

    float fov         = (float) ddata[0];
    float zoom        = ((float) ddata[1] - 1.0f)/((float)(VIEWPOINT_MAX_ZOOM-1));

    int   interactive =         idata[0];

    success = device->add( new Viewpoint(ddata+2, fov, zoom, interactive) );

  }

  *successptr = success;
}

void rgl_getZoom(int* successptr, double* zoom)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if ( device ) {
    RGLView* rglview = device->getRGLView();
    Scene* scene = rglview->getScene();
    Viewpoint* viewpoint = scene->getViewpoint();
    *zoom = viewpoint->getZoom();
    *zoom = 1.0f+(*zoom)* ((float)(VIEWPOINT_MAX_ZOOM-1)) ;
    success = true;
  }
  *successptr = success;
}

void rgl_setZoom(int* successptr, double* zoom)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if ( device ) {
    RGLView* rglview = device->getRGLView();
    Scene* scene = rglview->getScene();
    Viewpoint* viewpoint = scene->getViewpoint();
    viewpoint->setZoom((*zoom - 1.0f)/((float)(VIEWPOINT_MAX_ZOOM-1)));
    rglview->update();
    success = true;
  }
  *successptr = success;
}

void rgl_getFOV(int* successptr, double* fov)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if ( device ) {
    RGLView* rglview = device->getRGLView();
    Scene* scene = rglview->getScene();
    Viewpoint* viewpoint = scene->getViewpoint();
    *fov = viewpoint->getFOV();
    success = true;
  }
  *successptr = success;
}

void rgl_setFOV(int* successptr, double* fov)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if ( device ) {
    RGLView* rglview = device->getRGLView();
    Scene* scene = rglview->getScene();
    Viewpoint* viewpoint = scene->getViewpoint();
    viewpoint->setFOV(*fov);
    rglview->update();
    success = true;
  }
  *successptr = success;
}

void rgl_primitive(int* successptr, int* idata, double* vertex)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if ( device ) {

    int   type    = idata[0];
    int   nvertex = idata[1];

    SceneNode* node;

    switch(type) {
    case 1: // RGL_POINTS:
      node = new PointSet( currentMaterial, nvertex, vertex);
      break;
    case 2: // RGL_LINES:
      node = new LineSet( currentMaterial, nvertex, vertex);
      break;
    case 3: // RGL_TRIANGLES:
      node = new TriangleSet( currentMaterial, nvertex, vertex);
      break;
    case 4: // RGL_QUADS:
      node = new QuadSet( currentMaterial, nvertex, vertex);
      break;
    case 5: // RGL_LINE_STRIP:
      node = new LineStripSet( currentMaterial, nvertex, vertex);
      break;
    default:
      node = NULL;
    }

    if (node)
      success = device->add( node );
  }

  *successptr = (int) success;
}

void rgl_surface(int* successptr, int* idata, double* x, double* z, double* y)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if (device) {
    int nx         = idata[0];
    int nz         = idata[1];

    success = device->add( new Surface(currentMaterial, nx, nz, x, z, y) );

  }

  *successptr = (int) success;
}

void rgl_spheres(int* successptr, int* idata, double* vertex, double* radius)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if (device) {
    int nvertex = idata[0];
    int nradius = idata[1];

    success = device->add( new SphereSet(currentMaterial, nvertex, vertex, nradius, radius) );
  }

  *successptr = (int) success;
}

void rgl_sprites(int* successptr, int* idata, double* vertex, double* radius)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if (device) {
    int nvertex = idata[0];
    int nradius = idata[1];

    success = device->add( new SpriteSet(currentMaterial, nvertex, vertex, nradius, radius) );
  }

  *successptr = (int) success;
}

void rgl_material(int *successptr, int* idata, char** cdata, double* ddata)
{
  Material& mat = currentMaterial;

  bool success = false;

  int ncolor    = idata[0];
  mat.lit       = (idata[1]) ? true : false;
  mat.smooth    = (idata[2]) ? true : false;
  mat.front     = (Material::PolygonMode) idata[3];
  mat.back      = (Material::PolygonMode) idata[4];
  mat.fog       = (idata[5]) ? true : false;
  Texture::Type textype = (Texture::Type) idata[6];
  bool mipmap = (idata[7]) ? true : false;
  int  minfilter = idata[8];
  int  magfilter = idata[9];
  int    nalpha = idata[10];
  mat.ambient.set3iv( &idata[11] );
  mat.specular.set3iv( &idata[14] );
  mat.emission.set3iv( &idata[17] );
  int* colors   = &idata[20];

  char*  pixmapfn = cdata[0];

  mat.shininess   = (float) ddata[0];
  mat.size        = (float) ddata[1];
  double* alpha   = &ddata[2];

  if ( strlen(pixmapfn) > 0 ) {
    mat.texture = new Texture(pixmapfn, textype, mipmap, (unsigned int) minfilter, (unsigned int) magfilter);
    if ( !mat.texture->isValid() ) {
      mat.texture->unref();
      // delete mat.texture;
      mat.texture = NULL;
    }
  } else
    mat.texture = NULL;

  mat.colors.set( ncolor, colors, nalpha, alpha);
  mat.alphablend  = mat.colors.hasAlpha();

  mat.setup(); 

  success = true;

  *successptr = success;
}

void rgl_texts(int* successptr, int* idata, double* adj, char** text, double* vertex)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if (device) {
    int ntext   = idata[0];

    success = device->add( new TextSet(currentMaterial, ntext, text, vertex, *adj) );
  }

  *successptr = (int) success;
}

void rgl_bbox(int* successptr,
              int* idata,
              double* ddata,
              double* xat, char** xtext,
              double* yat, char** ytext,
              double* zat, char** ztext)
{
  bool success = false;

  Device* device = deviceManager->getAnyDevice();

  if (device) {

    int   xticks     =        idata[0];
    int   yticks     =        idata[1];
    int   zticks     =        idata[2];
    int   xlen       =        idata[3];
    int   ylen       =        idata[4];
    int   zlen       =        idata[5];
    int   marklen_rel =       idata[6];

    float xunit      = (float) ddata[0];
    float yunit      = (float) ddata[1];
    float zunit      = (float) ddata[2];
    float marklen    = (float) ddata[3];

    AxisInfo xaxis(xticks, xat, xtext, xlen, xunit);
    AxisInfo yaxis(yticks, yat, ytext, ylen, yunit);
    AxisInfo zaxis(zticks, zat, ztext, zlen, zunit);

    success = device->add( new BBoxDeco(currentMaterial, xaxis, yaxis, zaxis, marklen, (bool) marklen_rel ) );
  }

  *successptr = (int) success;
}

void rgl_snapshot(int* successptr, int* idata, char** cdata)
{
  bool success = false;

  Device* device = deviceManager->getCurrentDevice();

  if (device) {

    int   format   = idata[0];
    char* filename = cdata[0];

    success = device->snapshot( format, filename );
  }

  *successptr = (int) success;
}


void rgl_user2window(int* successptr, int* idata, double* point, double* pixel, double* model, double* proj, int* view)
{
  bool success = false;
  GLdouble* vertex = pixel;
  int columns = idata[0];

  Device* device = deviceManager->getAnyDevice();

  if ( device ) {
  	for (int i=0; i<columns; i++) {
		gluProject(point[0],point[1],point[2],model,proj,view,
		vertex,vertex+1,vertex+2);
		vertex[0] /= view[2];
		vertex[1] /= view[3];
		point += 3;
		vertex += 3;
	}
	success = true;
  }

  *successptr = (int) success;
}

void rgl_window2user(int* successptr, int* idata, double* point, double* pixel, double* model, double* proj, int* view)
{
  bool success = false;
  GLdouble* vertex = point;
  int columns = idata[0];

  Device* device = deviceManager->getAnyDevice();

  if ( device ) {
  	for (int i=0; i<columns; i++) {
	        pixel[0] *= view[2];
	        pixel[1] *= view[3];
		gluUnProject(pixel[0],pixel[1],pixel[2],model,proj,view,
		vertex,vertex+1,vertex+2);
		pixel += 3;
		vertex += 3;
	}
	success = true;
  }

  *successptr = (int) success;
}

void rgl_getMouseMode(int* successptr, int *button, int* mode)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if (device) {
 	RGLView* rglview = device->getRGLView();
  	*mode = static_cast<int>( rglview->getMouseMode(*button) );
    	success = true;
  }

  *successptr = success;
}

void rgl_setMouseMode(int* successptr, int* button, int* mode)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if (device) {
 	RGLView* rglview = device->getRGLView();
	rglview->setMouseMode(*button, (MouseModeID)(*mode));

    	success = true;
  }

  *successptr = success;
}


void rgl_selectstate(int* successptr, int* selectstate, double* locations)
{
	bool success = false;
	Device* device;

	device = deviceManager->getAnyDevice();

  	if (device){

		RGLView* rglview = device->getRGLView();
		selectstate[0] = (int)rglview->getSelectState();
		double* mousePosition = rglview->getMousePosition();

		locations[0] = *mousePosition;
		locations[1] = *(mousePosition+1);
		locations[2] = *(mousePosition+2);
		locations[3] = *(mousePosition+3);

		success = true;
	}

	*successptr = (int) success;

}

void rgl_setselectstate(int* successptr, int *idata)
{
  bool success = false;
  Device* device = deviceManager->getAnyDevice();

  if (device) {

    MouseSelectionID selectState = (MouseSelectionID) idata[0];
	RGLView* rglview = device->getRGLView();
	rglview->setSelectState(selectState);

    success = true;

  }

  *successptr = success;
}

void rgl_getUserMatrix(int* successptr, double* userMatrix)
{
	bool success = false;
  	Device* device = deviceManager->getAnyDevice();

  	if (device) {

		RGLView* rglview = device->getRGLView();
		rglview->getUserMatrix(userMatrix);

    	success = true;

  	}

  *successptr = success;
}

void rgl_setUserMatrix(int* successptr, double* userMatrix)
{

	bool success = false;
  	Device* device = deviceManager->getAnyDevice();

  	if (device) {

		RGLView* rglview = device->getRGLView();
		rglview->setUserMatrix(userMatrix);

		success = true;

  	}

  *successptr = success;

}

void rgl_getModelMatrix(int* successptr, double* modelMatrix)
{
	bool success = false;
  	Device* device = deviceManager->getAnyDevice();

	if (device){

		RGLView* rglview = device->getRGLView();
		for (int i=0; i<16; i++) {
	    		modelMatrix[i] = rglview->modelMatrix[i];
		}
    		success = true;
  	}

  *successptr = success;
}

void rgl_getProjMatrix(int* successptr, double* projMatrix)
{
	bool success = false;
  	Device* device = deviceManager->getAnyDevice();

	if (device){

		RGLView* rglview = device->getRGLView();
		for (int i=0; i<16; i++) {
	    		projMatrix[i] = rglview->projMatrix[i];
		}
    		success = true;
  	}

  *successptr = success;
}

void rgl_getViewport(int* successptr, int* viewport)
{
	bool success = false;
  	Device* device = deviceManager->getAnyDevice();

	if (device){

		RGLView* rglview = device->getRGLView();
		for (int i=0; i<4; i++) {
	    		viewport[i] = rglview->viewport[i];
		}
    		success = true;
  	}

  *successptr = success;
}

void rgl_postscript(int* successptr, int* idata, char** cdata)
{
  bool success = false;
 
  Device* device = deviceManager->getCurrentDevice();

  if (device) {

    int   format   = idata[0];
    char* filename = cdata[0];

    success = device->postscript( format, filename );
  }

  *successptr = (int) success;
}

