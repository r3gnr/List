#include "wx_gui.h"

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "List") {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    grid = new wxGrid(this, wxID_ANY);
    grid->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(MyFrame::OnCellChanged), nullptr, this);
    grid->SetCellHighlightPenWidth(true);
    sizer->Add(grid, 1, wxEXPAND);
    SetSizer(sizer);
    SetMinClientSize(wxSize(400, 300));
    Maximize(true);
    grid->CreateGrid(0, 0);
    
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_New, "&New\tCtrl-N",
                     "Create New Table");
    menuFile->Append(ID_Open, "&Open...\tCtrl-O",
                     "Open File");
    menuFile->Append(ID_Save, "&Save...\tCtrl-S",
                     "Save File");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuControl = new wxMenu;
    menuControl->Append(ID_AddRow, "Add Row\tCtrl-R",
                     "Add Row");
    menuControl->Append(ID_AddColumn, "Add Column\tCtrl-T",
                 "Add Column");
    menuControl->Append(ID_DeleteRow, "Delete Row\tCtrl-Y",
                 "Delete Row");
    menuControl->Append(ID_DeleteColumn, "Delete Column\tCtrl-D",
                 "Delete Column");
    menuControl->Append(ID_ChangeDelimiter, "Change Delimiter\tCtrl-L",
                 "Change Delimiter");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuControl, "&Control");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText(table.getFileName());

    Bind(wxEVT_MENU, &MyFrame::OnNew, this, ID_New);
    Bind(wxEVT_MENU, &MyFrame::AddRow, this, ID_AddRow);
    Bind(wxEVT_MENU, &MyFrame::AddColumn, this, ID_AddColumn);
    Bind(wxEVT_MENU, &MyFrame::DeleteRow, this, ID_DeleteRow);
    Bind(wxEVT_MENU, &MyFrame::DeleteColumn, this, ID_DeleteColumn);
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, ID_Open);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, ID_Save);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::ChangeDelimiter, this, ID_ChangeDelimiter);

    Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnCloseWindow, this);

}

void MyFrame::OnNew(wxCommandEvent& event) {
    if (!table.isSaved()) {
        if (MyFrame::HandleUnsavedData() == false) {
            return;
        }
    }
    table.clear();
    grid->DeleteRows(0, grid->GetNumberRows());
    grid->DeleteCols(0, grid->GetNumberCols());
    DrawGrid();
}

void MyFrame::OnCloseWindow(wxCloseEvent& event) {
    if (!table.isSaved()) {
        if (HandleUnsavedData() == false) {
            event.Veto();
            return;
        }
    }
    event.Skip();
}

void MyFrame::OnExit(wxCommandEvent& event) {
    if (!table.isSaved()) { 
        if (MyFrame::HandleUnsavedData() == false) {
            return;
        }
    }
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Simple Table Program",
                 "About", wxOK | wxICON_INFORMATION);
}

bool MyFrame::HandleUnsavedData() {
    switch (wxMessageBox("You have unsaved data. Do you want to save?",
        "Unsaved data", wxYES_NO | wxCANCEL)) {
    case wxYES:
        SaveFile();
        return true;
        break;
    case wxNO:
        return true;
        break;
    case wxCANCEL:
        return false;
        break;
    }
}

