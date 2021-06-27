/*
Adaptation of ribbon art provider by user "iwbnwif" from:

https://forums.wxwidgets.org/viewtopic.php?f=21&t=37348&p=152217&hilit=art_metro#p152217
*/

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/ribbon/art.h
// Purpose:     Art providers for ribbon-bar-style interface
// Author:      Peter Cawley
// Modified by:
// Created:     2009-05-25
// RCS-ID:      $Id$
// Copyright:   (C) Peter Cawley
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RIBBON_METRO_ART_H_
#define _WX_RIBBON_METRO_ART_H_

#include "wx/defs.h"
#include "wx/ribbon/art.h"
#include "wx/brush.h"
#include "wx/colour.h"
#include "wx/font.h"
#include "wx/pen.h"
#include "wx/bitmap.h"

class WXDLLIMPEXP_FWD_CORE wxDC;
class WXDLLIMPEXP_FWD_CORE wxWindow;

/// Ribbon art provider that emulates the Windows 8 "metro" look.
class wxRibbonMetroArtProvider : public wxRibbonMSWArtProvider
{
public:
    wxRibbonMetroArtProvider(bool set_colour_scheme = true);
    virtual ~wxRibbonMetroArtProvider();
    void SetFlags(long flags);

    void GetColourScheme(wxColour* primary,
                         wxColour* secondary,
                         wxColour* tertiary) const;

    /** Sets the colour of the tabs.
        @param primary The color of the active tab and its tool area.
        @param secondary The color of the background (including inactive tabs).
        @param tertiary The background color of hovered buttons and non-active tabs.*/
    void SetColourScheme(const wxColour& primary,
                         const wxColour& secondary,
                         const wxColour& tertiary);

    void DrawTabCtrlBackground(
                        wxDC& dc,
                        [[maybe_unused]] wxWindow* wnd,
                        const wxRect& rect);

    void DrawTab(wxDC& dc,
                 [[maybe_unused]] wxWindow* wnd,
                 const wxRibbonPageTabInfo& tab);

    void DrawPageBackground(
                        wxDC& dc,
                        [[maybe_unused]] wxWindow* wnd,
                        const wxRect& rect);

    void DrawPanelBackground(
                        wxDC& dc,
                        wxRibbonPanel* wnd,
                        const wxRect& rect);

    void DrawMinimisedPanel(
                        wxDC& dc,
                        wxRibbonPanel* wnd,
                        const wxRect& rect,
                        wxBitmap& bitmap);

    void DrawButtonBarBackground(
                        wxDC& dc,
                        wxWindow* wnd,
                        const wxRect& rect);

    void DrawButtonBarButton(
                        wxDC& dc,
                        [[maybe_unused]] wxWindow* wnd,
                        const wxRect& rect,
                        wxRibbonButtonKind kind,
                        long state,
                        const wxString& label,
                        const wxBitmap& bitmap_large,
                        const wxBitmap& bitmap_small);

    void DrawToolBarBackground(
                        wxDC& dc,
                        wxWindow* wnd,
                        const wxRect& rect);

    void DrawToolGroupBackground(
                        wxDC& dc,
                        [[maybe_unused]] wxWindow* wnd,
                        const wxRect& rect);

    void DrawTool(
                wxDC& dc,
                [[maybe_unused]] wxWindow* wnd,
                const wxRect& rect,
                const wxBitmap& bitmap,
                wxRibbonButtonKind kind,
                long state);

    void DrawToggleButton(
                        wxDC& dc,
                        wxRibbonBar* wnd,
                        const wxRect& rect,
                        wxRibbonDisplayMode mode);

    void DrawHelpButton(
                wxDC& dc,
                wxRibbonBar* wnd,
                const wxRect& rect);

protected:
    void DrawPartialPageBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect,
        bool allow_hovered = true);
    void DrawPartialPageBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect,
         wxRibbonPage* page, wxPoint offset, [[maybe_unused]] bool hovered = false);
    void DrawPanelBorder(wxDC& dc, const wxRect& rect, [[maybe_unused]] wxPen& primary_colour,
        [[maybe_unused]] wxPen& secondary_colour);

    void DrawButtonBarButtonForeground(
                        wxDC& dc,
                        const wxRect& rect,
                        wxRibbonButtonKind kind,
                        long state,
                        const wxString& label,
                        const wxBitmap& bitmap_large,
                        const wxBitmap& bitmap_small);
    void DrawMinimisedPanelCommon(
                        wxDC& dc,
                        wxRibbonPanel* wnd,
                        const wxRect& rect,
                        wxRect* preview_rect);
};

#endif // _WX_RIBBON_METRO_ART_H_
