#include "Common.h"
#include "Logger.h"
#include <stdarg.h>

using namespace JustRenderIt;

IMPL_SINGLETON(Logger);

void Logger::AddPolicy(STRING filename, LogPolicy* policy)
{
  m_policies.push_back(policy);

  policy->Create(filename);
  m_isOK = m_isOK && policy->IsOK();
}

#define r_INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define rva_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )

void Logger::Write(LogLevel level, const char* format, ...)
{
  char buffer[BUFFER_LENGTH];
  va_list arglist;

  va_start(arglist, format);
  vsprintf(buffer, format, arglist);
  va_end(arglist);

  if(level < m_logLevel)
    return;

  for(IterPolicy iter = m_policies.begin();
    iter != m_policies.end();
    iter ++)
  {
    (*iter)->Write(buffer, level);
  }
}

void Logger::WriteBanner(const char* format, ...)
{
  char buffer[BUFFER_LENGTH];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(buffer, format, argptr);
  va_end(argptr);

  for(IterPolicy iter = m_policies.begin();
    iter != m_policies.end();
    iter ++)
  {
    (*iter)->WriteBanner(buffer);
  }
}

void Logger::Warning(const char* format, ...)
{
  char buffer[BUFFER_LENGTH];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(buffer, format, argptr);
  va_end(argptr);

  Write(LogHigher, "Warning: %s", buffer);
}

void Logger::Error(const char* format, ...)
{
  char buffer[BUFFER_LENGTH];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(buffer, format, argptr);
  va_end(argptr);

  Write(LogHigher, "Error: %s", buffer);
}

void Logger::Fatal(const char* format, ...)
{
  char buffer[BUFFER_LENGTH];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(buffer, format, argptr);
  va_end(argptr);

  Write(LogHigher, "Fatal: %s", buffer);
  assert(false);
}

void Logger::Destroy()
{
  internalDestroy();
}

void Logger::internalCreate()
{

}

void Logger::internalDestroy()
{
  for( IterPolicy iter = m_policies.begin();
    iter != m_policies.end();
    iter ++)
  {
    (*iter)->Destroy();
    delete (*iter);
  }
}

const char html_inc[] =
  "<head>"
  "<style>"
  "<!--"
  "BODY { color: #000000; font-family: 'Verdana', 'Arial', 'Helvetica', sans-serif; font-size: 10pt }"
  "P.header { font-family: 'Verdana', 'Arial', 'Helvetica', sans-serif; font-size: 12pt; font-weight: bold }"
  "P.mainheader { background: #6b7d6b; color: #ffffff; font-family: 'Verdana', 'Arial', 'Helvetica', sans-serif; font-size: 10pt; font-weight: bold }"
  "P.maincontent { font-family: 'Verdana', 'Arial', 'Helvetica', sans-serif; font-size: 10pt }"
  "BLOCKQUOTE { color: #000000; font-family:'Verdana', 'Arial', 'Helvetica', sans-serif; font-size: 10pt }"
  "A:link { color: #003300 }"
  "-->"
  "  </style>"
  "</head>\n";

const char* colors[LoggerHtml::LogColorCount] =
{
  "FF0000",
  "00FF00",
  "0000FF",
  "000000"
};

const char* mark[LoggerHtml::LogFormatCount] =
{
  "<title>%s</title>",                                // title
  "<p class=""header"" align=""center"">%s</p>",      // header
  "<p class=""mainheader"" align=""left"">%s</p>",    // subheader
  "<p class=""maincontent"" align=""left"">%s</p>",   // maincontent
  "<li>%s</li>"                                       // list
};

const char* cmark[LoggerHtml::LogFormatCount] =
{
  "<font color=""%s""> <title>%s</title> </font>",    // title
  "<font color=""%s""> <p class=""header"" align=""center"">%s</p> </font>",
  // header
  "<font color=""%s""> <p class=""mainheader"" align=""left"">%s</p> </font>",
  // subheader
  "<font color=""%s""> <p class=""maincontent"" align=""left"">%s</p> </font>",
  // maincontent
  "<font color=""%s""> <li>%s</li> </font>"         // list
};

LoggerHtml::LoggerHtml()
{
  m_file = NULL;
}

void LoggerHtml::Create(STRING filename)
{
  m_filename = filename;
  internalCreate();
}

void LoggerHtml::Destroy()
{
  internalDestroy();
}

void LoggerHtml::internalCreate()
{
  assert(!m_filename.empty());

  m_file = fopen(m_filename.c_str(), "w");
  m_isOK = (m_file != NULL);

  if(!m_isOK)
    return;

  fprintf(m_file, "<html>\n");
  fprintf(m_file, "%s", html_inc);
}

void LoggerHtml::internalDestroy()
{
  assert(m_isOK);
  assert(m_file);

  fprintf(m_file, "</html>\n");
  fclose(m_file);
}

void LoggerHtml::Write(STRING content, LogLevel level)
{
  WriteFormat(content, level, LogContent);
}

void LoggerHtml::WriteFormat(STRING content, LogLevel level,
                             LoggerFormat format)
{
  fprintf(m_file, mark[format], content.c_str());
  fflush(m_file);
}

void LoggerHtml::WriteBanner(STRING content)
{
  WriteFormat(content, LogHigher, LogSubheader);
}

LoggerPlainText::LoggerPlainText()
{
  m_file = NULL;
}

void LoggerPlainText::Create(STRING filename)
{
  m_filename = filename;

  internalCreate();
}

void LoggerPlainText::Destroy()
{
  internalDestroy();
}

void LoggerPlainText::Write(STRING content, LogLevel level)
{
  assert(m_file);

  fprintf(m_file, "%s\n", content.c_str());
}

void LoggerPlainText::WriteBanner(STRING content)
{
  assert(m_file);

  const int screen_width = 70;
  int length = content.length();
  int start = ( 80 - length ) / 2;
  int end = start + length;
  for(int i=0; i<start; i++)
    fprintf(m_file, "=");

  fprintf(m_file, "%s", content.c_str());

  for(int i=end-1; i<screen_width; i++)
    fprintf(m_file, "=");

  fprintf(m_file, "\n");
}

void LoggerPlainText::internalCreate()
{
  m_file = fopen(m_filename.c_str(), "w");
  if(m_file)
    m_isOK = true;
}

void LoggerPlainText::internalDestroy()
{
  fclose(m_file);
}
