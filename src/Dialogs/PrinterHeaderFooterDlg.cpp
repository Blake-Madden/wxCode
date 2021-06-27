/* copyright (c) Oleander Software, Ltd.
   author: Blake Madden
   This program is free software; you can redistribute it and/or modify
   it under the terms of the BSD License.
*/

#include "PrinterHeaderFooterDlg.h"

wxBEGIN_EVENT_TABLE(wxPrinterHeaderFooterDlg, wxDialogWithHelp)
    EVT_BUTTON(wxID_OK, wxPrinterHeaderFooterDlg::OnOK)
wxEND_EVENT_TABLE()

//-------------------------------------------------------------
void wxPrinterHeaderFooterDlg::OnOK([[maybe_unused]] wxCommandEvent& event)
    {
    Validate();
    TransferDataFromWindow();

    //have to do our own custom data transfer here because what is shown in the interface
    //is different from the underlying data.
    m_leftPrinterHeader = ConvertStringToPrinterCommand(leftHeaderPrinterCombo->GetValue());
    m_centerPrinterHeader = ConvertStringToPrinterCommand(centerHeaderPrinterCombo->GetValue());
    m_rightPrinterHeader = ConvertStringToPrinterCommand(rightHeaderPrinterCombo->GetValue());
    m_leftPrinterFooter = ConvertStringToPrinterCommand(leftFooterPrinterCombo->GetValue());
    m_centerPrinterFooter = ConvertStringToPrinterCommand(centerFooterPrinterCombo->GetValue());
    m_rightPrinterFooter = ConvertStringToPrinterCommand(rightFooterPrinterCombo->GetValue());

    if (IsModal())
        { EndModal(wxID_OK); }
    else
        { Show(false); }
    }

//-------------------------------------------------------------
wxString wxPrinterHeaderFooterDlg::ConvertPrinterCommandToString(const wxString& printerString) const
    {
    if (printerString == DONTTRANSLATE("@TITLE@", DTExplanation::InternalKeyword))
        { return _("Title"); }
    else if (printerString == DONTTRANSLATE("@DATE@", DTExplanation::InternalKeyword))
        { return _("Date"); }
    else if (printerString == DONTTRANSLATE("@TIME@", DTExplanation::InternalKeyword))
        { return _("Time"); }
    else if (printerString == DONTTRANSLATE("@PAGENUM@", DTExplanation::InternalKeyword))
        { return _("Page #"); }
    else if (printerString == DONTTRANSLATE("Page @PAGENUM@ of @PAGESCNT@", DTExplanation::InternalKeyword))
        { return _("Page # of #"); }
    else if (printerString == wxEmptyString)
        { return _("None"); }
    else
        { return printerString; }
    }

//-------------------------------------------------------------
wxString wxPrinterHeaderFooterDlg::ConvertStringToPrinterCommand(const wxString& printerString) const
    {
    if (printerString == _("Title"))
        { return DONTTRANSLATE("@TITLE@", DTExplanation::InternalKeyword); }
    else if (printerString == _("Date"))
        { return DONTTRANSLATE("@DATE@", DTExplanation::InternalKeyword); }
    else if (printerString == _("Time"))
        { return DONTTRANSLATE("@TIME@", DTExplanation::InternalKeyword); }
    else if (printerString == _("Page #"))
        { return DONTTRANSLATE("@PAGENUM@", DTExplanation::InternalKeyword); }
    else if (printerString == _("Page # of #"))
        { return DONTTRANSLATE("Page @PAGENUM@ of @PAGESCNT@", DTExplanation::InternalKeyword); }
    else if (printerString == _("None"))
        { return wxEmptyString; }
    else
        { return printerString; }
    }

