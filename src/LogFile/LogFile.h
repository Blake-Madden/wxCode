/** @addtogroup wxCode
    @brief A collection of wxWidget tools.
    @date 2005-2021
    @copyright Oleander Software, Ltd.
    @author Blake Madden
    @details This program is free software; you can redistribute it and/or modify
    it under the terms of the BSD License.
* @{*/

#ifndef __WXLOGFILLE_H__
#define __WXLOGFILLE_H__

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/log.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

/** Logging system that writes its records to a temp file.*/
class wxLogFile : public wxLog
    {
public:
    wxLogFile();
    
    /// @returns The string contents with all messages logged.
    [[nodiscard]] wxString ReadLog();

    /// @returns The path of the log file.
    [[nodiscard]] const wxString& GetLogFilePath() const noexcept
        { return m_logFilePath; }

    void Flush() final;

protected:
    void DoLogText(const wxString &msg) final
        { m_buffer << msg << L"\n"; }
    void DoLogRecord(wxLogLevel level, const wxString &msg, const wxLogRecordInfo &info) final;
    void DoLogTextAtLevel(wxLogLevel level, const wxString &msg) final;
private:
    wxString m_buffer;
    wxString m_logFilePath;
    wxDECLARE_NO_COPY_CLASS(wxLogFile);
    };

/** @}*/

#endif //__WXLOGFILLE_H__
