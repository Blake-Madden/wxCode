#include "LogFile.h"

wxLogFile::wxLogFile()
    {
    m_logFilePath = wxStandardPaths::Get().GetTempDir() + wxFileName::GetPathSeparator() +
        wxTheApp->GetAppName() + wxDateTime::Now().FormatISODate() + L".log";
    wxFile logFile;
    if (!logFile.Create(m_logFilePath, true))
        {
        wxMessageBox(wxString::Format(_("Unable to create log file at '%s'"), m_logFilePath),
            _("Logging Error"), wxOK|wxICON_WARNING);
        }
    else
        {
        // clear file (from a previous program run) and prepare for appending
        logFile.Write(wxEmptyString);
        }
    }

wxString wxLogFile::ReadLog()
    {
    wxLogFile::Flush();

    wxString logBuffer;
    wxFile logFile(m_logFilePath, wxFile::read);
    if (logFile.IsOpened())
        { logFile.ReadAll(&logBuffer); }
    // flushing to temp file failed somehow,
    // so return whatever is queued up at least
    else
        { return m_buffer; }
    return logBuffer;
    }

void wxLogFile::Flush()
    {
    wxLog::Flush();
    if (m_buffer.length())
        {
        wxFile logFile(m_logFilePath, wxFile::write_append);
        if (logFile.IsOpened())
            {
            logFile.Write(m_buffer);
            m_buffer.clear();
            }
        }
    }

void wxLogFile::DoLogTextAtLevel(wxLogLevel level, const wxString &msg)
    {
    wxString prefix;
    switch (level)
        {
        case wxLOG_Debug:
            [[fallthrough]];
        case wxLOG_Trace:
            prefix = L"Debug: ";
            break;
        case wxLOG_FatalError: 
            [[fallthrough]];
        case wxLOG_Error:
            prefix = L"Error: ";
            break;
        case wxLOG_Warning:
            prefix = L"Warning: ";
        }
    m_buffer += (prefix + msg);
    }

void wxLogFile::DoLogRecord(wxLogLevel level, const wxString &msg, const wxLogRecordInfo &info)
    {
    wxString prefix;
    switch (level)
        {
        case wxLOG_Debug:
            [[fallthrough]];
        case wxLOG_Trace:
            prefix = L"Debug: ";
            break;
        case wxLOG_FatalError: 
            [[fallthrough]];
        case wxLOG_Error:
            prefix = L"Error: ";
            break;
        case wxLOG_Warning:
            prefix = L"Warning: ";
        }
    m_buffer += wxString::Format(L"%s%s\t%s\t%s\t%s: line %d\n",
        prefix, msg,
        wxDateTime(info.timestamp).FormatISOCombined(' '),
        (info.func ? wxString(info.func) : L"NA"),
        (info.filename ? wxFileName(info.filename).GetFullName() : L"NA"),
        info.line);
    }
