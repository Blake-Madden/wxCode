/* copyright (c) Oleander Software, Ltd.
   author: Blake Madden
   This program is free software; you can redistribute it and/or modify
   it under the terms of the BSD License.
*/

#include "CodeEditor.h"
#include <wx/tokenzr.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/wupdlock.h>

wxIMPLEMENT_CLASS(wxCodeEditor, wxStyledTextCtrl)

wxBEGIN_EVENT_TABLE(wxCodeEditor, wxStyledTextCtrl)
    EVT_STC_MARGINCLICK(wxID_ANY, wxCodeEditor::OnMarginClick)
    EVT_STC_CHARADDED(wxID_ANY, wxCodeEditor::OnCharAdded)
    EVT_STC_AUTOCOMP_SELECTION(wxID_ANY, wxCodeEditor::OnAutoCompletionSelected)
    EVT_KEY_DOWN(wxCodeEditor::OnKeyDown)
    EVT_FIND(wxID_ANY, wxCodeEditor::OnFind)
    EVT_FIND_NEXT(wxID_ANY, wxCodeEditor::OnFind)
wxEND_EVENT_TABLE()

wxCodeEditor::wxCodeEditor(wxWindow* parent, wxWindowID id/*=wxID_ANY*/, const wxPoint& pos/*=wxDefaultPosition*/,
                           const wxSize& size/*=wxDefaultSize*/, long style/*=0*/, const wxString& name/*"wxCodeEditor"*/) :
    wxStyledTextCtrl(parent, id, pos, size, style, name)
    {
    StyleClearAll();
    const wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
    for (auto i = 0; i < wxSTC_STYLE_LASTPREDEFINED; ++i)
        { StyleSetFont(i, font); }

    // code-folding options
    SetProperty(L"fold", L"1");
    SetProperty(L"fold.compact", L"1");
    MarkerDefine (wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_DOTDOTDOT, *wxBLACK, *wxBLACK);
    MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN, *wxBLACK, *wxBLACK);
    MarkerDefine (wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY,     *wxBLACK, *wxBLACK);
    MarkerDefine (wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_DOTDOTDOT, *wxBLACK, *wxWHITE);
    MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, *wxBLACK, *wxWHITE);
    MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY,     *wxBLACK, *wxBLACK);
    MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY,     *wxBLACK, *wxBLACK);
    // margin settings
    SetMarginType(0, wxSTC_MARGIN_NUMBER);
    SetMarginWidth(0, 0); // hide the number margin by default
    SetMarginType(1, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(1, wxSTC_MASK_FOLDERS);
    SetMarginWidth(1, 0); // hide the folding margin by default
    SetMarginSensitive(1, true);
    SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED|wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
    // enable auto-completion
    AutoCompSetIgnoreCase(true);
    AutoCompSetAutoHide(true);

    CallTipUseStyle(40);
    }

void wxCodeEditor::SetLanguage(const int lang)
    {
    if (wxSTC_LEX_LUA == lang)
        {
        // core language keywords
        SetLexer(lang);
        SetKeyWords(0, "and break do else elseif end false for function if in local nil not or repeat return then true until while");
        // other language settings
        SetFileFilter(_("Lua Script (*.lua)|*.lua"));
        SetLibraryAccessor(L'.');
        SetObjectAccessor(L':');
        }

    // highlighting for all supported languages
    const wxColour commentColor(L"FOREST GREEN");
    const wxColour wordColor(*wxBLUE);
    const wxColour stringColor(L"#A31515");
    const wxColour operatorColor(L"#B928C1");

    StyleSetForeground(wxSTC_LUA_WORD, wordColor);
    StyleSetBold(wxSTC_LUA_WORD, true);
    StyleSetForeground(wxSTC_LUA_WORD2, wordColor);
    StyleSetBold(wxSTC_LUA_WORD2, true);
    StyleSetForeground(wxSTC_LUA_STRING, stringColor);
    StyleSetForeground(wxSTC_LUA_OPERATOR, operatorColor);
    StyleSetBold(wxSTC_LUA_OPERATOR, true);
    StyleSetForeground(wxSTC_LUA_COMMENTLINE, commentColor);
    }

