#include "Poly.h"
#include "SketchPadDefs.h"
#include "InitShader.h"

Poly::Poly() : Shape::Shape() {}

Poly::Poly(const vec2* vertices, int vertex_count, bool filled,
  const vec4& color, GLfloat thickness)
{
  m_params.color = color;
  m_params.thickness = thickness;

  // build m_outline which is used in the fill/unFill functions
  for ( int i = 0; i < vertex_count; ++i )
    m_outline.push_back(vertices[i]);

  if ( filled )
    fillShape();
  else
    unFillShape();
}

Poly::Poly(const vec2& startPoint, bool filled, const vec4& color,
  GLfloat thickness)
{
  // TODO
}

Poly::Poly(const Poly& rhs) :
  Shape::Shape(rhs)
{}

const Poly& Poly::operator=(const Poly& rhs)
{
  Shape::operator=(rhs);
}

void Poly::fillShape()
{
  resetShape(false);
  
  m_params.filled = true;
  triangulateVertices();
}

void Poly::unFillShape()
{
  resetShape(false);
  
  m_params.filled = false;
  m_vertex_count = m_outline.size();
  m_vertices = new vec4[m_vertex_count];
  for ( int i = 0; i < m_vertex_count; ++i ) 
    m_vertices[i] = vec4(m_outline[i][0], m_outline[i][1], 0.0, 1.0);
  m_shader.drawMode = GL_LINE_LOOP;
}

bool Poly::isInside(vec2 loc) const
{
  return true;// TODO
}

void Poly::mouseDown(vec2 cameraCoordLoc, int mode)
{
  // TODO  
}

void Poly::mouseMove(vec2 cameraCorrdLoc, int mode)
{
  // TODO  
}

void Poly::mouseUp(vec2 cameraCorrdLoc, int mode)
{
  // TODO  
}

void beginCallback( GLenum type, void* polygon_data )
{
  Poly::PolyData* data = (Poly::PolyData*)polygon_data;

  if ( type == GL_TRIANGLE_FAN )
    std::cout << "GL_TRIANGLE_FAN" << std::endl;
  else if ( type == GL_TRIANGLE_STRIP )
    std::cout << "GL_TRIANGLE_STRIP" << std::endl;
  else if ( type == GL_TRIANGLES )
    std::cout << "GL_TRIANGLES" << std::endl;
  else
    std::cout << "OTHER" << std:: endl;

  data->shader->drawMode = type;
}

void endCallback( void* polygon_data )
{}

void vertexCallback( void *vertex_data, void *polygon_data )
{
  GLdouble* vertex = (GLdouble*)vertex_data;
  std::cout << "VERTEX";
  std::cout << "( " << vertex[0] << ", " << vertex[1] << " )" << std::endl;
  
  Poly::PolyData* data = (Poly::PolyData*)polygon_data;

  data->verts.push_back(vec2(vertex[0],vertex[1]));
}

void combineCallback(GLdouble coords[3], void *vertex_data[4],
 GLfloat* weight, void **outData, void* polygon_data)
{
  std::cout << "COMBINE" << std::endl;
  GLdouble* out = new GLdouble[3];
  out[0] = coords[0];
  out[1] = coords[1];
  out[2] = coords[2];
  *outData = out;
}

void Poly::triangulateVertices()
{
  GLUtesselator *tess;
  tess = gluNewTess();

  ASSERT( tess != 0, "Unable to allocate GLUtesselator" );

  // holds data used during callbacks
  PolyData polygon_data;
  polygon_data.params = &m_params;
  polygon_data.shader = &m_shader;
  //polygon_data.verts = std::vector<vec2>(); // alread initialized empty

  // specify callbacks
  gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (void(*)())&vertexCallback);
  gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (void(*)())&beginCallback);
  gluTessCallback(tess, GLU_TESS_END_DATA, (void(*)())&endCallback);
  gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (void(*)())&combineCallback);

  // gluTessNormal(tess,0,0,-1);

  // set to winding rule for non-simple polygons
  gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

  // begin polygon tesselation (single contour)
  gluTessBeginPolygon(tess, (void*)&polygon_data);
  gluTessBeginContour(tess);

  GLdouble** coords = new GLdouble*[m_outline.size()];
  for ( size_t i = 0; i < m_outline.size(); ++i )
  {
    coords[i] = new GLdouble[3];
    coords[i][0] = m_outline[i].x;
    coords[i][1] = m_outline[i].y;
    coords[i][2] = 0.0;
    
    gluTessVertex(tess, coords[i], coords[i]);
  }
  
  gluTessEndContour(tess);
  gluTessEndPolygon(tess);

  // clean up coords
  for ( size_t i = 0; i < m_outline.size(); ++i )
    delete [] coords[i];
  delete [] coords;

  // load back into m_vertices
  m_vertex_count = polygon_data.verts.size();
  m_vertices = new vec4[m_vertex_count];
  
  for ( size_t i = 0; i < m_vertex_count; ++i )
    m_vertices[i] = vec4(polygon_data.verts[i].x,polygon_data.verts[i].y,0.0,1.0);

  // delete tesslator
  gluDeleteTess(tess);
}

/*
void Poly::triangulateVertices()
{
  // triangulate the points
  tpp::Delaunay del(m_outline);
  del.Triangulate();

  // load back into m_vertices
  resetShape(false);
  m_vertex_count = del.ntriangles()*3;
  m_vertices = new vec4[m_vertex_count];
  
  unsigned index = 0;
  for ( tpp::Delaunay::fIterator fit = del.fbegin(); fit != del.fend(); ++fit )
  {
    static tpp::Delaunay::Point p[3];
    p[0] = del.point_at_vertex_id(del.Org(fit));
    p[1] = del.point_at_vertex_id(del.Dest(fit));
    p[2] = del.point_at_vertex_id(del.Apex(fit));
    m_vertices[index++] = vec4(p[0][0], p[0][1], 0.0, 1.0);
    m_vertices[index++] = vec4(p[1][0], p[1][1], 0.0, 1.0);
    m_vertices[index++] = vec4(p[2][0], p[2][1], 0.0, 1.0);
  }
	
  for ( int i = 0; i < m_vertex_count; ++i )
  {
    std::cout << m_vertices[i] << ' ';
    if ( (i+1)%3 == 0 )
      std::cout << std::endl;
  }

}*/
