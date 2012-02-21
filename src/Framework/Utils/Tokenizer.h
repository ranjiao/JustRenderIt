#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "Utils/Util.h"

namespace JustRenderIt
{
  class DLL_DECLARE Tokenizer
  {
  protected:
    char *m_buffer, *m_crt, *m_next;
    char m_delim[32], m_comment[32];

    unsigned m_length;
  public:
    Tokenizer()
    {
      strcpy(m_delim, " \t\n\r");
      strcpy(m_comment, "#");
      m_buffer = NULL;
      m_length = -1;
    };

    ~Tokenizer()
    {
      SAFE_DELETE_ARRAY(m_buffer);
    }

    void SetDelim(char* delim)
    {
      unsigned length = max(size_t(31), strlen(delim));
      memcpy(m_delim, delim, length);
      m_delim[32] = 0;
    }

    void SetComment(char* comment)
    {
      unsigned length = max(size_t(31), strlen(comment));
      memcpy(m_comment, comment, length);
      m_comment[32] = 0;
    }

    char* GetBuffer(){ return m_buffer; };
    unsigned GetLength(){ return m_length; };

    bool LoadFromFile(STRING filename)
    {
      if(filename.empty())
        return false;
      if(!FileExists(filename))
        return false;

      FILE* infile = fopen(filename.c_str(), "rb");
      if(!infile)
      {
        assert(false);
        return false;
      }

      SAFE_DELETE(m_buffer);

      unsigned length = FileLength(filename);
      printf("file length: %d\n", length);
      m_buffer = new char[length+1];
      m_buffer[length] = '0';
      fread(m_buffer, 1, length, infile);


      m_length = length;
      m_next = m_crt =  NULL;
      return true;
    }

    bool LoadFromMemory(const char* data, unsigned length = 0)
    {
      if(!data)
        return false;

      SAFE_DELETE(m_buffer);
      unsigned newLength = length == 0 ? strlen(data) : length;
      m_buffer = new char[newLength+1];
      m_length = newLength;

      memcpy(m_buffer, data, newLength);

      m_buffer[newLength] = 0;

      m_crt = m_next = NULL;

      return true;
    }

    STRING CrtToken()
    {
      STRING result;
      char buffer[128]; // TOOD: avoid overflow
      char* ptr;

      assert(m_next > m_crt);

      int index = 0;
      for(ptr = m_crt ; ptr != m_next && index < 128; ptr++ && index++)
        buffer[index] = *ptr;

      buffer[index] = 0;
      return STRING(buffer);
    }

    void Next()
    {
      // initialize the pointers
      if(m_crt == m_next && m_crt == NULL)
      {
        m_crt = m_buffer;
        SkipDelim(&m_crt);

        m_next = m_crt;
        SkipNoDelim(&m_next);
        return;
      }

      assert(m_crt && m_next);
      SkipDelim(&m_next);
      m_crt = m_next;
      SkipNoDelim(&m_next);
      return;
    }

    unsigned SkipLine(char** ptr)
    {
      assert(ptr);
      char* buffer = *ptr;
      unsigned traveled = 0;

      // Skip curent line
      for( ; *buffer != 0 && *buffer !='\n'; buffer++ && traveled++) ;

      // Skip \n
      buffer++;
      traveled++;

      *ptr = buffer;
      return traveled;
    }

    unsigned SkipNoDelim(char** ptr)
    {
      assert(ptr);

      unsigned traveled = 0;
      int delimCount = strlen(m_delim);
      int commCount = strlen(m_comment);
      char* buffer = *ptr;

      // jump to next line if we hit comment at first
      for(int i=0; i<commCount; i++)
        if(*buffer == m_comment[i])
        {
          traveled += SkipLine(&buffer);
        }

      bool isNotDelim = true, isComment = false;
      for( ; *buffer != 0 && isNotDelim && !isComment; buffer++ && traveled++)
      {
        isNotDelim = true;
        for(int i=0; i<commCount; i++)
          if(*buffer == m_comment[i])
          {
            isComment = true; // stop going if we meet a comment
          }

        for(int i=0; i<delimCount; i++)
          if(*buffer == m_delim[i])
          {
            isNotDelim = false;
            break;
          }
      }

      if(traveled)
      {
        buffer--;
        traveled--;
      }

      *ptr = buffer;
      return traveled;
    }

    unsigned SkipDelim(char** ptr)
    {
      assert(ptr);

      unsigned traveled = 0;
      int delimCount = strlen(m_delim);
      int commCount = strlen(m_comment);
      char* buffer = *ptr;

      // jump to next line if we hit comment at first
      for(int i=0; i<commCount; i++)
        if(*buffer == m_comment[i])
        {
          traveled += SkipLine(&buffer);
        }

      bool isDelim = true, isComment = false;
      for( ; *buffer != 0 && isDelim && !isComment; buffer++ && traveled++)
      {
        isDelim = false;
        for(int i=0; i<commCount; i++)
          if(*buffer == m_comment[i])
          {
            // conntinue to next line if we meed comments
            traveled += SkipLine(&buffer);
          }

        for(int i=0; i<delimCount; i++)
          if(*buffer == m_delim[i])
          {
            isDelim = true;
            break;
          }
      }

      if(traveled)
      {
        buffer--;
        traveled--;
      }

      *ptr = buffer;
      return traveled;
    }
  };
};

#endif /* __TOKENIZER_H__ */
