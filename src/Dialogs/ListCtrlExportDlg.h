/** @addtogroup wxCode
    @brief A collection of wxWidget tools.
    @date 2005-2020
    @copyright Oleander Software, Ltd.
    @author Blake Madden
    @details This program is free software; you can redistribute it and/or modify
    it under the terms of the BSD License.
* @{*/

#ifndef __WXLISTCTRLEXPORT_DLG_H__
#define __WXLISTCTRLEXPORT_DLG_H__

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/valgen.h>
#include "DialogWithHelp.h"

/** @brief Stores information about exporting a list control.

    @note Column and row indices are stored as 1-indexed values here because that is the
     indexing that a user would use. Caller is responsible for converting these to 0-indexed values.*/
struct wxListCtrlExportOptions
    {
    /// Default constructor.
    wxListCtrlExportOptions() noexcept : m_fromRow(1), m_fromColumn(1), m_toRow(-1), m_toColumn(-1),
        m_includeColumnHeaders(true), m_exportSelected(false), m_includePageBreaks(false)
        {}
    int m_fromRow{ 1 };
    int m_fromColumn{ 1 };
    int m_toRow{ -1 };
    int m_toColumn{ -1 };
    bool m_includeColumnHeaders{ true };
    bool m_exportSelected{ false };
    bool m_includePageBreaks{ false };
    };

/// Formats for exporting a list control.
enum class wxListExportFormat
    {
    wxExportText, /*!< Plain (tab-delimited) text.*/
    wxExportHtml  /*!< HTML report.*/
    };

/// @brief List control export options dialog.
class wxListCtrlExportDlg final : public wxDialogWithHelp
    {
public:
    /** Constructor.
        @param parent The parent to this dialog.
        @param rowCount The total number of rows in the list control that is being exported.
        @param columnCount The total number of columns in the list control that is being exported.
        @param exportFormat The file type to export as.
        @param id The window ID for this dialog.
        @param caption The dialog title.
        @param pos The window position for this dialog.
        @param size The dialog's size.
        @param style The dialog's window styling.*/
    wxListCtrlExportDlg(wxWindow* parent, int rowCount, int columnCount, const wxListExportFormat& exportFormat,
                      wxWindowID id = wxID_ANY, const wxString& caption = _("List Export Options"),
                      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE|wxCLIP_CHILDREN) :
                      m_exportFormat(exportFormat),
                      m_rangeStaticBox(nullptr),
                      m_fromRowLabel(nullptr), m_fromColumnLabel(nullptr), m_toRowLabel(nullptr), m_toColumnLabel(nullptr),
                      m_fromRowSpinCtrl(nullptr), m_fromColumnSpinCtrl(nullptr), m_toRowSpinCtrl(nullptr), m_toColumnSpinCtrl(nullptr)
        {
        m_options.m_toRow = rowCount;
        m_options.m_toColumn = columnCount;
        SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS | wxWS_EX_CONTEXTHELP);
        wxDialogWithHelp::Create(parent, id, caption, pos, size, style);

        CreateControls();
        Centre();
        }
    wxListCtrlExportDlg(const wxListCtrlExportDlg&) = delete;
    wxListCtrlExportDlg(wxListCtrlExportDlg&&) = delete;
    wxListCtrlExportDlg& operator=(const wxListCtrlExportDlg&) = delete;
    wxListCtrlExportDlg& operator=(wxListCtrlExportDlg&&) = delete;
    /// @returns True if user requested page breaks in the output.
    [[nodiscard]] bool IsIncludingPageBreaks() const noexcept
        { return m_options.m_includePageBreaks; }

    /// @returns True if user requested column headers in the output.
    [[nodiscard]] bool IsIncludingColumnHeaders() const noexcept
        { return m_options.m_includeColumnHeaders; }
    /** Sets whether to include column headers in the output.
        @param includeColumnHeaders True to include column headers, false to exclude them.*/
    void IncludeColumnHeaders(const bool includeColumnHeaders)
        {
        m_options.m_includeColumnHeaders = includeColumnHeaders;
        TransferDataToWindow();
        }

    /// @returns True if user requested to export only the rows that are selected.
    [[nodiscard]] bool IsExportingSelectedRows() const noexcept
        { return m_options.m_exportSelected; }
    /** Specifies whether to export all rows, or just the ones that are selected.
        @param exportSelectedRows True to export just the rows that are selected; false to export all rows.*/
    void ExportSelectedRowsOnly(const bool exportSelectedRows)
        {
        m_options.m_exportSelected = exportSelectedRows;
        //changing this option enable/disable the row range options
        TransferDataToWindow();
        wxCommandEvent cmd;
        OnExportSelectedRows(cmd);
        }

    /// @returns The requested start row (1-indexed).
    /// @note May be -1 if not specified.
    [[nodiscard]] int GetFromRow() const noexcept
        { return m_options.m_fromRow; }
    /// @returns The requested ending row (1-indexed).
    /// @note May be -1 if not specified.
    [[nodiscard]] int GetToRow() const noexcept
        { return m_options.m_toRow; }

    /// @returns The requested start column (1-indexed).
    /// @note May be -1 if not specified.
    [[nodiscard]] int GetFromColumn() const noexcept
        { return m_options.m_fromColumn; }
    /// @returns The requested ending column (1-indexed).
    /// @note May be -1 if not specified.
    [[nodiscard]] int GetToColumn() const noexcept
        { return m_options.m_toColumn; }

    /// @returns The user's specified options.
    [[nodiscard]] const wxListCtrlExportOptions& GetExportOptions() const noexcept
        { return m_options; }
private:
    wxListCtrlExportDlg() noexcept {}

    void CreateControls();
    void OnOK([[maybe_unused]] wxCommandEvent& event);
    void OnExportSelectedRows([[maybe_unused]] wxCommandEvent& event);

    wxListCtrlExportOptions m_options;

    wxListExportFormat m_exportFormat{ wxListExportFormat::wxExportText };

    wxStaticBox* m_rangeStaticBox{ nullptr };

    wxStaticText* m_fromRowLabel{ nullptr };
    wxStaticText* m_fromColumnLabel{ nullptr };
    wxStaticText* m_toRowLabel{ nullptr };
    wxStaticText* m_toColumnLabel{ nullptr };

    wxSpinCtrl* m_fromRowSpinCtrl{ nullptr };
    wxSpinCtrl* m_fromColumnSpinCtrl{ nullptr };
    wxSpinCtrl* m_toRowSpinCtrl{ nullptr };
    wxSpinCtrl* m_toColumnSpinCtrl{ nullptr };

    static constexpr int ID_SELECTED_ROW_CHECKBOX = 10001;

    wxDECLARE_EVENT_TABLE();
    };

/** @}*/

#endif //__WXLISTCTRLEXPORT_DLG_H__