void wxCodeEditor::New()
    {
    if (GetModify())
        {
        if (wxMessageBox(_("Do you wish to save your unsaved changes?"),
                _("Save Lua Script"), wxYES_NO|wxICON_QUESTION) == wxYES)
            { Save(); }
        }
    SetText(m_defaultHeader);
    SetSelection(GetLastPosition(), GetLastPosition());
    SetModified(false);
    SetFocus();

    SetScriptFilePath(wxEmptyString);
    }

void wxCodeEditor::Open()
    {
    if (GetModify())
        {
        if (wxMessageBox(_("Do you wish to save your unsaved changes?"),
                _("Save Lua Script"), wxYES_NO|wxICON_QUESTION) == wxYES)
            { Save(); }
        }
    wxFileDialog dialogOpen
            (this, _("Select Script to Open"),
            wxEmptyString, wxEmptyString,
            GetFileFilter(),
            wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_PREVIEW);
    if (dialogOpen.ShowModal() != wxID_OK)
        { return; }
    const wxString filePath = dialogOpen.GetPath();

    wxWindowUpdateLocker noUpdates(this);
    ClearAll();
    LoadFile(filePath);
    SetSelection(0,0);
    SetScriptFilePath(filePath);
    }

void wxCodeEditor::Save()
    {
    if (GetScriptFilePath().empty())
        {
        wxFileDialog dialogSave
                (this, _("Save Script As"),
                wxEmptyString, wxEmptyString,
                GetFileFilter(),
                wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

        if (dialogSave.ShowModal() != wxID_OK)
            { return; }
        SetScriptFilePath(dialogSave.GetPath());
        }
    if (!SaveFile(GetScriptFilePath()) )
        {
        wxMessageBox(wxString::Format(_("Unable to save file \"%s\"."), GetScriptFilePath()), 
            _("Error"), wxOK|wxICON_EXCLAMATION); 
        }
    }

void wxCodeEditor::OnKeyDown(wxKeyEvent& event)
    {
    if (event.ControlDown() && event.GetKeyCode() == L'S')
        { Save(); }
    else if (event.ControlDown() && event.GetKeyCode() == L'O')
        { Open(); }
    else if (event.ControlDown() && event.GetKeyCode() == L'N')
        { New(); }
    else
        { event.Skip(); }
    }

void wxCodeEditor::OnFind(wxFindDialogEvent &event)
    {
    const int flags = event.GetFlags();
    int searchFlags = 0;
    if (flags & wxFR_MATCHCASE)
        { searchFlags = searchFlags|wxSTC_FIND_MATCHCASE; }
    if (flags & wxFR_WHOLEWORD)
        { searchFlags = searchFlags|wxSTC_FIND_WHOLEWORD; }

    if (flags & wxFR_DOWN)
        { FindNext(event.GetFindString(), searchFlags); }
    else
        { FindPrevious(event.GetFindString(), searchFlags); }
    }

void wxCodeEditor::FindPrevious(const wxString& textToFind, const int searchFlags /*= 0*/)
    {
    SearchAnchor();
    long selStart(0), selEnd(0);
    GetSelection(&selStart,&selEnd);
    int foundPos = SearchPrev(searchFlags, textToFind);
    if (foundPos == selStart && foundPos != 0)
        {
        SetSelection(foundPos-1,foundPos-1);
        SearchAnchor();
        foundPos = SearchPrev(searchFlags, textToFind);
        if (foundPos != wxSTC_INVALID_POSITION)
            {
            SetSelection(foundPos,foundPos+textToFind.length());
            EnsureCaretVisible();
            }
        else
            { SetSelection(selStart, selEnd); }
        }
    else if (foundPos != wxSTC_INVALID_POSITION)
        {
        SetSelection(foundPos,foundPos+textToFind.length());
        EnsureCaretVisible();
        }
    // not found going forward, so start from beginning and try from there
    else if (foundPos == wxSTC_INVALID_POSITION)
        {
        wxMessageBox(_("No occurrences found."),
                _("Item Not Found"), wxOK|wxICON_INFORMATION);
        }
    }

void wxCodeEditor::FindNext(const wxString& textToFind, const int searchFlags /*= 0*/)
    {
    SearchAnchor();
    long selStart, selEnd;
    GetSelection(&selStart,&selEnd);
    int foundPos = SearchNext(searchFlags, textToFind);
    if (foundPos == selStart)
        {
        SetSelection(foundPos+textToFind.length(),foundPos+textToFind.length());
        SearchAnchor();
        foundPos = SearchNext(searchFlags, textToFind);
        if (foundPos != wxSTC_INVALID_POSITION)
            {
            SetSelection(foundPos,foundPos+textToFind.length());
            EnsureCaretVisible();
            }
        else
            { SetSelection(selStart, selEnd); }
        }
    else if (foundPos != wxSTC_INVALID_POSITION)
        {
        SetSelection(foundPos,foundPos+textToFind.length());
        EnsureCaretVisible();
        }
    // not found going forward, so start from beginning and try from there
    else if (foundPos == wxSTC_INVALID_POSITION)
        {
        foundPos = FindText(0, GetLength(), textToFind, searchFlags);
        if (foundPos != wxSTC_INVALID_POSITION)
            {
            SetSelection(foundPos,foundPos+textToFind.length());
            EnsureCaretVisible();
            }
        else
            {
            wxMessageBox(_("No occurrences found."),
                    _("Item Not Found"), wxOK|wxICON_INFORMATION);
            }
        }
    }

void wxCodeEditor::AddFunctionsOrClasses(const std::vector<wxString>& functions)
    {
    for (size_t i = 0; i < functions.size(); ++i)
        { m_libaryAndClassNames.insert(StripExtraInfo(functions[i])); }
    }

void wxCodeEditor::AddLibrary(const wxString& library, std::vector<wxString>& functions)
    {
    std::sort(functions.begin(), functions.end());
    wxString functionString;
    wxString returnTypeStr;
    for (size_t i = 0; i < functions.size(); ++i)
        {
        functionString += L" " + StripExtraInfo(functions[i]);
        returnTypeStr = GetReturnType(functions[i]);
        if (returnTypeStr.length())
            { m_libraryFunctionsWithReturnTypes.insert(std::pair<wxString, wxString>(library+L"."+StripExtraInfo(functions[i]), returnTypeStr) ); }
        }
    m_libraryCollection.insert(std::pair<wxString, wxString>(library, functionString) );
    m_libaryAndClassNames.insert(library);
    }

void wxCodeEditor::AddClass(const wxString& theClass, std::vector<wxString>& functions)
    {
    std::sort(functions.begin(), functions.end());
    wxString functionString;
    for (size_t i = 0; i < functions.size(); ++i)
        { functionString += L" " + StripExtraInfo(functions[i]); }
    m_classCollection.insert(std::pair<wxString, wxString>(theClass, functionString) );
    m_libaryAndClassNames.insert(theClass);
    }

void wxCodeEditor::Finalize()
    {
    for (const auto& className : m_libaryAndClassNames)
        { m_libaryAndClassNamesStr += L" " + className; }
    SetKeyWords(1, m_libaryAndClassNamesStr);
    }

wxString wxCodeEditor::StripExtraInfo(const wxString& function)
    {
    const int extraInfoStart = function.find_first_of(L"\t (");
    if (extraInfoStart != wxNOT_FOUND)
        { return function.Mid(0, extraInfoStart); }
    else
        { return function; }
    }

wxString wxCodeEditor::GetReturnType(const wxString& function)
    {
    const int parenthesisStart = function.find(L"\t");
    if (parenthesisStart != wxNOT_FOUND)
        {
        wxString returnType = function.Mid(parenthesisStart);
        returnType.Trim(true); returnType.Trim(false);
        return returnType;
        }
    else
        { return wxEmptyString; }
    }

bool wxCodeEditor::SplitFunctionAndParams(wxString& function, wxString& params)
    {
    const int parenthesisStart = function.Find(L'(');
    if (parenthesisStart != wxNOT_FOUND)
        {
        const int parenthesisEnd = function.Find(L')', true);
        // if empty parameter list then don't bother splitting this up
        if (parenthesisEnd == parenthesisStart+1)
            { return false; }
        params = function.Mid(parenthesisStart+1,(parenthesisEnd-1)-parenthesisStart);
        function.Truncate(parenthesisStart);
        return true;
        }
    return false;
    }

void wxCodeEditor::OnMarginClick(wxStyledTextEvent &event)
    {
    if (event.GetMargin() == 1)
        {
        const int lineClick = LineFromPosition(event.GetPosition());
        if ((GetFoldLevel(lineClick) & wxSTC_FOLDLEVELHEADERFLAG) > 0)
            { ToggleFold(lineClick); }
        }
    }

void wxCodeEditor::OnCharAdded(wxStyledTextEvent &event)
    {
    if (event.GetKey() == GetLibraryAccessor())
        {
        const int wordStart = WordStartPosition(GetCurrentPos()-1, true);
        const wxString lastWord = GetTextRange(wordStart, GetCurrentPos()-1);

        std::map<wxString,wxString,wxStringCmpNoCase>::const_iterator pos = m_libraryCollection.find(lastWord);
        if (pos != m_libraryCollection.end())
            { AutoCompShow(0, pos->second); }
        }
    else if (event.GetKey() == L')' || event.GetKey() == L'(')
        { CallTipCancel(); }
    else if (event.GetKey() == GetObjectAccessor())
        {
        int wordStart = WordStartPosition(GetCurrentPos()-1, false);
        const wxString lastWord = GetTextRange(wordStart, GetCurrentPos()-1);

        if (lastWord == L"()")
            {
            wordStart = WordStartPosition(wordStart-1, false);
            const wxString functionName = GetTextRange(wordStart, GetCurrentPos()-3);
            wordStart = WordStartPosition(wordStart-1, false);
            const wxString libraryName = GetTextRange(wordStart, GetCurrentPos()-3);
            std::map<wxString,wxString,wxStringCmpNoCase>::const_iterator libraryPos = m_libraryFunctionsWithReturnTypes.find(libraryName);
            if (libraryPos != m_libraryFunctionsWithReturnTypes.end())
                {
                libraryPos = m_classCollection.find(libraryPos->second);
                if (libraryPos != m_classCollection.end())
                    { AutoCompShow(0, libraryPos->second); }
                }
            }
        // might be a variable, look for where it was first assigned to something
        int foundPos = 0;
        while (foundPos+lastWord.length()+2 < static_cast<size_t>(wordStart))
            {
            foundPos = FindText(foundPos, wordStart, lastWord, wxSTC_FIND_WHOLEWORD|wxSTC_FIND_MATCHCASE);
            if (foundPos != wxSTC_INVALID_POSITION && foundPos+lastWord.length()+2 < static_cast<size_t>(wordStart))
                {
                foundPos += lastWord.length();
                while (foundPos < GetLength() && GetCharAt(foundPos) == L' ')
                    { ++foundPos; }
                // found an assignment to this variable
                if (foundPos < GetLength() && GetCharAt(foundPos) == L'=')
                    {
                    // scan to whatever it is assigned to
                    do
                        { ++foundPos; }
                    while (foundPos < GetLength() && GetCharAt(foundPos) == L' ');
                    // if it is a known class of ours, then show the functions available for that class
                    wxString assignment = GetTextRange(foundPos, WordEndPosition(foundPos, true));
                    std::map<wxString,wxString,wxStringCmpNoCase>::const_iterator classPos = m_classCollection.find(assignment);
                    if (classPos != m_classCollection.end())
                        {
                        AutoCompShow(0, classPos->second);
                        break;
                        }
                    else
                        { continue; }
                    }
                else
                    { continue; }
                }
            else
                { break; }
            }
        }
    else
        {
        const int wordStart = WordStartPosition(GetCurrentPos(), true);
        const wxString lastWord = GetTextRange(wordStart, GetCurrentPos());

        if (lastWord.length())
            {
            // see if we are inside a library, if so show its list of functions
            if (wordStart > 2 && GetCharAt(wordStart-1) == GetLibraryAccessor())
                {
                const wxString libraryName = GetTextRange(WordStartPosition(wordStart-2, true), wordStart-1);
                std::map<wxString,wxString,wxStringCmpNoCase>::const_iterator pos = m_libraryCollection.find(libraryName);
                if (pos != m_libraryCollection.end())
                    {
                    if (AutoCompActive())
                        { AutoCompSelect(lastWord); }
                    else
                        { AutoCompShow(lastWord.length(), pos->second); }
                    }
                }
            // if an object...
            else if (wordStart > 2 && GetCharAt(wordStart-1) == GetObjectAccessor())
                {
                int previousWordStart = WordStartPosition(wordStart-2, false);
                const wxString previousWord = GetTextRange(previousWordStart, wordStart-1);

                // see if it is an object returned from a known function
                if (previousWord == L"()")
                    {
                    previousWordStart = WordStartPosition(previousWordStart-1, false);
                    const wxString functionName = GetTextRange(previousWordStart, wordStart-1);
                    previousWordStart = WordStartPosition(previousWordStart-1, false);
                    const wxString libraryName = GetTextRange(previousWordStart, wordStart-1);
                    std::map<wxString,wxString,wxStringCmpNoCase>::const_iterator libraryPos = m_libraryFunctionsWithReturnTypes.find(libraryName);
                    if (libraryPos != m_libraryFunctionsWithReturnTypes.end())
                        {
                        libraryPos = m_classCollection.find(libraryPos->second);
                        if (libraryPos != m_classCollection.end())
                            {
                            if (AutoCompActive())
                                { AutoCompSelect(lastWord); }
                            else
                                { AutoCompShow(lastWord.length(), libraryPos->second); }
                            }
                        }
                    }
                }
            // otherwise, we are at the global level, so show list of high-level classes and libraries
            else
                {
                std::set<wxString, wxStringPartialCmpNoCase>::const_iterator pos = m_libaryAndClassNames.find(lastWord);
                wxString foundKeyword, params;
                if (pos != m_libaryAndClassNames.end())
                    {
                    foundKeyword = *pos;
                    SplitFunctionAndParams(foundKeyword,params);
                    }
                // if found a full keyword, then just fix its case and let it auto-highlight
                if (pos != m_libaryAndClassNames.end() &&
                    foundKeyword.length() == lastWord.length())
                    {
                    SetSelection(wordStart,wordStart+lastWord.length());
                    // tooltip the parameters (if applicable)
                    if (params.length())
                        {
                        foundKeyword += L"(";
                        ReplaceSelection(foundKeyword);
                        SetSelection(wordStart+foundKeyword.length(), wordStart+foundKeyword.length());
                        params += L")";
                        CallTipShow(GetCurrentPos(), params);
                        }
                    else
                        {
                        ReplaceSelection(foundKeyword);
                        SetSelection(wordStart+foundKeyword.length(), wordStart+foundKeyword.length());
                        }
                    AutoCompCancel();
                    }
                // or if a partial find, then show auto-completion
                else if (pos != m_libaryAndClassNames.end())
                    {
                    if (AutoCompActive())
                        { AutoCompSelect(lastWord); }
                    else
                        { AutoCompShow(lastWord.length(), m_libaryAndClassNamesStr); }
                    }
                else
                    { AutoCompCancel(); }
                }
            }
        else
            { AutoCompCancel(); }
        }
    event.Skip();
    }

void wxCodeEditor::OnAutoCompletionSelected(wxStyledTextEvent &event)
    {
    wxString selectedString = event.GetText();
    wxString paramText;
    const bool hasParams = SplitFunctionAndParams(selectedString, paramText);

    const int wordStart = WordStartPosition(GetCurrentPos(), true);
    const int wordEnd = WordEndPosition(GetCurrentPos(), true);
    SetSelection(wordStart,wordEnd);
    if (hasParams)
        { selectedString += L"("; }
    ReplaceSelection(selectedString);
    SetSelection(wordStart+selectedString.length(), wordStart+selectedString.length());
    AutoCompCancel();
    if (hasParams)
        {
        paramText += L")";
        CallTipShow(GetCurrentPos(), paramText);
        }
    }
