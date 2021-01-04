/** @addtogroup wxCode
    @brief A collection of wxWidget tools.
    @date 2005-2020
    @copyright Oleander Software, Ltd.
    @author Blake Madden
    @details This program is free software; you can redistribute it and/or modify
    it under the terms of the BSD License.
* @{*/

#ifndef __WXPRINTERHEADERFOOTER_DLG_H__
#define __WXPRINTERHEADERFOOTER_DLG_H__

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/valgen.h>
#include <wx/textdlg.h>
#include <wx/combobox.h>
#include <wx/tooltip.h>
#include "DialogWithHelp.h"

/** @brief Dialog for specifying headers and footers for printing.

    The headers and footers returned from this dialog will embed the following tags
    that the client should convert in their printing code at runtime:

    - `@TITLE@`: The title of the printed document.
    - `@DATE@`: The date when it was printed.
    - `@TIME@`: The time when it was printed.
    - `@PAGENUM@`: The current page number.
    - `@PAGESCNT@`: The number of printed pages.
*/
class wxPrinterHeaderFooterDlg final : public wxDialogWithHelp
    {
public:
    /// Constructor.
    wxPrinterHeaderFooterDlg() noexcept :
                      leftHeaderPrinterCombo(nullptr),
                      centerHeaderPrinterCombo(nullptr),
                      rightHeaderPrinterCombo(nullptr),
                      leftFooterPrinterCombo(nullptr),
                      centerFooterPrinterCombo(nullptr),
                      rightFooterPrinterCombo(nullptr)
      {}
    /** @brief Constructor.
        @param parent The parent window.
        @param leftPrinterHeader The default left printer header.
        @param centerPrinterHeader The default center printer header.
        @param rightPrinterHeader The default right printer header.
        @param leftPrinterFooter The default left printer footer.
        @param centerPrinterFooter The default center printer footer.
        @param rightPrinterFooter The default right printer footer.
        @param id The window ID.
        @param caption The title of the export dialog.
        @param pos The screen position of the window.
        @param size The window size.
        @param style The window style (i.e., decorations and flags).*/
    wxPrinterHeaderFooterDlg(wxWindow* parent,
                      const wxString& leftPrinterHeader, const wxString& centerPrinterHeader,
                      const wxString& rightPrinterHeader, const wxString& leftPrinterFooter,
                      const wxString& centerPrinterFooter, const wxString& rightPrinterFooter,
                      wxWindowID id = wxID_ANY, const wxString& caption = _("Printer Headers & Footers"),
                      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE|wxCLIP_CHILDREN|wxRESIZE_BORDER) :
                      m_leftPrinterHeader(leftPrinterHeader), m_centerPrinterHeader(centerPrinterHeader),
                      m_rightPrinterHeader(rightPrinterHeader), m_leftPrinterFooter(leftPrinterFooter),
                      m_centerPrinterFooter(centerPrinterFooter), m_rightPrinterFooter(rightPrinterFooter),
                      leftHeaderPrinterCombo(nullptr),
                      centerHeaderPrinterCombo(nullptr),
                      rightHeaderPrinterCombo(nullptr),
                      leftFooterPrinterCombo(nullptr),
                      centerFooterPrinterCombo(nullptr),
                      rightFooterPrinterCombo(nullptr)
        { Create(parent, id, caption, pos, size, style); }

    /** Creation step.
        @param parent The parent window.
        @param id The window ID.
        @param caption The title of the export dialog.
        @param pos The screen position of the window.
        @param size The window size.
        @param style The window style(i.e., decorations and flags).
        @returns True upon successful creation.*/
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = _("Printer Headers & Footers"),
                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE|wxCLIP_CHILDREN|wxRESIZE_BORDER)
        {
        SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS|wxDIALOG_EX_METAL);
        wxDialogWithHelp::Create(parent, id, caption, pos, size, style);

        CreateControls();
        Centre();
        return true;
        }

    /// @returns The left header.
    const wxString& GetLeftPrinterHeader() const
        { return m_leftPrinterHeader; }
    /// @returns The center header.
    const wxString& GetCenterPrinterHeader() const
        { return m_centerPrinterHeader; }
    /// @returns The right header.
    const wxString& GetRightPrinterHeader() const
        { return m_rightPrinterHeader; }
    /// @returns The left footer.
    const wxString& GetLeftPrinterFooter() const
        { return m_leftPrinterFooter; }
    /// @returns The center footer.
    const wxString& GetCenterPrinterFooter() const
        { return m_centerPrinterFooter; }
    /// @returns The right footer.
    const wxString& GetRightPrinterFooter() const
        { return m_rightPrinterFooter; }
private:
    void CreateControls();
    void OnOK([[maybe_unused]] wxCommandEvent& event);

    wxString ConvertPrinterCommandToString(const wxString& printerString) const;
    wxString ConvertStringToPrinterCommand(const wxString& printerString) const;
    // headers
    wxString m_leftPrinterHeader;
    wxString m_centerPrinterHeader;
    wxString m_rightPrinterHeader;
    // footers
    wxString m_leftPrinterFooter;
    wxString m_centerPrinterFooter;
    wxString m_rightPrinterFooter;

    static constexpr int ID_LEFT_HEADER_COMBOBOX = 10001;
    static constexpr int ID_CENTER_HEADER_COMBOBOX = 10002;
    static constexpr int ID_RIGHT_HEADER_COMBOBOX = 10003;
    static constexpr int ID_LEFT_FOOTER_COMBOBOX = 10004;
    static constexpr int ID_CENTER_FOOTER_COMBOBOX = 10005;
    static constexpr int ID_RIGHT_FOOTER_COMBOBOX = 10006;

    wxComboBox* leftHeaderPrinterCombo{ nullptr };
    wxComboBox* centerHeaderPrinterCombo{ nullptr };
    wxComboBox* rightHeaderPrinterCombo{ nullptr };
    wxComboBox* leftFooterPrinterCombo{ nullptr };
    wxComboBox* centerFooterPrinterCombo{ nullptr };
    wxComboBox* rightFooterPrinterCombo{ nullptr };

    wxDECLARE_NO_COPY_CLASS(wxPrinterHeaderFooterDlg);
    wxDECLARE_EVENT_TABLE();
    };

/** @}*/

#endif //__WXPRINTERHEADERFOOTER_DLG_H__
