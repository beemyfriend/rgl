#ifndef RGLVIEW_H
#define RGLVIEW_H

// C++ header file
// This file is part of RGL
//
// $Id$


#include "scene.h"
#include "gui.hpp"
#include "fps.h"
#include "select.h"
#include "pixmap.h"

namespace rgl {

enum MouseModeID {mmTRACKBALL = 1, mmXAXIS, mmYAXIS, mmZAXIS, mmPOLAR, 
                  mmSELECTING, mmZOOM, mmFOV, mmUSER};
enum MouseSelectionID {msNONE=1, msCHANGING, msDONE, msABORT};

enum WheelModeID {wmPUSH = 1, wmPULL, wmUSER};

typedef void (*userControlPtr)(void *userData, int mouseX, int mouseY);
typedef void (*userControlEndPtr)(void *userData);
typedef void (*userCleanupPtr)(void **userData);
typedef void (*userWheelPtr)(void *userData, int dir);

class RGLView : public View
{
public:
  RGLView(Scene* scene);
  ~RGLView();
  bool snapshot(PixmapFileFormatID formatID, const char* filename);
  bool pixels(int* ll, int* size, int component, float* result);
  bool postscript(int format, const char* filename, bool drawText);
// event handler:
  void show(void);
  void hide(void);
  void paint(void);
  void resize(int width, int height);
  void buttonPress(int button, int mouseX, int mouseY);
  void buttonRelease(int button, int mouseX, int mouseY);
  void mouseMove(int mouseX, int mouseY);
  void wheelRotate(int dir);
  void captureLost();
  void keyPress(int code);
  Scene* getScene();

  MouseModeID getMouseMode(int button);
  void        setMouseMode(int button, MouseModeID mode);
  void        setMouseCallbacks(int button, userControlPtr begin, userControlPtr update, 
                                            userControlEndPtr end, userCleanupPtr cleanup, void** user);
  void        getMouseCallbacks(int button, userControlPtr *begin, userControlPtr *update, 
                                            userControlEndPtr *end, userCleanupPtr *cleanup, void** user);
  WheelModeID getWheelMode();
  void        setWheelMode(WheelModeID mode);
  void        setWheelCallback(userWheelPtr wheel, void* user);
  void        getWheelCallback(userWheelPtr *wheel, void** user);
  
  MouseSelectionID getSelectState();
  void        setSelectState(MouseSelectionID state);
  double*     getMousePosition();
  void        getUserMatrix(double* dest);
  void        setUserMatrix(double* src);
  void        getScale(double* dest);
  void        setScale(double* src);
  const char* getFontFamily() const;
  void        setFontFamily(const char *family);
  int         getFontStyle() const;
  void        setFontStyle(int style);
  double      getFontCex() const;
  void        setFontCex(double cex);
  bool        getFontUseFreeType() const;
  void        setFontUseFreeType(bool useFreeType);
  void	      setDefaultFont(const char *family, int style, double cex, bool useFreeType);
  const char* getFontname() const;
  
  /* NB:  these functions do not maintain consistency with userMatrix */
  
  void        getPosition(double* dest);
  void 	      setPosition(double* src);

protected:

  void setWindowImpl(WindowImpl* impl);


private:
  typedef void (RGLView::*viewControlPtr)(int mouseX,int mouseY);
  typedef void (RGLView::*viewControlEndPtr)();
  typedef void (RGLView::*viewWheelPtr)(int dir);
    

  viewControlPtr ButtonBeginFunc[3], ButtonUpdateFunc[3];
  viewControlEndPtr ButtonEndFunc[3];
  viewWheelPtr WheelRotateFunc;

  void setDefaultMouseFunc();

//
// DRAG USER-INPUT
//

  int drag, activeSubscene;
  int vwidth, vheight;      /* width and height of active subscene */
  
// Translate from OS window-relative coordinates (relative to top left corner) to
// OpenGL window relative (relative to bottom left corner)
  void translateCoords(int* mouseX, int* mouseY) const { *mouseY = height - *mouseY; }

// o DRAG FEATURE: adjustDirection

  void polarBegin(int mouseX, int mouseY);
  void polarUpdate(int mouseX, int mouseY);
  void polarEnd();

  void trackballBegin(int mouseX, int mouseY);
  void trackballUpdate(int mouseX, int mouseY);
  void trackballEnd();
  
  void oneAxisBegin(int mouseX, int mouseY);
  void oneAxisUpdate(int mouseX, int mouseY);  

  void wheelRotatePull(int dir);
  void wheelRotatePush(int dir);
  
  PolarCoord camBase, dragBase, dragCurrent;
  Vertex rotBase, rotCurrent, axis[3];


// o DRAG FEATURE: adjustZoom

  void adjustZoomBegin(int mouseX, int mouseY);
  void adjustZoomUpdate(int mouseX, int mouseY);
  void adjustZoomEnd();

  int zoomBaseY;

// o DRAG FEATURE: adjustFOV (field of view)

  void adjustFOVBegin(int mouseX, int mouseY);
  void adjustFOVUpdate(int mouseX, int mouseY);
  void adjustFOVEnd();

  int fovBaseY;
  
// o DRAG FEATURE: user supplied callback

  void userBegin(int mouseX, int mouseY);
  void userUpdate(int mouseX, int mouseY);
  void userEnd();
  
  void userWheel(int dir);
  
  void* userData[9];
  userControlPtr beginCallback[3], updateCallback[3];
  userControlEndPtr endCallback[3];
  userCleanupPtr cleanupCallback[3];
  int activeButton;
  bool busy;
  
  void* wheelData;
  userWheelPtr wheelCallback;

// o DRAG FEATURE: mouseSelection
  void mouseSelectionBegin(int mouseX,int mouseY);
  void mouseSelectionUpdate(int mouseX,int mouseY);
  void mouseSelectionEnd();

//
// RENDER SYSTEM
//
  
// o LAYERS
  
  Scene*  scene;
  FPS     fps;
  SELECT  select;

// o CONTEXT
  
  RenderContext renderContext;

  bool autoUpdate;

  enum {
    FSHOWFPS    = 1<<0,
    FAUTOUPDATE = 1<<1
  };

  int  flags;

  MouseModeID mouseMode[3];
  MouseSelectionID selectState;
  double  mousePosition[4];

  WheelModeID wheelMode;
};

} // namespace rgl

#endif /* RGLVIEW_H */
