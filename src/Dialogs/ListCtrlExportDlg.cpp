/* copyright (c) Oleander Software, Ltd.
   author: Blake Madden
   This program is free software; you can redistribute it and/or modify
   it under the terms of the BSD License.
*/

#include "ListCtrlExportDlg.h"

wxBEGIN_EVENT_TABLE(wxListCtrlExportDlg, wxDialogWithHelp)
    EVT_BUTTON(wxID_OK, wxListCtrlExportDlg::OnOK)
    EVT_CHECKBOX(ID_SELECTED_ROW_CHECKBOX, wxListCtrlExportDlg::OnExportSelectedRows)
wxEND_EVENT_TABLE()

void wxListCtrlExportDlg::OnExportSelectedRows([[maybe_unused]] wxCommandEvent& event)
    {
    TransferDataFromWindow();
    m_fromRowSpinCtrl->Enable(!IsExportingSelectedRows());
    m_toRowSpinCtrl->Enable(!IsExportingSelectedRows());
    m_fromRowLabel->Enable(!IsExportingSelectedRows());
    m_toRowLabel->Enable(!IsExportingSelectedRows());
    }

//-------------------------------------------------------------
void wxListCtrlExportDlg::OnOK([[maybe_unused]] wxCommandEvent& event)
    {
    Validate();
    TransferDataFromWindow();

    m_options.m_fromRow = m_fromRowSpinCtrl->GetValue();
    m_options.m_toRow = m_toRowSpinCtrl->GetValue();
    m_options.m_fromColumn = m_fromColumnSpinCtrl->GetValue();
    m_options.m_toColumn = m_toColumnSpinCtrl->GetValue();

    if (IsModal())
        { EndModal(wxID_OK); }
    else
        { Show(false); }
    }

/// Creates the controls and sizers
//-------------------------------------------------------------
void wxListCtrlExportDlg::CreateControls()
    {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(itemBoxSizer3, 0, wxALIGN_TOP|wxALL, wxSizerFlags::GetDefaultBorder());

    wxCheckBox* itemCheckBox4 = new wxCheckBox(this, wxID_ANY, _("&Include column headers"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE, wxGenericValidator(&m_options.m_includeColumnHeaders));
    itemBoxSizer3->Add(itemCheckBox4, 0, wxALIGN_LEFT|wxALL, wxSizerFlags::GetDefaultBorder());

    wxCheckBox* exportSelectedRowsCheckBox = new wxCheckBox(this, ID_SELECTED_ROW_CHECKBOX, _("&Only export selected rows"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE, wxGenericValidator(&m_options.m_exportSelected));
    itemBoxSizer3->Add(exportSelectedRowsCheckBox, 0, wxALIGN_LEFT|wxALL, wxSizerFlags::GetDefaultBorder());

    if (m_exportFormat == wxListExportFormat::wxExportHtml)
        {
        wxCheckBox* pageBreaksCheckBox = new wxCheckBox(this, wxID_ANY, _("Insert &page breaks"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE, wxGenericValidator(&m_options.m_includePageBreaks));
        itemBoxSizer3->Add(pageBreaksCheckBox, 0, wxALIGN_LEFT|wxALL, wxSizerFlags::GetDefaultBorder());
        }

    m_rangeStaticBox = new wxStaticBox(this, wxID_ANY, _("Range:"));
    wxStaticBoxSizer* itemStaticBoxSizer5 = new wxStaticBoxSizer(m_rangeStaticBox, wxHORIZONTAL);
    itemBoxSizer3->Add(itemStaticBoxSizer5, 0, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    wxFlexGridSizer* rangeOptionsSizer = new wxFlexGridSizer(2, 4, 5, 5);
    itemStaticBoxSizer5->Add(rangeOptionsSizer);

    //row options
    m_fromRowLabel = new wxStaticText( this, wxID_STATIC, _("Rows: from"));
    rangeOptionsSizer->Add(m_fromRowLabel, 0, wxALIGN_CENTER_VERTICAL);
    
    m_fromRowSpinCtrl = new wxSpinCtrl(this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(FromDIP(wxSize(100,100)).GetWidth(),-1), wxSP_ARROW_KEYS, 1, m_options.m_toRow, 1);
    rangeOptionsSizer->Add(m_fromRowSpinCtrl);

    m_toRowLabel = new wxStaticText( this, wxID_STATIC, _("to"));
    rangeOptionsSizer->Add(m_toRowLabel, 0, wxALIGN_CENTER_VERTICAL);

    m_toRowSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(FromDIP(wxSize(100,100)).GetWidth(),-1), wxSP_ARROW_KEYS, 1, m_options.m_toRow, 1 );
    m_toRowSpinCtrl->SetValue(m_options.m_toRow);
    rangeOptionsSizer->Add(m_toRowSpinCtrl);

    //column options
    m_fromColumnLabel = new wxStaticText( this, wxID_STATIC, _("Columns: from"));
    rangeOptionsSizer->Add(m_fromColumnLabel, 0, wxALIGN_CENTER_VERTICAL);

    m_fromColumnSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(FromDIP(wxSize(100,100)).GetWidth(),-1), wxSP_ARROW_KEYS, 1, m_options.m_toColumn, 1);
    rangeOptionsSizer->Add(m_fromColumnSpinCtrl, 0, wxALIGN_CENTER_HORIZONTAL);

    m_toColumnLabel = new wxStaticText( this, wxID_STATIC, _("to"));
    rangeOptionsSizer->Add(m_toColumnLabel, 0, wxALIGN_CENTER_VERTICAL);

    m_toColumnSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(FromDIP(wxSize(100,100)).GetWidth(),-1), wxSP_ARROW_KEYS, 1, m_options.m_toColumn, 1 );
    m_toColumnSpinCtrl->SetValue(m_options.m_toColumn);
    rangeOptionsSizer->Add(m_toColumnSpinCtrl);

    mainSizer->Add(CreateSeparatedButtonSizer(wxOK|wxCANCEL|wxHELP), 0, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    SetSizerAndFit(mainSizer);
    }
