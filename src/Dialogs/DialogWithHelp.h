/** @addtogroup wxCode
    @brief A collection of wxWidget tools.
    @date 2005-2020
    @copyright Oleander Software, Ltd.
    @author Blake Madden
    @details This program is free software; you can redistribute it and/or modify
    it under the terms of the BSD License.
* @{*/

#ifndef __WXDIALOG_WITH_HELP_H__
#define __WXDIALOG_WITH_HELP_H__

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/filename.h>

/** @brief Dialog with built-in support for help events.

    A path to the dialog's HTML help topic can be specified.
*/
class wxDialogWithHelp : public wxDialog
    {
public:
    /** Constructor.
        @param parent the parent of the dialog.
        @param id the window ID for this dialog.
        @param caption the title of this dialog.*/
    wxDialogWithHelp(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = wxEmptyString)
        {
        SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);
        wxDialog::Create(parent, id, caption, wxDefaultPosition, wxDefaultSize,  wxDEFAULT_DIALOG_STYLE|wxCLIP_CHILDREN|wxRESIZE_BORDER);

        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &wxDialogWithHelp::OnHelpClicked, this, wxID_HELP);
        Bind(wxEVT_HELP, &wxDialogWithHelp::OnContextHelp, this);

        Centre();
        }
    wxDialogWithHelp()
        {
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &wxDialogWithHelp::OnHelpClicked, this, wxID_HELP);
        Bind(wxEVT_HELP, &wxDialogWithHelp::OnContextHelp, this);
        }
    wxDialogWithHelp(const wxDialogWithHelp&) = delete;
    wxDialogWithHelp(wxDialogWithHelp&&) = delete;
    wxDialogWithHelp& operator=(const wxDialogWithHelp&) = delete;
    wxDialogWithHelp& operator=(wxDialogWithHelp&&) = delete;
    /** Sets the help topic for the dialog.
        @param helpProjectDirectory The folder/base URL where the topics are stored.
        @param topicPath The path (after @c helpProjectDirectory) to the topic.*/
    void SetHelpTopic(const wxString& helpProjectDirectory, const wxString& topicPath)
        {
        m_helpProjectFolder = helpProjectDirectory;
        m_helpTopic = topicPath;
        }
private:
    void OnHelpClicked([[maybe_unused]] wxCommandEvent& event)
        {
        if (m_helpTopic.length())
            { wxLaunchDefaultBrowser(wxFileName::FileNameToURL(m_helpProjectFolder + wxFileName::GetPathSeparator() + m_helpTopic)); }
        }
    void OnContextHelp([[maybe_unused]] wxHelpEvent& event)
        {
        wxCommandEvent cmd;
        OnHelpClicked(cmd);
        }
    wxString m_helpProjectFolder;
    wxString m_helpTopic;
    };

/** @}*/

#endif //__WXDIALOG_WITH_HELP_H__
