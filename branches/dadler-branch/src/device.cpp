// C++ source
// This file is part of RGL.
//
// $Id: device.cpp,v 1.2.2.1 2004/06/10 23:10:24 dadler Exp $

#include "device.h"
#include "rglview.h"
#include "gui.h"

/**
 * Command class
 **/

Command::Command(Device* me, CommandPtr f, void* u) : Event( static_cast<IEventHandler*>(me) ), func(f) , userdata(u)
{ }

/**
 * Device class
 **/

Device::Device()
{
  destroyHandler = NULL;
  window = NULL;
  scene = NULL;
}

Device::~Device()
{
  if (destroyHandler)
    destroyHandler->notifyDestroy(destroyHandler_userdata);

  if (window) {
    window->setDestroyHandler(NULL, NULL);
    delete window;
  }

  if (scene) {
    delete scene;
  }
}

void Device::processEvent(Event* e)
{
  printMessage("Device: processEvent");
  Command* ethis = reinterpret_cast<Command*>(e);
    (this->*(ethis->func) )( ethis->userdata);
}

void Device::do_open(void*)
{
  printMessage("Device: open window...");
  window->setVisibility(true);  
}
 
#include "lib.h"

bool Device::open()
{
  printMessage("Manager: starting thread now...");
  start();
  printMessage("Manager: Device started... sending command...");
  postEvent( new Command(this,&Device::do_open,NULL) );
  printMessage("Manager: posted...");
  return true;
}

void Device::do_close(void*)
{
  
}

void Device::close()
{
  /*
  post( new Command(this,&Device::do_close,NULL) );
  Thread::sleep(5);
  delete this;
   */
}

/**
 * thread method
 **/

void Device::run()
{
  printMessage("Device: init console ...");
  scene   = new Scene();
  rglview = new RGLView(scene);
  window  = new Window( rglview, getGUIFactory() );
  window->setDestroyHandler(this, window);
  printMessage("Device: run main loop...");

  Task::run();
}

void Device::shutdown()
{
  delete this;
}

void Device::notifyDestroy(void* userdata)
{
  window = NULL;
  delete this;
}

void Device::setDestroyHandler(DestroyHandler* inDestroyHandler, void* userdata)
{
  destroyHandler = inDestroyHandler;
  destroyHandler_userdata = userdata;
}

#include <cstring>

void Device::do_setName(void* userdata)
{
  char* string = reinterpret_cast<char*>(userdata);
  window->setTitle(string);
  free(string);
}

void Device::setName(char* string)
{
  postEvent( new Command(this,&Device::do_setName,reinterpret_cast<void*>( strdup(string) ) ) );
}

void Device::update()
{
//  window->update();
}


void Device::bringToTop(void)
{
#ifdef _WIN32
  window->bringToTop();
#endif
}

//
// scene management:
//

#include "lib.h"

void Device::do_clear(void* userdata)
{
  printMessage("clear!");
  TypeID stackTypeID = (int) userdata;
  bool success;
  if ( success = scene->clear(stackTypeID) )
    rglview->update();

  // return success;
}

bool Device::clear(TypeID stackTypeID)
{
  postEvent( new Command(this, &Device::do_clear, reinterpret_cast<void*>(stackTypeID) ) );
  return true;
}

void Device::do_add(void* userdata)
{
  SceneNode* node = reinterpret_cast<SceneNode*>(userdata);
  bool success;
  if ( success = scene->add(node) )
    rglview->update();
  // return success;
}

bool Device::add(SceneNode* node)
{
  postEvent( new Command(this, &Device::do_add, reinterpret_cast<void*>(node) ) );  
  return true;
}

void Device::do_pop(void* userdata)
{
  TypeID stackTypeID = reinterpret_cast<TypeID>(userdata);
  bool success;
  if ( success = scene->pop(stackTypeID) )
    rglview->update();
//  return success;
}

bool Device::pop(TypeID stackTypeID)
{
  postEvent( new Command(this, &Device::do_pop, reinterpret_cast<void*>(stackTypeID) ) );  
  return true;
}

//
// export
//

void Device::do_snapshot(void* userdata)
{
  PixmapFileFormatID format = PIXMAP_FILEFORMAT_PNG;
  char* filename = reinterpret_cast<char*>(userdata);

  rglview->snapshot( (PixmapFileFormatID) format, filename );
  free(filename);
}

bool Device::snapshot(int format, const char* filename)
{
  postEvent( new Command(this, &Device::do_snapshot, reinterpret_cast<void*>( strdup(filename) ) ) );  
  return true;
}
