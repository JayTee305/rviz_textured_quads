/*
 * MeshDisplayCustom declaration.
 *
 * Author: Felipe Bacim.
 *
 * help with selection of robot parts
 */
/*
 * Copyright (c) 2008, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RVIZ_MESH_DISPLAY_H
#define RVIZ_MESH_DISPLAY_H

#include <QObject>
// kinetic compatibility http://answers.ros.org/question/233786/parse-error-at-boost_join/
#ifndef Q_MOC_RUN

#include "rviz/display.h"
#include "rviz/frame_manager.h"
#include "rviz/image/image_display_base.h"
#include "rviz/image/ros_image_texture.h"

#include <image_transport/image_transport.h>
#include <sensor_msgs/CameraInfo.h>
#include <message_filters/subscriber.h>
#include <tf/message_filter.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <shape_msgs/Mesh.h>
#include <std_msgs/Float64.h>

#include <tf/transform_listener.h>

#include <OGRE/OgreVector3.h>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreRenderTargetListener.h>
#include <OGRE/OgreRenderQueueListener.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <sensor_msgs/Image.h>
#include <tf/tf.h>
#include <tf_conversions/tf_eigen.h>
#include <eigen_conversions/eigen_msg.h>

#include <map>
#include <vector>

#endif  // Q_MOC_RUN

namespace Ogre
{
class Entity;
class SceneNode;
class ManualObject;
}

namespace rviz
{
class FloatProperty;
class RenderPanel;
class RosTopicProperty;
class TfFrameProperty;

/**
 * \class MeshDisplayCustom
 * \brief Uses a pose from topic + offset to render a bounding object with shape, size and color
 */
class MeshDisplayCustom: public rviz::Display,  public Ogre::RenderTargetListener, public Ogre::RenderQueueListener
{
  Q_OBJECT
public:
  MeshDisplayCustom();
  virtual ~MeshDisplayCustom();

  // Overrides from Display
  virtual void onInitialize();
  virtual void update(float wall_dt, float ros_dt);
  virtual void reset();

private Q_SLOTS:
  void updateMeshProperties();
  void updateDisplayImages();

protected:
  virtual void load();

  // overrides from Display
  virtual void onEnable();
  virtual void onDisable();

  // This is called by incomingMessage().
  void processImage(int index, const sensor_msgs::Image& msg);

  virtual void subscribe();
  virtual void unsubscribe();

private:
  void clear();
  bool updateCamera(bool update_image);

  void createProjector(int index);
  void addDecalToMaterial(int index, const Ogre::String& matName);

  void updateImageMeshes(const sensor_msgs::Image::ConstPtr& images);
  void constructQuads(const sensor_msgs::Image::ConstPtr& images);

  shape_msgs::Mesh constructMesh(geometry_msgs::Pose mesh_origin, float width, float height, float border_size);
  void clearStates();

  float time_since_last_transform_;

  RosTopicProperty* image_topic_property_;
  TfFrameProperty* tf_frame_property_;
  FloatProperty* meters_per_pixel_property_;
  ros::Subscriber image_sub_;

  std::vector<shape_msgs::Mesh> last_meshes_;
  std::vector<geometry_msgs::Pose> mesh_poses_;
  int img_widths_, img_heights_;
  float physical_widths_, physical_heights_;
  std::vector<float> border_colors_;
  float border_sizes_;

  ros::NodeHandle nh_;

  std::vector<sensor_msgs::Image::ConstPtr> last_images_;

  Ogre::SceneNode* mesh_nodes_;
  Ogre::ManualObject* manual_objects_;
  Ogre::MaterialPtr mesh_materials_;
  ROSImageTexture* textures_;

  Ogre::Frustum* decal_frustums_;
  // need multiple filters (back, up, down, left, right)
  std::vector<Ogre::Frustum*> filter_frustums_;
  Ogre::SceneNode* projector_nodes_;

  RenderPanel* render_panel_;  // this is the active render panel

  boost::mutex mesh_mutex_;
};

}  // namespace rviz

#endif


