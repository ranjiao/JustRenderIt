#include "Common.h"
#include "Effect.h"
#include "Utils/Util.h"
#include "Utils/Logger.h"
#include "Graphics/Renderer.h"
#include "Graphics/OpenGL/OpenGLTexture.h"
#include "Cg/cgGL.h"
#include "Common/Resource.h"

using namespace JustRenderIt;

static CGcontext s_cgContext = NULL;

Effect::Effect(STRING filename)
{
  m_filename = filename;
  Create(filename);
}

Effect::Effect()
{
  m_filename = EMPTY_STRING;
}

CGcontext CGEffect::GetCGContext()
{
  if(!s_cgContext)
  {
    // init cg
    s_cgContext = cgCreateContext();
    if(g_renderer->GetRendererType() == Renderer::OPENGL)
    {
      // register opengl state for cg
      cgGLRegisterStates(s_cgContext);
    }

#ifdef _DEBUG
    cgGLSetDebugMode( CG_TRUE );
#else
    cgGLSetDebugMode( CG_FALSE );
#endif

  }
  return s_cgContext;
}


void CGEffect::DestroyCGContext()
{
  if(!s_cgContext)
  {
    cgDestroyContext(s_cgContext);
    CheckCGError("Destroying CG Context");
    s_cgContext = NULL;
  }
}

bool CGEffect::CheckCGError(STRING context)
{
  // check for warning
  const char *warning = cgGetLastListing(GetCGContext());
  if(warning)
  {
    LOG_WARNING3("%s : %s", context.c_str(), warning);
  }

  // check for error
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR)
  {
    if(context.empty())
      LOG_ERROR1(string);
    else
      LOG_ERROR3("%s : %s", context.c_str(), string);

    if (error == CG_COMPILER_ERROR)
    {
      const char* msg = cgGetLastListing(GetCGContext());
      LOG_ERROR2("CG Shader Compiler Error: \n%s", msg);
    }
    //assert(false);
    return false;
  }

  return true;
}

void JustRenderIt::Effect::Create( STRING filename /*= EMPTY_STRING*/ )
{
  if(filename.empty())
  {
    if(m_filename.empty())
    {
      LOG_ERROR1("Trying to create an effect without filename");
      return;
    }
    filename = m_filename;
  }
  else
    m_filename = filename;

  LOG_CONTENT2("Loading effect %s.", filename.c_str());

  internalCreate();
}

void JustRenderIt::Effect::Destroy()
{
   if(!m_isOK)
     return;

  LOG_CONTENT2("Destroying effect %s", GetFilename().c_str());
  internalDestroy();
}

void JustRenderIt::CGEffect::internalCreate()
{
  assert(!m_filename.empty());

  STRING filename = m_filename;
  if(!ResourceManager::GetResourcePath(filename, "Shaders"))
  {
    m_isOK = false;
    return;
  }

  // Load effect
  m_effect = cgCreateEffectFromFile(GetCGContext(), filename.c_str(), NULL);
  CheckCGError(filename);
  if(!m_effect)
  {
    LOG_ERROR2("Failed to create effect %s", m_filename.c_str());
    // TODO
    //m_isOK = false;
    return;
  }

  CheckCGError("Loading Effect");

  // Technique validation
  CGtechnique tech = cgGetFirstTechnique(m_effect);
  while(tech)
  {
    if(cgValidateTechnique(tech) == CG_FALSE)
    {
      LOG_ERROR2("Failed to validate technique \"%s\"",
        cgGetTechniqueName(tech));
    }
    tech = cgGetNextTechnique(tech);
  }
  // TODO
  //m_isOK = true;
}

void JustRenderIt::CGEffect::internalDestroy()
{
  assert(!m_filename.empty());

  cgDestroyEffect(m_effect);
}

int JustRenderIt::CGEffect::GetTechniqueCount()
{
  assert(false);
  return 0;
}

int JustRenderIt::CGEffect::GetPassCount()
{
  assert(false);
  return 0;
}

bool JustRenderIt::CGEffect::SelectTechnique( STRING techName )
{
  CGtechnique tech = cgGetNamedTechnique(m_effect, techName.c_str());
  if(!tech)
    return false;

  m_crtTech = tech;
  return true;
}

bool JustRenderIt::CGEffect::SelectPass( STRING passName )
{
  if(passName.empty())
  {
    cgSetPassState(NULL);
    return true;
  }

  CGpass pass = cgGetNamedPass(m_crtTech, passName.c_str());
  if(!pass)
    return false;

  cgSetPassState(pass);
  return true;
}

JustRenderIt::CGEffect::CGEffect( STRING filename ) :Effect(filename)
{
  m_effect = NULL;
  m_crtTech = NULL;
}

JustRenderIt::CGEffect::CGEffect() :Effect()
{
  m_effect = NULL;
  m_crtTech = NULL;
}

void JustRenderIt::CGEffect::SetShaderParam( STRING name, const int val )
{
  cgSetParameter1i(GetParameter(name), val);
}

void JustRenderIt::CGEffect::SetShaderParam( STRING name, const float val )
{
  cgSetParameter1f(GetParameter(name), val);
}

void JustRenderIt::CGEffect::SetShaderParam( STRING name, glm::vec2 val )
{
  cgSetParameter2f(GetParameter(name), val.x, val.y);
}

void JustRenderIt::CGEffect::SetShaderParam( STRING name, glm::vec3 val )
{
  cgSetParameter3f(GetParameter(name), val.x, val.y, val.z);
}

void JustRenderIt::CGEffect::SetShaderParam( STRING name, glm::vec4 val )
{
  cgSetParameter4f(GetParameter(name), val.x, val.y, val.z, val.w);
}

void JustRenderIt::CGEffect::SetShaderParam( STRING name, glm::mat4 val )
{
   glm::mat4 m = glm::transpose(val);
   cgSetMatrixParameterfr(GetParameter(name), &m[0][0]);
}

void JustRenderIt::CGEffect::SetShaderParam( STRING name, Texture* image )
{
  if(g_renderer->GetRendererType() == Renderer::OPENGL)
  {
    OpenGLTexture* texture = static_cast<OpenGLTexture*>(image);
    assert(texture);

    CGparameter texParam = GetParameter(name);
    cgGLSetTextureParameter(texParam, texture->GetTextureID());
  }
  else
  {
    assert(false);    // TODO
  }
}

CGparameter JustRenderIt::CGEffect::GetParameter(STRING name)
{
  ShaderVarMapIter iter = m_params.find(name);
  if(iter != m_params.end())
    return iter->second;

  CGparameter param = cgGetNamedEffectParameter(m_effect, name.c_str());

  m_params[name] = param;

#ifdef _DEBUG
  CheckError(name);
#endif

  return param;
}
