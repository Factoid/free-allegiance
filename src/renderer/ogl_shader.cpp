#include "ogl_common.hpp"
#include "ogl_shader.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void getFile( const std::string& path, std::string& contents )
{
  std::ifstream file(path.c_str());
  file.seekg(0, std::ios_base::end);
  contents.resize(file.tellg());
  file.seekg(0, std::ios_base::beg);
  file.read(&contents[0],contents.size());
}

OGLShader::OGLShader( const std::string& path )
{
  std::string src;
  vertexID = glCreateShader(GL_VERTEX_SHADER);
  getFile( path+".vertex", src );
  const char* c_src = src.c_str();
  BOOST_LOG_TRIVIAL(debug) << "Vertex Shader Source:\n" << src;
  glShaderSource(vertexID, 1, &c_src, 0);
  glCompileShader(vertexID);
  
  fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
  getFile( path+".fragment", src );
  c_src = src.c_str();
  BOOST_LOG_TRIVIAL(debug) << "Fragment Shader Source:\n" << src;
  glShaderSource(fragmentID, 1, &c_src, 0);
  glCompileShader(fragmentID);

  programID = glCreateProgram();
  glAttachShader(programID,vertexID);
  glAttachShader(programID,fragmentID);
  glLinkProgram(programID);
}

glm::mat4 model = glm::scale( glm::mat4(1.0f), glm::vec3( 4.0f,4.0f,4.0f ) );

void OGLShader::bind()
{
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glEnable (GL_CULL_FACE);
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  glClearColor( 0.0f, 0.0f, 0.4f, 0.0f );
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(programID);

  GLuint matID = glGetUniformLocation(programID, "MVP");
  GLuint worldID = glGetUniformLocation(programID, "M" );
  GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

  glm::mat4 projection = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 1000.0f );
  glm::mat4 view = glm::lookAt( glm::vec3(5,3,20), glm::vec3(0,0,0), glm::vec3(0,1,0) );
  glm::mat4 MVP = projection * view * model;
  
  MVP = projection * view * model;
  model = glm::rotate( model, 1.0f, glm::vec3(0.5f,0.5f,0.3f));

  glUniformMatrix4fv(matID, 1, GL_FALSE, &MVP[0][0] );
  glUniformMatrix4fv(worldID, 1, GL_FALSE, &model[0][0] );
  glUniform1i(TextureID, 0);
}

OGLShader::~OGLShader()
{
  glDeleteProgram(programID);
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
  BOOST_LOG_TRIVIAL(debug) << "~OGLShader()";
}
