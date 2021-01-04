/** @addtogroup wxCode
    @brief A collection of wxWidget tools.
    @date 2005-2020
    @copyright Oleander Software, Ltd.
    @author Blake Madden
    @details This program is free software; you can redistribute it and/or modify
    it under the terms of the BSD License.
* @{*/

#ifndef __WXSTART_PAGE_H__
#define __WXSTART_PAGE_H__

#include <wx/wx.h>
#include <wx/window.h>
#include <wx/filename.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/vector.h>

DECLARE_EVENT_TYPE(wxEVT_STARTPAGE_CLICKED, -1)

#define EVT_STARTPAGE_CLICKED(winid, fn) \
    wx__DECLARE_EVT1(wxEVT_STARTPAGE_CLICKED, winid, wxCommandEventHandler(fn))

/** @brief A landing page for an application.

    It displays a MRU list on the right side and a list of customizable list of buttons on the left.

    The events of a user clicking on a file or button can be handled via
    a `EVT_STARTPAGE_CLICKED` message map or bound to `wxEVT_STARTPAGE_CLICKED`.
*/
class wxStartPage final : public wxWindow
    {
public:
    /// @brief The appearance of the buttons on the start page.
    enum class wxStartPageStyle
        {
        wxStartPageFlat, /*!<Flat button appearance.*/
        wxStartPage3D    /*!<3D button appearance.*/
        };
    wxStartPage(wxWindow* parent, wxWindowID id = wxID_ANY, const wxArrayString& mruFiles = wxArrayString(),
        const wxBitmap& logo = wxNullBitmap, const wxBitmap& fileImage = wxNullBitmap,
        const wxString productDescription = wxEmptyString);
    /// Adds a feature button on the left side.
    /// @param bmp The image for the button.
    /// @param label The label on the button.
    void AddButton(const wxBitmap& bmp, const wxString& label)
        { m_buttons.push_back(wxStartPageButton(bmp, label)); }
    /// @returns The ID of the given index into the custom button list, or returns wxNOT_FOUND if an invalid index is given.
    /// @param buttonIndex The index into the custom button list.
    [[nodiscard]] const int GetButtonID(const size_t buttonIndex) const
        { return buttonIndex > m_buttons.size() ? wxNOT_FOUND : m_buttons[buttonIndex].m_id; }
    /// Sets the list of files to be shown in the "most recently used" list.
    /// @param mruFiles The list of file names.
    void SetMRUList(const wxArrayString& mruFiles);
    /// @returns The number of items in the MRU list
    [[nodiscard]] const size_t GetMRUFileCount() const
        { return m_fileButtons.size(); }
    /// @returns The ID of the given index into the MRU file list, or returns wxNOT_FOUND if an invalid index is given.
    /// @param buttonIndex The index into the list of files.
    [[nodiscard]] const int GetMRUButtonID(const size_t buttonIndex) const
        { return buttonIndex > GetMRUFileCount() ? wxNOT_FOUND : m_fileButtons[buttonIndex].m_id; }
    /// Recalculate the sizes of the controls and their layout.
    void Realise();
    /// Sets the appearance of the start page.
    /// @param style The style for the start page.
    void SetStyle(const wxStartPageStyle style) noexcept
        { m_style = style; }
    /// @returns The color of the left side of the start page.
    [[nodiscard]] wxColour GetBackstageBackgroundColor() const noexcept
        { return m_backstageBackgroundColor; }
    /// Sets the color of the left side of the start page.
    /// @param color The color to use.
    void SetBackstageBackgroundColor(const wxColour& color) noexcept
        { m_backstageBackgroundColor = color; }
    /// @returns The font color of the left side of the start page.
    [[nodiscard]] wxColour GetBackstageFontColor() const noexcept
        { return m_backstageFontColor; }
    /// Sets the font color of the left side of the start page.
    /// @param color The color to use.
    void SetBackstageFontColor(const wxColour& color) noexcept
        { m_backstageFontColor = color; }
    /// @returns The color of the right side of the start page.
    [[nodiscard]] wxColour GetDetailBackgroundColor() const noexcept
        { return m_detailBackgroundColor; }
    /// Sets the color of the right side of the start page.
    /// @param color The color to use.
    void SetDetailBackgroundColor(const wxColour& color) noexcept
        { m_detailBackgroundColor = color; }
    /// @returns The font color of the right side of the start page.
    [[nodiscard]] wxColour GetDetailFontColor() const noexcept
        { return m_detailFontColor; }
    /// Sets the font color of the right side of the start page.
    /// @param color The color to use.
    void SetDetailFontColor(const wxColour& color) noexcept
        { m_detailFontColor = color; }
    /// @returns The color of buttons when they are hovered over.
    [[nodiscard]] wxColour GetHoverColor() const noexcept
        { return m_hoverColor; }
    /// Sets the color of buttons when they are hovered over.
    /// @param color The color to use.
    void SetHoverColor(const wxColour& color) noexcept
        { m_hoverColor = color; }
    /// @returns The font color of buttons when they are hovered over.
    [[nodiscard]] wxColour GetHoverFontColor() const noexcept
        { return m_hoverFontColor; }
    /// Sets the font color of buttons when they are hovered over.
    /// @param color The color to use.
    void SetHoverFontColor(const wxColour& color) noexcept
        { m_hoverFontColor = color; }
private:
    struct wxStartPageButton
        {
        wxStartPageButton(const wxBitmap& icon, const wxString& label) : m_icon(icon), m_label(label), m_id(-1)
            {}
        wxStartPageButton(): m_id(-1) {}
        bool IsOk() const
            { return m_label.Length() > 0; }
        wxRect m_rect;
        wxBitmap m_icon;
        wxString m_label;
        int m_id{ -1 };
        };
    void OnPaint([[maybe_unused]] wxPaintEvent& event);
    void OnMouseChange(wxMouseEvent& event);
    void OnMouseClick(wxMouseEvent& event);
    static constexpr int ID_FILE_ID_START = 10001;
    static constexpr int ID_BUTTON_ID_START = 10050;
    /// @returns The padding height around the labels.
    [[nodiscard]] const wxCoord GetLabelPaddingHeight() const
        { return FromDIP(wxSize(wxSizerFlags::GetDefaultBorder(), wxSizerFlags::GetDefaultBorder())).GetHeight(); }
    /// @returns The padding width around the labels.
    [[nodiscard]] const wxCoord GetLabelPaddingWidth() const
        { return FromDIP(wxSize(wxSizerFlags::GetDefaultBorder(), wxSizerFlags::GetDefaultBorder())).GetWidth(); }
    /// @returns The padding at the top of the control.
    [[nodiscard]] const wxCoord GetTopBorder() const
        { return FromDIP(wxSize(wxSizerFlags::GetDefaultBorder()*4, wxSizerFlags::GetDefaultBorder()*4)).GetHeight(); }
    /// @returns The left border around the icons/labels.
    [[nodiscard]] const wxCoord GetLeftBorder() const
        { return FromDIP(wxSize(wxSizerFlags::GetDefaultBorder()*4, wxSizerFlags::GetDefaultBorder()*4)).GetWidth(); }
    /// @returns Size of an icon scaled to 16x16, with label padding above and below it
    [[nodiscard]] const wxCoord GetMruButtonHeight() const
        { return m_mruButtonHeight; }
    void DrawHighlight(wxDC& dc, const wxRect rect, const wxColour color) const
        {
        wxDCPenChanger pc(dc, *wxLIGHT_GREY_PEN);
        if (m_style == wxStartPageStyle::wxStartPage3D)
            {
            // fill with the color
            dc.GradientFillLinear(rect, color, color.ChangeLightness(140), wxSOUTH);
            // create a shiny overlay
            dc.GradientFillLinear(wxRect(rect.GetX(), rect.GetY(),
                                  rect.GetWidth(), rect.GetHeight()*.25),
                                  wxColour(255,255,255,25), wxColour(255,255,255,125), wxSOUTH);
            dc.DrawLine(rect.GetLeftTop(), rect.GetRightTop());
            dc.DrawLine(rect.GetRightTop(), rect.GetRightBottom());
            dc.DrawLine(rect.GetRightBottom(), rect.GetLeftBottom());
            dc.DrawLine(rect.GetLeftBottom(), rect.GetLeftTop());
            }
        else
            {
            wxDCBrushChanger bc(dc, color);
            dc.DrawRectangle(rect);
            }
        }
    wxCoord m_buttonWidth{ 0 };
    wxCoord m_buttonHeight{ 0 };
    wxCoord m_buttonsStart{ 0 };
    wxCoord m_fileColumnHeight{ 0 };
    wxCoord m_mruButtonHeight{ 0 };
    wxFont m_logoFont;
    wxFont m_buttonFont;
    int m_activeButton{ 0 };
    wxVector<wxStartPageButton> m_fileButtons;
    wxVector<wxStartPageButton> m_buttons;
    wxBitmap m_logo;
    wxBitmap m_fileImage;
    wxString m_toolTip;
    wxString m_productDescription;
    wxStartPageStyle m_style{ wxStartPageStyle::wxStartPageFlat };
    wxColour m_backstageBackgroundColor;
    wxColour m_backstageFontColor;
    wxColour m_detailBackgroundColor;
    wxColour m_detailFontColor;
    wxColour m_hoverColor;
    wxColour m_hoverFontColor;

    wxStartPage() : wxWindow(nullptr, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, wxT("wxStartPage")),
        m_buttonWidth(0), m_buttonHeight(0), m_buttonsStart(0), m_fileColumnHeight(),
        m_activeButton(-1), m_logo(wxNullBitmap), m_fileImage(wxNullBitmap),
        m_logoFont(wxFontInfo(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize() * 2)),
        m_buttonFont(wxFontInfo(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize()*1.5)),
        m_style(wxStartPageStyle::wxStartPageFlat), m_productDescription(wxEmptyString),
        m_backstageBackgroundColor(wxColour(145, 168, 208)),
        m_backstageFontColor(*wxWHITE),
        m_detailBackgroundColor(*wxWHITE),
        m_detailFontColor(*wxBLACK),
        m_hoverColor(100, 140, 250),
        m_hoverFontColor(*wxBLACK)
    {}
    wxDECLARE_NO_COPY_CLASS(wxStartPage);
    wxDECLARE_EVENT_TABLE();
    };

/** @}*/

#endif //__WXSTART_PAGE_H__
