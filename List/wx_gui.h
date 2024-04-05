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
    void OnCloseWindow(wxCloseEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void SaveFile();
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void DrawGrid();
    void AddRecord(wxCommandEvent& event);
    void AddAttribute(wxCommandEvent& event);
    void DeleteRecord(wxCommandEvent& event);
    void DeleteAttribute(wxCommandEvent& event);
    void OnCellChanged(wxGridEvent& event);
    void SetTableName(wxCommandEvent& event);
    void RenameAttribute(wxCommandEvent& event);
    bool HandleUnsavedData();
    void OnNew(wxCommandEvent& event);
};

enum {
    ID_New = 1,
    ID_Open = 2,
    ID_Save = 3,
    ID_AddRecord = 4,
    ID_AddAttribute = 5,
    ID_DeleteRecord = 6,
    ID_DeleteAttribute = 7,
    ID_SetTableName = 8,
    ID_RenameAttribute = 9,
};

#endif
