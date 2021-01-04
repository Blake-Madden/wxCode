/* copyright (c) Oleander Software, Ltd.
   author: Blake Madden
   This program is free software; you can redistribute it and/or modify
   it under the terms of the BSD License.
*/

#include "ArchiveDlg.h"

wxBEGIN_EVENT_TABLE(wxArchiveDlg, wxDialogWithHelp)
    EVT_BUTTON(wxArchiveDlg::ID_FILE_BROWSE_BUTTON, wxArchiveDlg::OnFileButtonClick)
    EVT_BUTTON(wxID_OK, wxArchiveDlg::OnOK)
wxEND_EVENT_TABLE()

//-------------------------------------------------------------
void wxArchiveDlg::OnOK([[maybe_unused]] wxCommandEvent& event)
    {
    TransferDataFromWindow();

    if (m_filePath.IsEmpty() || !wxFileName::Exists(m_filePath))
        {
        wxMessageBox(_("Please select a valid archive file."), _("Invalid File"),
                     wxICON_EXCLAMATION|wxOK, this);
        return;
        }

    if (IsModal())
        { EndModal(wxID_OK); }
    else
        { Show(false); }
    }

//-------------------------------------------------------------
void wxArchiveDlg::OnFileButtonClick([[maybe_unused]] wxCommandEvent& event)  
    {
    TransferDataFromWindow();
    wxFileDialog dialog
            (
            this,
            _("Select Archive File"),
            wxEmptyString,
            wxEmptyString,
            _("Archive files (*.zip)|*.zip"),
            wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_PREVIEW|wxFD_MULTIPLE);

    if (dialog.ShowModal() != wxID_OK)
        { return; }
    m_filePath = dialog.GetPath();
    TransferDataToWindow();
    SetFocus();
    }

//-------------------------------------------------------------
void wxArchiveDlg::CreateControls()
    {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* fileBrowseBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(fileBrowseBoxSizer, 0, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    wxTextCtrl* filePathEdit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RICH2|wxBORDER_THEME, wxGenericValidator(&m_filePath) );
    filePathEdit->AutoCompleteFileNames();
    fileBrowseBoxSizer->Add(filePathEdit, 1, wxEXPAND);

    wxBitmapButton* fileBrowseButton = new wxBitmapButton(this, ID_FILE_BROWSE_BUTTON, wxArtProvider::GetBitmap(wxART_FILE_OPEN,wxART_BUTTON, FromDIP(wxSize(16,16))));
    fileBrowseBoxSizer->Add(fileBrowseButton, 0, wxALIGN_CENTER_VERTICAL);

    wxBoxSizer* fileTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(fileTypeSizer, 0, wxEXPAND);
    fileTypeSizer->Add(new wxStaticText(this, wxID_STATIC, _("File types to include:")), 0, wxALIGN_CENTER_VERTICAL|wxLEFT, wxSizerFlags::GetDefaultBorder());
    wxArrayString choiceStrings;
    wxStringTokenizer tkz(m_fullFileFilter, wxT("|"), wxTOKEN_STRTOK);
    while (tkz.HasMoreTokens() )
        {
        wxString currentFilter = tkz.GetNextToken();
        if (currentFilter.Length() && currentFilter[0] != wxT('*'))
            { choiceStrings.Add(currentFilter); }
        }
    m_fileFilterCombo = new wxChoice(this, wxID_ANY, wxDefaultPosition,
        // need to hard code size in case file filter is too wide
        wxSize(FromDIP(wxSize(150,150)).GetWidth(),-1),
        choiceStrings,
        0, wxGenericValidator(&m_selectedFileFilter));
    fileTypeSizer->Add(m_fileFilterCombo, 1, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    mainSizer->AddStretchSpacer();

    mainSizer->Add(CreateSeparatedButtonSizer(wxOK|wxCANCEL|wxHELP), 0, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    SetSizerAndFit(mainSizer);

    filePathEdit->SetFocus();
    }
