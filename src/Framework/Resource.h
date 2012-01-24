#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <unordered_map>
#include "Common.h"
#include "Utils/Singleton.h"

namespace JustRenderIt
{
  class ResourceManager;
  class BaseResourceLoader;
  DLL_DECLARE extern ResourceManager* g_resourceMgr;

  /// Handle
  class DLL_DECLARE Handle
  {
    enum
    {
      // sizes to use for bit fields
      MAX_BITS_INDEX = 16,
      MAX_BITS_MAGIC = 12,
      MAX_BITS_TAG = 4,

      // sizes to compare against for asserting dereferences
      MAX_INDEX = ( 1 << MAX_BITS_INDEX ) - 1,
      MAX_MAGIC = ( 1 << MAX_BITS_MAGIC ) - 1,
      MAX_TAG = ( 1 << MAX_BITS_TAG ) -1
    };

    union
    {
      struct
      {
        unsigned m_index : MAX_BITS_INDEX;  // index into resource array
        unsigned m_magic : MAX_BITS_MAGIC;  // magic number to check
        unsigned m_tag : MAX_BITS_TAG;
      };
      unsigned int m_handle;
    };

  public:
    // Lifetime.
    Handle( void ) : m_handle( 0 )  {};
    Handle(int handle) : m_handle(handle) {};

    void Init( unsigned int index, unsigned char tag );

    // Query.
    unsigned int GetIndex ( void ) const  {  return (  m_index  );  }
    unsigned int GetMagic ( void ) const  {  return (  m_magic  );  }
    unsigned int GetHandle( void ) const  {  return (  m_handle );  }
    unsigned int GetTag   ( void ) const  {  return (  m_tag ); }
    bool         IsNull   ( void ) const  {  return ( !m_handle );  }

    operator unsigned int ( void ) const  {  return (  m_handle );  }
  };

  inline bool operator != ( Handle l, Handle r )
  {  return ( l.GetHandle() != r.GetHandle() );  }


  inline bool operator == ( Handle l, Handle r )
  {  return ( l.GetHandle() == r.GetHandle() );  }


  /** Base class which  of the resource
   **/
  class DLL_DECLARE BaseResource
  {
  protected:
    bool m_dirty;
    STRING m_filename;
  public:
    BaseResource()
    {
      m_dirty = false;
      m_filename = EMPTY_STRING;
    };
    virtual ~BaseResource(){};

    virtual STRING GetFilename()
    {
      return m_filename;
    };
    virtual bool IsDirty(){ return m_dirty; };

    virtual char GetHash() = 0;
  };

  typedef HashMap<int, BaseResource*> HashHandle;
  typedef HashMap<STRING, int> HashFilename;
  typedef HashHandle::iterator IterResource;
  typedef HashFilename::iterator IterFilename;

  /* TODO: If these two hash table is put in ResourceManager and exported, 
  they will cause runtime exception in visual studio. Is there any better
  way ? */
  extern HashHandle g_handleResTable;
  extern HashFilename g_nameHandleTable;

  class DLL_DECLARE ResourceManager: public Singleton<ResourceManager>
  {
  public:
    ResourceManager();
    virtual ~ResourceManager();

    Handle NewResource(BaseResource* r, char tag);

    char RegisterResourceLoader(BaseResourceLoader* const loader);

    BaseResource* GetResource(Handle h);;

    DECL_SINGLETON(ResourceManager);
  };

  class DLL_DECLARE BaseResourceLoader
  {
  protected:
    /// subclass must rewrite this function
    virtual BaseResource* doLoadFromFile(STRING filename){ return NULL; };
  public:
    virtual Handle LoadFromFile(STRING filename)
    {
      BaseResource* resource = doLoadFromFile(filename);
      return g_resourceMgr->NewResource(resource, GetTag());
    };

    virtual void UnLoad(BaseResource* r) = 0;
    virtual char GetTag() = 0;
    virtual STRING GetTagName() = 0;
  };

};

#endif /* __RESOURCE_H__ */
