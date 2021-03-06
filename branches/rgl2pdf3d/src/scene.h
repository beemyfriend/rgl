#ifndef SCENE_H
#define SCENE_H

// C++ header file
// This file is part of RGL
//
// $Id$

#include <vector>
#include "types.h"
#include "subscene.h"

namespace rgl {

class Scene {
public:
  Scene();
  ~Scene();

  // ---[ client services ]---------------------------------------------------

  /**
   * remove all nodes of the given type.  This is always recursive through subscenes.
   **/
  bool clear(TypeID type);
  
  /**
   * add node to current subscene
   **/
  bool add(SceneNode* node);
  
  /**
   * remove specified node of given type, or last-added if id==0
   **/
  bool pop(TypeID stackTypeID, int id);
  
  /**
   * hide specified node in all subscenes
   **/
  void hide(int id);
  
  /**
   * get information about stacks
   */
  int get_id_count(TypeID type);
  void get_ids(TypeID type, int* ids, char** types);
  
  /**
   * get a SceneNode by id
   */
   
  SceneNode* get_scenenode(int id);
  SceneNode* get_scenenode(TypeID type, int id);
  
  /**
   * get information about particular shapes
   **/
  Shape* get_shape(int id);
  
  /**
   * get information about particular lights
   **/
  Light* get_light(int id);
  
  /**
   * get a background
   */
  Background* get_background(int id);
  
  /**
   * get the bbox
   */
  BBoxDeco* get_bboxdeco(int id);
  
  /**
   * get subscene
   */
  Subscene* getSubscene(int id);
  Subscene* whichSubscene(int mouseX, int mouseY); /* coordinates are window-relative */

  /** 
   * set/get the current subscene
   **/
  void setCurrentSubscene(Subscene* subscene);
  Subscene* getCurrentSubscene() const { return currentSubscene; }
  const Subscene* getRootSubscene() const { return &rootSubscene; }
  
  // ---[ grouping component ]-----------------------------------------------
  
  /**
   * obtain subscene's axis-aligned bounding box. 
   **/
  const AABox& getBoundingBox() const { return currentSubscene->getBoundingBox(); }
  
  // ---[ Renderable interface ]---------------------------------------------
  
  /**
   * TODO: implements Renderable
   **/
  void render(RenderContext* renderContext);

  // ---[ bindable component ]-----------------------------------------------
  
  /**
   * obtain bounded viewpoint
   **/
  UserViewpoint* getUserViewpoint();
  ModelViewpoint* getModelViewpoint();
  
  /**
   * Get and set flag to ignore elements in bounding box
   **/
  
  int getIgnoreExtent(void) const { return doIgnoreExtent; }
  void setIgnoreExtent(int in_ignoreExtent) { doIgnoreExtent = in_ignoreExtent; }
  
  /**
   * invalidate display lists so objects will be rendered again
   **/
  void invalidateDisplaylists();

  // ---[ write PRC file ]---------------------------------------------
  
  /**
   * write PRC file
   **/
  vector<std::string> writePRC(const char* const basename, const RenderContext* const renderContext,
                  const bool writepdf, const bool writeprc, const bool writevws, const bool writejs);

  Subscene rootSubscene;  

private:

  // Whether objects created in this scene should affect the bounding box or not
  
  bool doIgnoreExtent;

  void setupLightModel();

  // --- [ Subscenes ]-------------------------------------------------------

  Subscene* currentSubscene;

  // ---[ nodes ]-----------------------------------------------------------
  
  /**
   * list of scene nodes.  The scene owns them, the subscenes display a subset.
   **/
  std::vector<SceneNode*> nodes;

  void deleteAll(std::vector<SceneNode*> list);

};

} // namespace rgl

#endif /* SCENE_H */

