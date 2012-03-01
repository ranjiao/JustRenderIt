#ifndef __CVAR_H__
#define __CVAR_H__

#include <unordered_map>
#include "Common.h"
#include "Singleton.h"
#include "Util.h"

namespace JustRenderIt
{
  class DLL_DECLARE CVar: public BaseObject
  {
  public:
    typedef void (*CVarChangeCB) (CVar newValue, CVar oldValue);
  protected:
    STRING m_name, m_desc, m_value;
    CVarChangeCB m_changeCB;
  public:
    CVar()
    {
      m_name = m_desc = m_value = EMPTY_STRING;
      m_changeCB = NULL;
    };

    CVar(STRING name, STRING desc, CVarChangeCB cb)
    {
      m_name = name;
      m_desc = desc;
      m_changeCB = cb;
    };

    bool IsOK(){ return m_value == EMPTY_STRING; };

    STRING GetName(){ return m_name; };
    STRING GetDesc(){ return m_desc; };
    CVarChangeCB GetCallback(){ return m_changeCB; };
    void SetCallback(CVarChangeCB cb){ m_changeCB = cb; };

    void SetValue(int v){ m_value = ToString(v); };
    void SetValue(float f){ m_value = ToString(f); };
    void SetValue(STRING s){ m_value = ToString(s); };
    int GetValuei(){ return ToInt(m_value); };
    float GetValuef(){ return ToFloat(m_value); };
    STRING GetValues(){ return m_value; };

    STRING Serialize()
    {
      return m_name + " = " + m_value;
    }
  };

  class DLL_DECLARE CVarManager: public Singleton<CVarManager>,
                               public BaseObject
  {
  public:
    CVarManager();
    ~CVarManager();

    /// delete all existing cvars
    void Reset();

    /// Load a cvar file
    bool LoadFromFile(STRING filename);

    /// Load cvar string from memory
    bool LoadFromMemory(const char* data, unsigned length);

    /// Get a CVar
    CVar Get(STRING name);

    /// Set a CVar
    void Set(CVar v);

  protected:
    typedef HashMap<STRING, CVar> HashCVar;
    typedef HashCVar::iterator IterCVar;

    HashCVar m_cvars;

    DECL_SINGLETON(CVarManager);
  };
};

#endif /* __CVAR_H__ */
