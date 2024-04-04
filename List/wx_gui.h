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
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
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
};

enum {
    ID_Open = 1,
    ID_Save = 2,
    ID_AddRecord = 3,
    ID_AddAttribute = 4,
    ID_DeleteRecord = 5,
    ID_DeleteAttribute = 6,
    ID_SetTableName = 7,
    ID_RenameAttribute = 8,
};

#endif
