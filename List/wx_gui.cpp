#include "wx_gui.h"

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "List") {

    grid = new wxGrid(this, wxID_ANY);
    grid->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(MyFrame::OnCellChanged), nullptr, this);
    grid->SetCellHighlightPenWidth(true);

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Open, "&Open...\tCtrl-O",
                     "Open File");
    menuFile->Append(ID_Save, "&Save...\tCtrl-S",
                     "Save File");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuControl = new wxMenu;
    menuControl->Append(ID_AddRecord, "Add Record\tCtrl-R",
                     "Add Record");
    menuControl->Append(ID_AddAttribute, "Add Attribute\tCtrl-T",
                 "Add Attribute");
    menuControl->Append(ID_DeleteRecord, "Delete Record\tCtrl-Y",
                 "Delete Record");
    menuControl->Append(ID_DeleteAttribute, "Delete Attribute\tCtrl-D",
                 "Delete Attribute");
    menuControl->Append(ID_SetTableName, "Set Table Name\tCtrl-N",
             "Set Table Name");
    menuControl->Append(ID_RenameAttribute, "Rename Attribute\tCtrl-J",
         "Rename Attribute");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuControl, "&Control");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText(table.getEntityName());

    Bind(wxEVT_MENU, &MyFrame::AddRecord, this, ID_AddRecord);
    Bind(wxEVT_MENU, &MyFrame::AddAttribute, this, ID_AddAttribute);
    Bind(wxEVT_MENU, &MyFrame::DeleteRecord, this, ID_DeleteRecord);
    Bind(wxEVT_MENU, &MyFrame::DeleteAttribute, this, ID_DeleteAttribute);
    Bind(wxEVT_MENU, &MyFrame::SetTableName, this, ID_SetTableName);
    Bind(wxEVT_MENU, &MyFrame::RenameAttribute, this, ID_RenameAttribute);
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, ID_Open);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, ID_Save);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Simple Table Program",
                 "About", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnOpen(wxCommandEvent& event) {
    if (grid) {
        grid->Destroy();
        grid = new wxGrid(this, wxID_ANY);
        grid->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(MyFrame::OnCellChanged), nullptr, this);
        grid->SetCellHighlightPenWidth(true);
    }

    
    wxString wildcard = "All files (*.*)|*.*";
    wxFileDialog openFileDialog(this, _("Open File"), "", "", wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wxString filePath = openFileDialog.GetPath();
    table.readTableFile(filePath.ToStdString());
    DrawGrid();
    SetStatusText(table.getEntityName());
}

void MyFrame::OnSave(wxCommandEvent& event) {
    wxString wildcard = "Text files (*.txt)|*.txt";
    wxFileDialog saveFileDialog(this, _("Save File"), "", "", wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString filePath = saveFileDialog.GetPath();

    table.writeTableFile(filePath.ToStdString());
}

void MyFrame::DrawGrid() {
    grid->CreateGrid(table.getRecordSize(), table.getAttributeSize());
    for (size_t i = 0; i < table.getAttributeSize(); ++i) {
        grid->SetColLabelValue(i, table.getAttributeName(i));
    }
    for (size_t i = 0; i < table.getRecordSize(); ++i) {
        for (size_t j = 0; j < table.getAttributeSize(); ++j) {
            grid->SetCellValue(i, j, wxString::FromUTF8(table.getAttribute(i, j).c_str()));
        }
    }
    grid->AutoSize();
    grid->InvalidateBestSize();
    SetClientSize(grid->GetBestSize());
}

void MyFrame::AddRecord(wxCommandEvent& event) {
    if (grid) {
        grid->Destroy();
        grid = new wxGrid(this, wxID_ANY);
    }
    table.addRecord();
    DrawGrid();
}

void MyFrame::AddAttribute(wxCommandEvent& event) {
    if (grid) {
        grid->Destroy();
        grid = new wxGrid(this, wxID_ANY);
        grid->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(MyFrame::OnCellChanged), nullptr, this);
        grid->SetCellHighlightPenWidth(true);
    }
    wxTextEntryDialog dialog(this, "Enter Attribute Name", "Title", "");

    if (dialog.ShowModal() == wxID_CANCEL)
        return;
    wxString name = dialog.GetValue();
    table.addAttribute(name.ToStdString());
    DrawGrid();
}

void MyFrame::OnCellChanged(wxGridEvent& event) {
    size_t recordIndex = event.GetRow();
    size_t attributeIndex = event.GetCol();
    wxString newValue = grid->GetCellValue(recordIndex, attributeIndex);
    table.setAttribute(recordIndex, attributeIndex, newValue.ToStdString());
    DrawGrid();
}

void MyFrame::DeleteRecord(wxCommandEvent & event) {
    if (grid) {
        grid->Destroy();
        grid = new wxGrid(this, wxID_ANY);
        grid->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(MyFrame::OnCellChanged), nullptr, this);
        grid->SetCellHighlightPenWidth(true);
    }
    wxTextEntryDialog dialog(this, "Enter Record Index (starting from 0)", "Title", "");

    if (dialog.ShowModal() == wxID_CANCEL)
        return;
    wxString name = dialog.GetValue();
    int recordIndex;
    name.ToInt(&recordIndex);
    if (recordIndex >= table.getRecordSize() || recordIndex < 0) {
        DrawGrid();
        return;
    }
    table.deleteRecord(recordIndex);
    DrawGrid();
}
void MyFrame::DeleteAttribute(wxCommandEvent& event) {
    if (grid) {
        grid->Destroy();
        grid = new wxGrid(this, wxID_ANY);
        grid->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(MyFrame::OnCellChanged), nullptr, this);
        grid->SetCellHighlightPenWidth(true);
    }
    wxTextEntryDialog dialog(this, "Enter Attribute Index (starting from 0)", "Title", "");

    if (dialog.ShowModal() == wxID_CANCEL)
        return;
    wxString name = dialog.GetValue();
    int attributeIndex;
    name.ToInt(&attributeIndex);
    if (attributeIndex >= table.getAttributeSize() || attributeIndex < 0) {
        DrawGrid();
        return;
    }
    table.deleteAttribute(attributeIndex);
    DrawGrid();
}

void MyFrame::SetTableName(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter Table Name", "Title", "");

    if (dialog.ShowModal() == wxID_CANCEL)
        return;
    wxString name = dialog.GetValue();
    table.setEntityName(name.ToStdString());
    SetStatusText(table.getEntityName());
}

void MyFrame::RenameAttribute(wxCommandEvent& event) {
    wxTextEntryDialog dialogIndex(this, "Enter Attribute Index (starting from 0)", "Title", "");
    if (dialogIndex.ShowModal() == wxID_CANCEL)
        return;
    wxString index = dialogIndex.GetValue();
    int attributeIndex;
    index.ToInt(&attributeIndex);
    if (attributeIndex >= table.getAttributeSize() || attributeIndex < 0) {
        DrawGrid();
        return;
    }
    wxTextEntryDialog dialog(this, "Enter Attribute Name", "Title", "");
    if (dialog.ShowModal() == wxID_CANCEL)
        return;
    wxString name = dialog.GetValue();
    table.renameAttribute(attributeIndex, name.ToStdString());
    DrawGrid();
}