#ifndef WXCLASS_H
#define WXCLASS_H

#include <wx/wx.h>
#include <wx/grid.h>
#include "table.h"

class MyApp : public wxApp {
public:
    bool OnInit() override;
};

class MyFrame : public wxFrame {
public:
    MyFrame();

private:
    wxGrid* grid;
    Table table;
    char delimiter = ';';
    void OnCloseWindow(wxCloseEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void SaveFile();
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void DrawGrid();
    void AddRow(wxCommandEvent& event);
    void AddColumn(wxCommandEvent& event);
    void DeleteRow(wxCommandEvent& event);
    void DeleteColumn(wxCommandEvent& event);
    void OnCellChanged(wxGridEvent& event);
    bool HandleUnsavedData();
    void OnNew(wxCommandEvent& event);
    void ChangeDelimiter(wxCommandEvent& event);
};

enum {
    ID_New = 1,
    ID_Open = 2,
    ID_Save = 3,
    ID_AddRow = 4,
    ID_AddColumn = 5,
    ID_DeleteRow = 6,
    ID_DeleteColumn = 7,
    ID_ChangeDelimiter = 8,
};

#endif