//-------------------------------------------------------------
void wxPrinterHeaderFooterDlg::CreateControls()
    {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBox* headerFrame = new wxStaticBox(this, wxID_ANY, _("Headers"));
    wxStaticBoxSizer* headerFrameSizer = new wxStaticBoxSizer(headerFrame, wxHORIZONTAL);
    mainSizer->Add(headerFrameSizer, 0, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    wxBoxSizer* headerLeftSizer = new wxBoxSizer(wxVERTICAL);
    headerFrameSizer->Add(headerLeftSizer, 1, wxEXPAND|wxLEFT|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    wxStaticText* leftHeaderTextLabel = new wxStaticText(this, wxID_STATIC, _("Left:"), wxDefaultPosition, wxDefaultSize, 0);
    headerLeftSizer->Add(leftHeaderTextLabel, 0, wxEXPAND|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    wxArrayString choiceStrings;
    choiceStrings.Add(_("None"));
    choiceStrings.Add(_("Title"));
    choiceStrings.Add(_("Page #"));
    choiceStrings.Add(_("Page # of #"));
    choiceStrings.Add(_("Date"));
    choiceStrings.Add(_("Time"));
    leftHeaderPrinterCombo = new wxComboBox(this, ID_LEFT_HEADER_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, choiceStrings, wxCB_DROPDOWN);
    leftHeaderPrinterCombo->SetValue(ConvertPrinterCommandToString(m_leftPrinterHeader));
    headerLeftSizer->Add(leftHeaderPrinterCombo, 1, wxEXPAND);

    wxBoxSizer* headerCenterSizer = new wxBoxSizer(wxVERTICAL);
    headerFrameSizer->Add(headerCenterSizer, 1, wxEXPAND|wxLEFT|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    wxStaticText* centerHeaderTextLabel = new wxStaticText(this, wxID_STATIC, _("Center:"), wxDefaultPosition, wxDefaultSize, 0);
    headerCenterSizer->Add(centerHeaderTextLabel, 0, wxEXPAND|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    centerHeaderPrinterCombo = new wxComboBox(this, ID_CENTER_HEADER_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, choiceStrings, wxCB_DROPDOWN);
    centerHeaderPrinterCombo->SetValue(ConvertPrinterCommandToString(m_centerPrinterHeader));
    headerCenterSizer->Add(centerHeaderPrinterCombo, 1, wxEXPAND);

    wxBoxSizer* headerRightSizer = new wxBoxSizer(wxVERTICAL);
    headerFrameSizer->Add(headerRightSizer, 1, wxEXPAND|wxLEFT|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    wxStaticText* rightHeaderTextLabel = new wxStaticText(this, wxID_STATIC, _("Right:"), wxDefaultPosition, wxDefaultSize, 0);
    headerRightSizer->Add(rightHeaderTextLabel, 0, wxEXPAND|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    rightHeaderPrinterCombo = new wxComboBox(this, ID_RIGHT_HEADER_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, choiceStrings, wxCB_DROPDOWN);
    rightHeaderPrinterCombo->SetValue(ConvertPrinterCommandToString(m_rightPrinterHeader));
    headerRightSizer->Add(rightHeaderPrinterCombo, 1, wxEXPAND);

    wxStaticBox* footersTextBox = new wxStaticBox(this, wxID_ANY, _("Footers"));
    wxStaticBoxSizer* footerSizer = new wxStaticBoxSizer(footersTextBox, wxHORIZONTAL);
    mainSizer->Add(footerSizer, 0, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    wxBoxSizer* footerLeftSizer = new wxBoxSizer(wxVERTICAL);
    footerSizer->Add(footerLeftSizer, 1, wxEXPAND|wxLEFT|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    wxStaticText* leftFooterPrinterText = new wxStaticText(this, wxID_STATIC, _("Left:"), wxDefaultPosition, wxDefaultSize, 0);
    footerLeftSizer->Add(leftFooterPrinterText, 0, wxEXPAND|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    leftFooterPrinterCombo = new wxComboBox(this, ID_LEFT_FOOTER_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, choiceStrings, wxCB_DROPDOWN);
    leftFooterPrinterCombo->SetValue(ConvertPrinterCommandToString(m_leftPrinterFooter));
    footerLeftSizer->Add(leftFooterPrinterCombo, 1, wxEXPAND);

    wxBoxSizer* footerCenterSizer = new wxBoxSizer(wxVERTICAL);
    footerSizer->Add(footerCenterSizer, 1, wxEXPAND|wxLEFT|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    wxStaticText* centerFooterPrinterText = new wxStaticText(this, wxID_STATIC, _("Center:"), wxDefaultPosition, wxDefaultSize, 0);
    footerCenterSizer->Add(centerFooterPrinterText, 0, wxEXPAND|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    centerFooterPrinterCombo = new wxComboBox(this, ID_CENTER_FOOTER_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, choiceStrings, wxCB_DROPDOWN);
    centerFooterPrinterCombo->SetValue(ConvertPrinterCommandToString(m_centerPrinterFooter));
    footerCenterSizer->Add(centerFooterPrinterCombo, 1, wxEXPAND);

    wxBoxSizer* footerRightSizer = new wxBoxSizer(wxVERTICAL);
    footerSizer->Add(footerRightSizer, 1, wxEXPAND|wxLEFT|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    wxStaticText* rightFooterPrinterText = new wxStaticText(this, wxID_STATIC, _("Right:"), wxDefaultPosition, wxDefaultSize, 0);
    footerRightSizer->Add(rightFooterPrinterText, 0, wxEXPAND|wxBOTTOM, wxSizerFlags::GetDefaultBorder());

    rightFooterPrinterCombo = new wxComboBox(this, ID_RIGHT_FOOTER_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, choiceStrings, wxCB_DROPDOWN);
    rightFooterPrinterCombo->SetValue(ConvertPrinterCommandToString(m_rightPrinterFooter));
    footerRightSizer->Add(rightFooterPrinterCombo, 1, wxEXPAND);

    mainSizer->Add(CreateSeparatedButtonSizer(wxOK|wxCANCEL|wxHELP), 0, wxEXPAND|wxALL, wxSizerFlags::GetDefaultBorder());

    SetSizerAndFit(mainSizer);
    }
