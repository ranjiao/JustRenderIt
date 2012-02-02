#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "Cg/cg.h"

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace JustRenderIt
{
  class Texture;

  class DLL_DECLARE Effect
  {
  public:
    Effect(STRING filename){ m_filename = filename; };
    Effect(){ m_filename = EMPTY_STRING; };

    virtual void Create(STRING filename = EMPTY_STRING);
    virtual void Destroy();

    STRING GetFilename(){ return m_filename; };

    virtual int GetTechniqueCount() = 0;
    virtual int GetPassCount() = 0;
    virtual bool SelectTechnique(STRING tch) = 0;
    virtual bool SelectPass(STRING pass = EMPTY_STRING) = 0;
    virtual bool CheckError(STRING context = EMPTY_STRING) = 0;

    // parameters
    virtual void SetShaderParam(STRING name, const int val) = 0;
    virtual void SetShaderParam(STRING name, const float val) = 0;
    virtual void SetShaderParam(STRING name, glm::vec2 val) = 0;
    virtual void SetShaderParam(STRING name, glm::vec3 val) = 0;
    virtual void SetShaderParam(STRING name, glm::vec4 val) = 0;
    virtual void SetShaderParam(STRING name, glm::mat4 val) = 0;
    virtual void SetShaderParam(STRING name, Texture* image) = 0;
  protected:
    STRING m_filename;
  };

  class DLL_DECLARE CGEffect: public Effect
  {
  public:
    CGEffect(STRING filename);
    CGEffect();

    virtual int GetTechniqueCount();
    virtual int GetPassCount();
    virtual bool SelectTechnique(STRING tech);
    virtual bool SelectPass(STRING pass = EMPTY_STRING);
    virtual bool CheckError(STRING context = EMPTY_STRING)
    {
      return CheckCGError(context);
    };

    // parameters
    virtual void SetShaderParam(STRING name, const int val);
    virtual void SetShaderParam(STRING name, const float val) ;
    virtual void SetShaderParam(STRING name, glm::vec2 val);
    virtual void SetShaderParam(STRING name, glm::vec3 val);
    virtual void SetShaderParam(STRING name, glm::vec4 val);
    virtual void SetShaderParam(STRING name, glm::mat4 val);
    virtual void SetShaderParam(STRING name, Texture* image);

    // CG functions
    static CGcontext GetCGContext();
    static void DestroyCGContext();
    static bool CheckCGError(STRING context = EMPTY_STRING);
  protected:
    CGparameter GetParameter(STRING name);

    virtual void internalCreate();
    virtual void internalDestroy();
  protected:
    CGeffect m_effect;
    CGtechnique m_crtTech;

    typedef std::map<STRING, CGparameter> ShaderVarMap;
    typedef ShaderVarMap::iterator ShaderVarMapIter;

    ShaderVarMap m_params;
  };

};

#if _MSC_VER
#pragma warning(pop)
#endif

#endif
