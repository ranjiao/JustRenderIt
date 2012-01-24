#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "Common.h"

namespace JustRenderIt
{
  template<typename T>
  class DLL_DECLARE Singleton
  {
  private:
    static T* __instance;
    static bool __created_by_singleton;
  public:
    Singleton()
    {
      if( !__created_by_singleton )
        throw JRIException("Singleton can only be created once");
    };

    virtual ~Singleton()
    {
      __created_by_singleton = false;
    };

    static T* _instance()
    {
      if( !__instance )
      {
        __created_by_singleton = true;
        try
        {
          __instance = new T;
        }
        catch (...)
        {
          __created_by_singleton = false;
        }
        __created_by_singleton = false;
      }
      return __instance;
    }

    static T& _instanceRef()
    {
      return (*_instance());
    }

  };

#define DECL_SINGLETON(klass)                   \
  public:                                       \
  static klass* Instance();                     \
  static klass& InstanceRef();

#define DECL_SINGLETON_DLL(klass)               \
  public:                                       \
  DLL_DECLARE static klass* Instance();         \
  DLL_DECLARE static klass& InstanceRef();

#if defined WIN32
#define IMPL_SINGLETON(klass)                                           \
  klass* Singleton<klass>::__instance = 0;                              \
  bool Singleton<klass>::__created_by_singleton = false;                \
  klass* klass::Instance(){ return Singleton<klass>::_instance(); };    \
  klass& klass::InstanceRef(){ return Singleton<klass>::_instanceRef(); };
#elif defined LINUX
#define IMPL_SINGLETON(klass)                                           \
  template<class klass> klass* Singleton<klass>::__instance = 0;        \
  template<class klass> bool Singleton<klass>::__created_by_singleton = false; \
  klass* klass::Instance(){ return Singleton<klass>::_instance(); };    \
  klass& klass::InstanceRef(){ return Singleton<klass>::_instanceRef(); };
#endif

};

#endif /* __SINGLETON_H__ */
