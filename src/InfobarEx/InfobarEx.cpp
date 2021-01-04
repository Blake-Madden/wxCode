/* copyright (c) Oleander Software, Ltd.
   author: Blake Madden
   This program is free software; you can redistribute it and/or modify
   it under the terms of the BSD License.
*/

#include "InfobarEx.h"

bool wxInfoBarEx::Create(wxWindow *parent, wxWindowID winid)
    {
    // calling Hide() before Create() ensures that we're created initially hidden
    Hide();
    if (!wxWindow::Create(parent, winid) )
        return false;

    // use special, easy to notice, colours
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));

    // create the controls: icon, text and the button to dismiss the message.

    // the icon is not shown unless it's assigned a valid bitmap
    m_icon = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);

    m_text = new wxStaticText(this, wxID_ANY, wxEmptyString);
    m_text->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT));

    wxBitmapButton* closeButton = wxBitmapButton::NewCloseButton(this, wxID_CLOSE);
    closeButton->Connect(wxEVT_BUTTON,
                  wxCommandEventHandler(wxInfoBarEx::OnButton), nullptr, this);
    closeButton->SetToolTip(_("Hide this notification message."));

    m_dontShowAgainCheckbox = new wxCheckBox(this, wxID_ANY, _("Do not show this again."), wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_dontShowAgain));

    // center the text inside the sizer with an icon to the left of it and a
    // button at the very right
    wxBoxSizer* const sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* const firstRowSizer = new wxBoxSizer(wxHORIZONTAL);
    firstRowSizer->Add(m_icon, wxSizerFlags().CentreVertical().Border(wxALL, wxSizerFlags::GetDefaultBorder()));
    firstRowSizer->Add(m_text, wxSizerFlags().Expand().Border(wxALL, wxSizerFlags::GetDefaultBorder()).Proportion(1));
    firstRowSizer->AddStretchSpacer();
    firstRowSizer->Add(closeButton, wxSizerFlags().CentreVertical().Border(wxALL, wxSizerFlags::GetDefaultBorder()));

    wxBoxSizer* const secondRowSizer = new wxBoxSizer(wxHORIZONTAL);
    secondRowSizer->Add(m_dontShowAgainCheckbox, wxSizerFlags().CentreVertical().Border(wxALL, wxSizerFlags::GetDefaultBorder()));

    sizer->Add(firstRowSizer, wxSizerFlags().Proportion(1).Expand());
    sizer->Add(secondRowSizer);
    sizer->Show(m_dontShowAgainCheckbox, m_includeDontShowAgain, true);

    SetSizer(sizer);

    return true;
    }

bool wxInfoBarEx::SetForegroundColour(const wxColour& colour)
    {
    if (m_text)
        { return m_text->SetForegroundColour(colour); }

    return true;
    }

wxInfoBarEx::BarPlacement wxInfoBarEx::GetBarPlacement() const
    {
    wxSizer * const sizer = GetContainingSizer();
    if (!sizer)
        return BarPlacement::BarPlacement_Unknown;

    const wxSizerItemList& siblings = sizer->GetChildren();
    if (siblings.GetFirst()->GetData()->GetWindow() == this)
        return BarPlacement::BarPlacement_Top;
    else if (siblings.GetLast()->GetData()->GetWindow() == this)
        return BarPlacement::BarPlacement_Bottom;
    else
        return BarPlacement::BarPlacement_Unknown;
    }

wxShowEffect wxInfoBarEx::GetShowEffect() const
    {
    if (m_showEffect != wxSHOW_EFFECT_MAX )
        { return m_showEffect; }

    switch (GetBarPlacement() )
        {
        case BarPlacement::BarPlacement_Top:
            return wxSHOW_EFFECT_SLIDE_TO_BOTTOM;
        case BarPlacement::BarPlacement_Bottom:
            return wxSHOW_EFFECT_SLIDE_TO_TOP;
        case BarPlacement::BarPlacement_Unknown:
            [[fallthrough]];
        default:
            return wxSHOW_EFFECT_NONE;
        }
    }

wxShowEffect wxInfoBarEx::GetHideEffect() const
    {
    if (m_hideEffect != wxSHOW_EFFECT_MAX )
        { return m_hideEffect; }

    switch (GetBarPlacement() )
        {
        case BarPlacement::BarPlacement_Top:
            return wxSHOW_EFFECT_SLIDE_TO_TOP;
        case BarPlacement::BarPlacement_Bottom:
            return wxSHOW_EFFECT_SLIDE_TO_BOTTOM;
        case BarPlacement::BarPlacement_Unknown:
            [[fallthrough]];
        default:
            return wxSHOW_EFFECT_NONE;
        }
    }

void wxInfoBarEx::UpdateParent()
    {
    wxWindow* const parent = GetParent();
    wxASSERT_LEVEL_2(parent);
    if (parent)
        { parent->Layout(); }
    }

void wxInfoBarEx::DoHide()
    {
    HideWithEffect(GetHideEffect(), GetEffectDuration());
    UpdateParent();
    }

void wxInfoBarEx::DoShow()
    {
    // re-layout the parent first so that the window expands into an already
    // unoccupied by the other controls area: for this we need to change our
    // internal visibility flag to force Layout() to take us into account (an
    // alternative solution to this hack would be to temporarily set
    // wxRESERVE_SPACE_EVEN_IF_HIDDEN flag but it's not really better)

    // just change the internal flag indicating that the window is visible,
    // without really showing it
    wxWindowBase::Show();

    // adjust the parent layout to account for us
    UpdateParent();

    // reset the flag back before really showing the window or it wouldn't be
    // shown at all because it would believe itself already visible
    wxWindowBase::Show(false);

    // finally do really show the window.
    ShowWithEffect(GetShowEffect(), GetEffectDuration());
    }

void wxInfoBarEx::ShowMessage(const wxString& msg, int flags)
    {
    // first update the controls
    const int icon = flags & wxICON_MASK;
    if (!icon || (icon == wxICON_NONE) )
        { m_icon->Hide(); }
    else // do show an icon
        {
        m_icon->SetBitmap(wxArtProvider::GetBitmap(
                            wxArtProvider::GetMessageBoxIconId(flags),
                          wxART_BUTTON, FromDIP(wxSize(16,16))));
        m_icon->Show();
        }

    m_dontShowAgain = false;

    m_text->SetFont(GetFont());
    m_text->SetLabel(wxControl::EscapeMnemonics(msg));

    TransferDataToWindow();

    // then show this entire window if not done yet
    if (!IsShown() )
        { DoShow(); }
    else // we're already shown
        {
        // just update the layout to correspond to the new message
        Layout();
        }
    }

void wxInfoBarEx::Dismiss()
    { DoHide(); }

void wxInfoBarEx::OnButton(wxCommandEvent& event)
    {
    TransferDataFromWindow();
    DoHide();
    event.Skip();
    }
