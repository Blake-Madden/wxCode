/** @addtogroup wxCode
    @brief A collection of wxWidget tools.
    @date 2005-2020
    @copyright Oleander Software, Ltd.
    @author Blake Madden
    @details This program is free software; you can redistribute it and/or modify
    it under the terms of the BSD License.
* @{*/

#ifndef __WXCODE_EDITOR_H__
#define __WXCODE_EDITOR_H__

#include <wx/settings.h>
#include <wx/stc/stc.h>
#include <wx/validate.h>
#include <wx/fdrepdlg.h>
#include <map>
#include <set>
#include <vector>

/** @brief A wxStyledTextCtrl-derived editor designed for code editing.

    You can specify a code language via SetLanguage(), and the editor
    will handle loading that language's keywords, lexer, highlighting, class & library separators (for autocompletion),
    and file filter.

    This editor offers a simplified interface for loaring a list of functions and libraries/classes
    (with sub-functions) that will then be recognized by autocompletion and highlighter.

    Also included is built-in file opening and saving support, as well as simplified searching functions.

    @par Example:
    @code
    codeEditor = new wxCodeEditor(theParentDlg);
    // core Lua keywords will now be loaded for highlighting
    codeEditor->SetLanguage(wxSTC_LEX_LUA);

    // add a set of global math functions
    std::vector<wxString> MathFunctions;
    MathFunctions.push_back(wxString(wxT("SIN(x)\t")) + _("Sine of x, which returns the sine of the angle X in radians."));
    MathFunctions.push_back(wxString(wxT("COS(x)\t")) + _("Cosine of x, returns the cosine of the angle X in radians."));

    codeEditor->AddFunctionsOrClasses(MathFunctions);

    // or, add these functions inside of a library named "Math"
    codeEditor->AddLibrary(L"Math", MathFunctions);

    // merge all custom functions, libraries, and classes into the autocompletion and highlighting systems
    codeEditor->Finalize();
    @endcode
*/
class wxCodeEditor final : public wxStyledTextCtrl
    {
public:
    /** Constructor.
        @param parent The parent window.
        @param id The ID for this editor.
        @param pos The position.
        @param size The size of the editor.
        @param style The window style for this editor.
        @param name The class name for this window.*/
    wxCodeEditor(wxWindow* parent, wxWindowID id=wxID_ANY, const wxPoint& pos=wxDefaultPosition,
                 const wxSize& size=wxDefaultSize, long style=0, const wxString& name=L"wxCodeEditor");
    /** Sets the language used in this editor.
        @param lang The language. `wxSTC_LEX_LUA` is currently supported.*/
    void SetLanguage(const int lang);
    /** Adds a library and its functions/classes. This information is used for autocompletion.
        @param library The name of the library.
        @param functions The classes and functions inside of the library. The syntax for this strings
         is the name of the function and (optionally) a return type following a tab character.
         For example, `"GetUser()\tUser"` will load a function named `GetUser` with a return type of `User`.
        @sa Finalize().*/
    void AddLibrary(const wxString& library, std::vector<wxString>& functions);
    /** Adds a class and its functions. This information is used for autocompletion.
        @param theClass The name of the class.
        @param functions The functions inside of the class. The syntax for this strings
         is the name of the function and (optionally) a return type following a tab character.
         For example, `"GetUser()\tUser"` will load a function named `GetUser` with a return type of `User`.
        @sa Finalize().*/
    void AddClass(const wxString& theClass, std::vector<wxString>& functions);
    /** Adds a vector of function or classe names that the highlighting and auto-completion should recognize.
        @param functions The array of functions to add.
        @sa Finalize().*/
    void AddFunctionsOrClasses(const std::vector<wxString>& functions);
    /// Call this after adding all the functions/classes/libraries.
    /// @sa AddFunctionsOrClasses(), AddClass(), AddLibrary().
    void Finalize();
    
    /** Sets whether to include the line-number margins.
        @param include Set to true to include the line-number margins, false to hide them.*/
    void IncludeNumberMargin(const bool include)
        { SetMarginWidth(0, include ? TextWidth(wxSTC_STYLE_LINENUMBER, L"_999999") : 0); }
    /** Sets whether to include the code-folding margins.
        @param include Set to true to include the code-folding margins, false to hide them.*/
    void IncludeFoldingMargin(const bool include)
        { SetMarginWidth(1, include ? 16 : 0); }
    /// @returns The filepath where the script is currently being saved to.
    wxString GetScriptFilePath() const
        { return m_scriptFilePath; }
    /** Sets the path of where the script is being saved.
        @param filePath The filepath of the script.*/
    void SetScriptFilePath(const wxString& filePath)
        { m_scriptFilePath = filePath; }
    /// Saves the script.
    /// @note If the script's filepath has not been set, then will prompt for a path.
    /// @sa SetScriptFilePath().
    void Save();
    /// Prompts for a Lua script and opens it.
    void Open();
    /// Closes the currently open script file and creates a blank one.
    void New();
    /** Search forwards (from the cursor) for a string and moves the selection to it (if found).
        @param textToFind The text to find.
        @param searchFlags How to search. Can be a combination of wxSTC_FIND_WHOLEWORD, wxSTC_FIND_MATCHCASE, wxSTC_FIND_WORDSTART, wxSTC_FIND_REGEXP, and wxSTC_FIND_POSIX.*/
    void FindNext(const wxString& textToFind, const int searchFlags = 0);
    /** Search backwards (from the cursor) for a string and moves the selection to it (if found).
        @param textToFind The text to find.
        @param searchFlags How to search. Can be a combination of wxSTC_FIND_WHOLEWORD, wxSTC_FIND_MATCHCASE, wxSTC_FIND_WORDSTART, wxSTC_FIND_REGEXP, and wxSTC_FIND_POSIX.*/
    void FindPrevious(const wxString& textToFind, const int searchFlags = 0);
    /** When creating a new script, this will be the first line always included.
        This is useful if there is another Lua script always included in new scripts.
        An example of this could be `SetDefaultHeader(L"dofile(\"AppLibrary.lua\")")`.
        @param header The string to include at the top of all new scripts.*/
    void SetDefaultHeader(const wxString& header)
        { m_defaultHeader = header; }
    /// @returns The default header being included in all new scripts.
    /// @sa SetDefaultHeader().
    const wxString& GetDefaultHeader() const noexcept
        { return m_defaultHeader; }
    /** For autocompletion, this sets the character that divides a library/namespace from its
        member classes/functions.
        @param ch The separator character.*/
    void SetLibraryAccessor(const wxChar ch) noexcept
        { m_libraryAccessor = ch; }
    /// @returns The separator between libraries/namespaces and their member classes/functions.
    wxChar GetLibraryAccessor() const noexcept
        { return m_libraryAccessor; }
    /** For autocompletion, this sets the character that divides an object from its member functions.
        @param ch The separator character.*/
    void SetObjectAccessor(const wxChar ch) noexcept
        { m_objectAccessor = ch; }
    /// @returns The separator between objects and their member functions.
    wxChar GetObjectAccessor() const noexcept
        { return m_objectAccessor; }
    /** Sets the file filter for the Open dialog.
        @param filter The file filter.*/
    void SetFileFilter(const wxString& filter) noexcept
        { m_fileFilter = filter; }
    /// @returns The file filter used when opening a script.
    const wxString& GetFileFilter() const noexcept
        { return m_fileFilter; }
private:
    struct wxStringCmpNoCase
        {
        bool operator()(const wxString& s1, const wxString& s2) const
            { return s1.CmpNoCase(s2) < 0; }
        };
    struct wxStringPartialCmpNoCase
        {
        bool operator()(const wxString& s1, const wxString& s2) const
            { return s1.CmpNoCase(s2.Mid(0,s1.length())) < 0; }
        };

    static bool SplitFunctionAndParams(wxString& function, wxString& params);
    static wxString StripExtraInfo(const wxString& function);
    static wxString GetReturnType(const wxString& function);

    void OnMarginClick(wxStyledTextEvent &event);
    void OnCharAdded(wxStyledTextEvent &event);
    void OnAutoCompletionSelected(wxStyledTextEvent &event);
    void OnKeyDown(wxKeyEvent& event);
    void OnFind(wxFindDialogEvent &event);

    std::map<wxString, wxString, wxStringCmpNoCase> m_libraryCollection;
    std::map<wxString, wxString, wxStringCmpNoCase> m_classCollection;
    std::map<wxString, wxString, wxStringCmpNoCase> m_libraryFunctionsWithReturnTypes;
    std::set<wxString, wxStringPartialCmpNoCase> m_libaryAndClassNames;
    wxString m_libaryAndClassNamesStr;

    wxString m_scriptFilePath;

    wxString m_defaultHeader;

    wxString m_fileFilter;

    wxChar m_libraryAccessor{ L'.' };
    wxChar m_objectAccessor{ L':' };

    wxDECLARE_NO_COPY_CLASS(wxCodeEditor);
    wxDECLARE_CLASS(wxCodeEditor);
    wxDECLARE_EVENT_TABLE();
    };

 /** @}*/

#endif //__WXCODE_EDITOR_H__
