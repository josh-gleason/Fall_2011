#include <GL/glew.h>
#include <GL/glut.h>

#include "Tessellate.h"
#include "InitShader.h"

// for debugging
#include <iostream>

struct PolyData
{
  std::vector<GLenum> type; // GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
  std::vector<unsigned> count; // number of vertices associated with type

  std::vector<vec2> verts;
  
  // vertices that were created in combine need to be deleted after tessellation
  std::vector<vec2*> deleteLater;
};

void beginCallback( GLenum type, void* polygon_data )
{
  PolyData* data = (PolyData*)polygon_data;
  
  // a new type is starting
  data->type.push_back(type);
  data->count.push_back(0);
}

void endCallback( void* polygon_data )
{}

void vertexCallback( void *vertex_data, void *polygon_data )
{
  // convert inputs
  PolyData* data = (PolyData*)polygon_data;
  vec2* vertex = (vec2*)vertex_data;

  data->count.back()++;

  data->verts.push_back(*vertex);
}

void combineCallback(GLdouble coords[3], void *vertex_data[4],
 GLfloat* weight, void **outData, void* polygon_data)
{
  PolyData* data = (PolyData*)polygon_data;
  vec2* out = new vec2;
  (*out)[0] = coords[0];
  (*out)[1] = coords[1];
  (*out)[2] = coords[2];
  *outData = out;
 
  // need to de-allocate him later
  data->deleteLater.push_back(out);
}

void tessellate(const vec4* input, unsigned insize, vec4** output, unsigned& outsize)
{
  GLUtesselator *tess;
  tess = gluNewTess();

  ASSERT( tess != 0, "Unable to allocate GLUtesselator" );

  // holds data used during callbacks
  PolyData polygon_data;

  // specify callbacks
  gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (void(*)())&vertexCallback);
  gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (void(*)())&beginCallback);
  //gluTessCallback(tess, GLU_TESS_END_DATA, (void(*)())&endCallback);
  gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (void(*)())&combineCallback);

  gluTessNormal(tess,0,0,1);

  // set to winding rule for non-simple polygons
  gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

  // begin polygon tesselation (single contour)
  gluTessBeginPolygon(tess, (void*)&polygon_data);
  gluTessBeginContour(tess);

  GLdouble** coords = new GLdouble*[insize];
  for ( unsigned i = 0; i < insize; ++i )
  {
    coords[i] = new GLdouble[3];
    coords[i][0] = input[i].x;
    coords[i][1] = input[i].y;
    coords[i][2] = 0.0;
  
    gluTessVertex(tess, coords[i], (void*)&(input[i]));
  }
 
  gluTessEndContour(tess);
  gluTessEndPolygon(tess);

  // delete tesslator
  gluDeleteTess(tess);

  // clean up coords
  for ( size_t i = 0; i < insize; ++i )
    delete [] coords[i];
  delete [] coords;

  // de-allocate any memory allocated in the combine function
  for ( std::vector<vec2*>::iterator i = polygon_data.deleteLater.begin();
        i != polygon_data.deleteLater.end(); ++i )
    delete (*i);

  // convert vertices so that they are all triangles
  std::vector<vec2> output_vec;

  unsigned offset = 0;
  for ( size_t i = 0; i < polygon_data.type.size(); ++i )
  {
    if ( polygon_data.type[i] == GL_TRIANGLES )
    {
      for ( size_t j = 0; j < polygon_data.count[i]; ++j )
        output_vec.push_back(polygon_data.verts[offset+j]);
    }
    else 
    {
      for ( size_t j = 2; j < polygon_data.count[i]; ++j )
      {
        // only difference between triangle strip and fan is 1st vert
        if ( polygon_data.type[i] == GL_TRIANGLE_STRIP )
          output_vec.push_back(polygon_data.verts[offset+j-2]);
        else
          output_vec.push_back(polygon_data.verts[offset]);

        output_vec.push_back(polygon_data.verts[offset+j-1]);
        output_vec.push_back(polygon_data.verts[offset+j]);
      }
    }
    
    // update offset
    offset += polygon_data.count[i];
  }

  outsize = output_vec.size();
  *output = new vec4[outsize];

  for ( int i = 0; i < outsize; ++i )
    (*output)[i] = vec4(output_vec[i].x, output_vec[i].y, 0.0, 1.0);
}
