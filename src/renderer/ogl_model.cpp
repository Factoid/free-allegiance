#include "ogl_common.hpp"
#include "ogl_model.hpp"
#include "resource_exception.hpp"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <boost/log/trivial.hpp>

OGLModel::OGLModel( const std::string& path ) : loaded( false )
{
  const aiScene* scene = aiImportFile( path.c_str(), 0 );
  if( scene == 0 )
  {
    return; 
  }

  aiMesh* mesh = scene->mMeshes[0];

  std::vector<float> verts;
  std::vector<float> normals;
  std::vector<float> uvs;
  std::vector<unsigned short> indices;

  for( unsigned int i = 0; i < mesh->mNumVertices; ++i )
  {
    aiVector3D& vert = mesh->mVertices[i];
    verts.push_back(vert.x);
    verts.push_back(vert.y);
    verts.push_back(vert.z);

    aiVector3D& normal = mesh->mNormals[i];
    normals.push_back(normal.x);
    normals.push_back(normal.y);
    normals.push_back(normal.z);

    aiVector3D& uv = mesh->mTextureCoords[0][i];
    uvs.push_back( uv.x );
    uvs.push_back( uv.y ); 
  }

  for( unsigned int i = 0; i < mesh->mNumFaces; ++i )
  {
    aiFace& f = mesh->mFaces[i];
    if( f.mNumIndices != 3 ) continue;
    for( unsigned int j = 0; j < f.mNumIndices; ++j )
    {
      indices.push_back(f.mIndices[j]);
    }
  }
  nIndices = indices.size();

  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  glGenBuffers(1,&vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER,verts.size()*sizeof(float),&verts[0],GL_STATIC_DRAW);

  glGenBuffers(1,&uvBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
  glBufferData(GL_ARRAY_BUFFER,uvs.size()*sizeof(float),&uvs[0],GL_STATIC_DRAW);

  glGenBuffers(1,&normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
  glBufferData(GL_ARRAY_BUFFER,normals.size()*sizeof(float),&normals[0],GL_STATIC_DRAW);

  glGenBuffers(1,&indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short),&indices[0],GL_STATIC_DRAW);

  loaded = true;
}

void OGLModel::bind()
{
  glBindVertexArray(vertexArrayObject);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
  glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
}

void OGLModel::draw()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_TRIANGLES,nIndices,GL_UNSIGNED_SHORT, (void*)0 );
//  glBindVertexArray(vertexArrayObject);
//  glDrawArrays(GL_TRIANGLES,0,3);
}

OGLModel::~OGLModel()
{
  glDeleteBuffers(1,&indexBuffer);
  glDeleteBuffers(1,&uvBuffer);
  glDeleteBuffers(1,&normalBuffer);
  glDeleteBuffers(1,&vertexBuffer);

  glDeleteVertexArrays(1,&vertexArrayObject);

  BOOST_LOG_TRIVIAL(debug) << "~OGLModel()";
}