void MyFrame::OnOpen(wxCommandEvent& event) {
    if (!table.isSaved()) {
        if (MyFrame::HandleUnsavedData() == false) {
            return;
        }
    }
    
    wxString wildcard = "All files (*.*)|*.*";
    wxFileDialog openFileDialog(this, _("Open File"), "", "", wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wxString filePath = openFileDialog.GetPath();
    table.readTableFile(filePath.ToStdString(), delimiter);
    size_t numRows = table.getRowSize();
    size_t numCols = table.getColumnSize();
    grid->DeleteRows(0, grid->GetNumberRows());
    grid->DeleteCols(0, grid->GetNumberCols());
    grid->AppendRows(numRows);
    grid->AppendCols(numCols);
    DrawGrid();
    SetStatusText(table.getFileName());
}

void MyFrame::SaveFile() {
    wxString wildcard = "CSV file (*.csv)|*.csv";
    wxFileDialog saveFileDialog(this, _("Save File"), "", "", wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL) return;
    wxString filePath = saveFileDialog.GetPath();
    table.writeTableFile(filePath.ToStdString());
}

void MyFrame::OnSave(wxCommandEvent& event) {
    SaveFile();
}

void MyFrame::DrawGrid() {
    int numColumns = table.getColumnSize();
    int windowWidth, windowHeight;
    GetClientSize(&windowWidth, &windowHeight);
    for (size_t i = 0; i < numColumns; ++i) {
        std::string labelName;
        size_t tmp = i + 1;
        while (tmp > 0) {
            char letter = (tmp - 1) % 26 + 'A';
            tmp = (tmp - 1)  / 26;
            labelName = letter + labelName;
        } 
        grid->SetColLabelValue(i, labelName);
    }
    for (size_t i = 0; i < table.getRowSize(); ++i) {
        for (size_t j = 0; j < numColumns; ++j) {
            grid->SetCellValue(i, j, table.getCellValue(i, j));
        }
    }
    grid->SetScrollRate(16, 16);
    size_t gridRowSize = 0;
    for (size_t i = 0; i < table.getRowSize(); ++i) {
        gridRowSize += grid->GetRowSize(i);
    }
    grid->SetVirtualSize(windowWidth, gridRowSize ? gridRowSize : 1);
    grid->SetSize(windowWidth, windowHeight);
    CreateStatusBar();
    SetStatusText(table.getFileName());
}

void MyFrame::AddRow(wxCommandEvent& event) {
    size_t newRowIndex = table.getRowSize();
    table.addRow();
    grid->InsertRows(newRowIndex);
    DrawGrid();
}

void MyFrame::AddColumn(wxCommandEvent& event) {
    size_t newColumnIndex = table.getColumnSize() ;
    table.addColumn();
    grid->InsertCols(newColumnIndex);
    DrawGrid();
}

void MyFrame::OnCellChanged(wxGridEvent& event) {
    size_t recordIndex = event.GetRow();
    size_t attributeIndex = event.GetCol();
    wxString newValue = grid->GetCellValue(recordIndex, attributeIndex);
    table.setCellValue(recordIndex, attributeIndex, newValue.ToStdString());
    DrawGrid();
}

void MyFrame::DeleteRow(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter Record Index (starting from 0)", "Title", "");

    if (dialog.ShowModal() == wxID_CANCEL)
        return;
    wxString indexStr = dialog.GetValue();
    long recordIndex;
    if (!indexStr.ToLong(&recordIndex) || recordIndex < 0 || recordIndex >= static_cast<long>(table.getRowSize())) {
        wxMessageBox("Invalid row index", "Error", wxOK | wxICON_ERROR);
        return;
    }
    table.deleteRow(static_cast<size_t>(recordIndex));
    grid->DeleteRows(recordIndex);
    DrawGrid();
}
void MyFrame::DeleteColumn(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter column Index (starting from 0)", "Title", "");

    if (dialog.ShowModal() == wxID_CANCEL)
        return;

    wxString indexStr = dialog.GetValue();
    long attributeIndex;
    if (!indexStr.ToLong(&attributeIndex) || attributeIndex < 0 || attributeIndex >= static_cast<long>(table.getColumnSize())) {
        wxMessageBox("Invalid column index", "Error", wxOK | wxICON_ERROR);
        return;
    }

    table.deleteColumn(static_cast<size_t>(attributeIndex));
    grid->DeleteCols(attributeIndex);
    DrawGrid();
}

void MyFrame::ChangeDelimiter(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter delimiter", "Title", "");

    if (dialog.ShowModal() == wxID_CANCEL)
        return;

    wxString delimiterStr = dialog.GetValue();
    table.setDilimiter(delimiterStr[0]);
    delimiter = delimiterStr[0];
}