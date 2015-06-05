/* ----------------------------------------------------------------------------
	MIViewBrowser.cpp


	MIView - Medical Image Viewer
    Copyright (C) 2009-2011  Gregory Book

    MIView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	--------------------------------------------------------------------------- */


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MIViewBrowser.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "MIViewBrowser.h"
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/imaglist.h>
#include <wx/progdlg.h>
#include "xpm/patient.xpm"
#include "xpm/study.xpm"
#include "xpm/series.xpm"
//#include "gdcm/gdcm.h"
//#include "gdcm/gdcmDocEntry.h"
#include "gdcmImageReader.h"
#include "gdcmAttribute.h"


////@begin XPM images
#include "xpm/close16.xpm"
#include "xpm/rescan.xpm"
////@end XPM images


/*!
 * MIViewBrowser type definition
 */

IMPLEMENT_CLASS( MIViewBrowser, wxFrame )


/*!
 * MIViewBrowser event table definition
 */

BEGIN_EVENT_TABLE( MIViewBrowser, wxFrame )

////@begin MIViewBrowser event table entries
    EVT_MENU( wxID_CLOSE, MIViewBrowser::OnCloseClick )

    EVT_BUTTON( ID_SCAN, MIViewBrowser::OnScanClick )

    EVT_BUTTON( ID_SELECT_SCAN_DIR, MIViewBrowser::OnSelectScanDirClick )

    EVT_TREE_SEL_CHANGED( ID_FILELIST_TREE, MIViewBrowser::OnFilelistTreeSelChanged )
    EVT_TREE_ITEM_ACTIVATED( ID_FILELIST_TREE, MIViewBrowser::OnFilelistTreeItemActivated )

    EVT_BUTTON( ID_EXPAND_ALL, MIViewBrowser::OnExpandAllClick )

    EVT_BUTTON( ID_EXPAND_ALL_PATIENTS, MIViewBrowser::OnExpandAllPatientsClick )

    EVT_BUTTON( ID_COLLAPSE_ALL, MIViewBrowser::OnCollapseAllClick )

    EVT_BUTTON( ID_OPEN_SERIES, MIViewBrowser::OnOpenSeriesClick )

    EVT_BUTTON( ID_OPEN_FILE, MIViewBrowser::OnOpenFileClick )

////@end MIViewBrowser event table entries

END_EVENT_TABLE()


/*!
 * MIViewBrowser constructors
 */

MIViewBrowser::MIViewBrowser()
{
    Init();
}

MIViewBrowser::MIViewBrowser( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*!
 * MIViewBrowser creator
 */

bool MIViewBrowser::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MIViewBrowser creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("../miview.ico")));
    Centre();
////@end MIViewBrowser creation
    return true;
}


/*!
 * MIViewBrowser destructor
 */

MIViewBrowser::~MIViewBrowser()
{
////@begin MIViewBrowser destruction
    GetAuiManager().UnInit();
////@end MIViewBrowser destruction
}


/*!
 * Member initialisation
 */

void MIViewBrowser::Init()
{
////@begin MIViewBrowser member initialisation
    htmlInfo = NULL;
    pnlListing = NULL;
    chkScanRecursively = NULL;
    btnScan = NULL;
    txtScanDir = NULL;
    btnSelectScanDir = NULL;
    treeFileList = NULL;
    btnExpandAll = NULL;
    btnExpandAllPatients = NULL;
    btnCollapseAll = NULL;
    btnOpenSeries = NULL;
    btnOpenFile = NULL;
////@end MIViewBrowser member initialisation

	treeImageList = new wxImageList(24,24);
	wxBitmap patient_bmp(patient_xpm);
	wxBitmap study_bmp(study_xpm);
	wxBitmap series_bmp(series_xpm);
	treeImageList->Add(wxBitmap(patient_bmp));
	treeImageList->Add(wxBitmap(study_bmp));
	treeImageList->Add(wxBitmap(series_bmp));
}


/*!
 * Control creation for MIViewBrowser
 */

void MIViewBrowser::CreateControls()
{    
////@begin MIViewBrowser content construction
    MIViewBrowser* itemFrame1 = this;

    GetAuiManager().SetManagedWindow(this);

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    {
        wxMenuItem* menuItem = new wxMenuItem(itemMenu3, wxID_CLOSE, _("Close"), _T(""), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("../xpm/close16.xpm")));
        menuItem->SetBitmap(bitmap);
        itemMenu3->Append(menuItem);
    }
    menuBar->Append(itemMenu3, _("File"));
    itemFrame1->SetMenuBar(menuBar);

    wxPanel* itemPanel5 = new wxPanel( itemFrame1, ID_PREVIEW_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(itemPanel5, wxAuiPaneInfo()
        .Name(_T("Pane3")).Caption(_T("Image Preview")).Centre().CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    htmlInfo = new wxHtmlWindow( itemFrame1, ID_INFORMATION_PANEL, wxDefaultPosition, wxSize(200, 150), wxHW_SCROLLBAR_AUTO|wxSIMPLE_BORDER|wxHSCROLL|wxVSCROLL );
    itemFrame1->GetAuiManager().AddPane(htmlInfo, wxAuiPaneInfo()
        .Name(_T("Pane1")).Caption(_T("Information")).Centre().MinSize(wxSize(-1, 200)).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    pnlListing = new wxPanel( itemFrame1, ID_LISTING_PANEL, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(pnlListing, wxAuiPaneInfo()
        .Name(_T("Pane2")).Caption(_T("File Listing")).MinSize(wxSize(500, -1)).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    pnlListing->SetSizer(itemBoxSizer8);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer8->Add(itemBoxSizer9, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText10 = new wxStaticText( pnlListing, wxID_STATIC, _("Select Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText10->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer9->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    chkScanRecursively = new wxCheckBox( pnlListing, ID_SCAN_RECURSIVELY, _("Scan Recursively"), wxDefaultPosition, wxDefaultSize, 0 );
    chkScanRecursively->SetValue(true);
    itemBoxSizer9->Add(chkScanRecursively, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    btnScan = new wxBitmapButton( pnlListing, ID_SCAN, itemFrame1->GetBitmapResource(wxT("rescan.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    itemBoxSizer9->Add(btnScan, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer8->Add(itemBoxSizer14, 0, wxGROW|wxALL, 0);

    txtScanDir = new wxTextCtrl( pnlListing, ID_SCAN_DIR, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemBoxSizer14->Add(txtScanDir, 1, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    btnSelectScanDir = new wxButton( pnlListing, ID_SELECT_SCAN_DIR, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer14->Add(btnSelectScanDir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    treeFileList = new wxTreeCtrl( pnlListing, ID_FILELIST_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_NO_LINES|wxTR_FULL_ROW_HIGHLIGHT|wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_ROW_LINES|wxTR_HAS_VARIABLE_ROW_HEIGHT|wxTR_SINGLE );
    treeFileList->SetHelpText(_("Single-click to expand/collapse. Double-click to expand/collapse if the item is already selected."));
    if (MIViewBrowser::ShowToolTips())
        treeFileList->SetToolTip(_("Single-click to expand/collapse. Double-click to expand/collapse if the item is already selected."));
    itemBoxSizer8->Add(treeFileList, 1, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer8->Add(itemBoxSizer18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    btnExpandAll = new wxButton( pnlListing, ID_EXPAND_ALL, _("Expand All"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer18->Add(btnExpandAll, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    btnExpandAllPatients = new wxButton( pnlListing, ID_EXPAND_ALL_PATIENTS, _("Expand Patients"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer18->Add(btnExpandAllPatients, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer18->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnCollapseAll = new wxButton( pnlListing, ID_COLLAPSE_ALL, _("Collapse All"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(btnCollapseAll, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer18->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine24 = new wxStaticLine( pnlListing, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    itemBoxSizer18->Add(itemStaticLine24, 0, wxGROW|wxALL, 0);

    itemBoxSizer18->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnOpenSeries = new wxButton( pnlListing, ID_OPEN_SERIES, _("Open Series"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer18->Add(btnOpenSeries, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    btnOpenFile = new wxButton( pnlListing, ID_OPEN_FILE, _("Open File"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer18->Add(btnOpenFile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemFrame1->GetAuiManager().GetPane(_T("Pane2")).BestSize(pnlListing->GetSizer()->Fit(pnlListing)).MinSize(pnlListing->GetSizer()->GetMinSize());

    GetAuiManager().Update();

    // Connect events and objects
    treeFileList->Connect(ID_FILELIST_TREE, wxEVT_RIGHT_DOWN, wxMouseEventHandler(MIViewBrowser::OnRightDown), NULL, this);
////@end MIViewBrowser content construction

	treeFileList->AssignImageList(treeImageList);
	ReloadDB();
}


/*!
 * Should we show tooltips?
 */

bool MIViewBrowser::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap MIViewBrowser::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MIViewBrowser bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("../xpm/close16.xpm"))
    {
        wxBitmap bitmap( close16_xpm);
        return bitmap;
    }
    else if (name == _T("rescan.xpm"))
    {
        wxBitmap bitmap( rescan_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end MIViewBrowser bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon MIViewBrowser::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MIViewBrowser icon retrieval
    wxUnusedVar(name);
    if (name == _T("../miview.ico"))
    {
        wxIcon icon(_T("../miview.ico"), wxBITMAP_TYPE_ICO);
        return icon;
    }
    return wxNullIcon;
////@end MIViewBrowser icon retrieval
}


void MIViewBrowser::SetPointers(MainFrame *Frame, MainFrameVariables *MainVars)
{
	frame = Frame;
	//logWindow = logwind;
	mainVars = MainVars;
}


// --------------------------------------------------------
// ---------- OnScanClick ---------------------------------
// --------------------------------------------------------
void MIViewBrowser::OnScanClick( wxCommandEvent& event )
{
	RecreateDB();
	ReloadDB();
}


// --------------------------------------------------------
// ---------- RecreateDB ----------------------------------
// --------------------------------------------------------
void MIViewBrowser::RecreateDB()
{
	wxArrayString files;
	wxFile outfile("browsedb.txt",wxFile::write);
	int fileCount;
	int i,j,k,m;
	int numStudies, numSeries, numFiles;
	wxArrayInt offs, lens;
	wxFile dicomfile;
	wxString str, msg;
	wxString patName, patDOB, studyDate, studyTime, seriesNum;
	wxString patientAndDOB, studyDateTimeStr, sliceNum;
	wxString studyName, seriesName, fileName;
	
	if (chkScanRecursively->GetValue())
		wxDir::GetAllFiles(txtScanDir->GetValue(),&files,"",wxDIR_DEFAULT);
	else
		wxDir::GetAllFiles(txtScanDir->GetValue(),&files,"",wxDIR_FILES);
	fileCount = (int)files.Count();

	/* setup the progress dialog */
	wxProgressDialog progDlg(wxT("Creating database..."), wxT(" "), fileCount, this, wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);

	for (i=0;i<fileCount;i++) {
		wxFileName fn(files[i]);
		if ( (fn.GetExt() == "dcm") || (fn.GetExt() == "acr") || (fn.GetExt() == "") ) {
			if (!progDlg.Update(i,msg.Format("Reading file %d of %d",i,fileCount)))
				break;

			gdcm::ImageReader reader;
			/* see if the file will open */
			reader.SetFileName(files[i].c_str());
			if (reader.Read()) {
				gdcm::DataSet dataSet;
				const gdcm::File &file = reader.GetFile();
				dataSet = file.GetDataSet();

				gdcm::Attribute<0x0010,0x0010> at; /* patient name */
				if (dataSet.FindDataElement( at.GetTag() )) {
					const gdcm::DataElement &de = dataSet.GetDataElement( at.GetTag() );
					at.SetFromDataElement( de );
					patName = at.GetValue().c_str();
					patName.Replace("^",",");
				} else { patName = "";}

				gdcm::Attribute<0x0010,0x0030> at1; /* patient birthdate */
				if (dataSet.FindDataElement( at1.GetTag() )) {
					const gdcm::DataElement &de1 = dataSet.GetDataElement( at1.GetTag() );
					at1.SetFromDataElement( de1 );
					patDOB = at1.GetValue().c_str();
				} else { patDOB = "";}

				gdcm::Attribute<0x0008,0x0020> at2; /* study date */
				if (dataSet.FindDataElement( at2.GetTag() )) {
					const gdcm::DataElement &de2 = dataSet.GetDataElement( at2.GetTag() );
					at2.SetFromDataElement( de2 );
					studyDate = at2.GetValue().c_str();
				} else { studyDate = "";}

				gdcm::Attribute<0x0008,0x0030> at3; /* study time */
				if (dataSet.FindDataElement( at3.GetTag() )) {
					const gdcm::DataElement &de3 = dataSet.GetDataElement( at3.GetTag() );
					at3.SetFromDataElement( de3 );
					studyTime = at3.GetValue().c_str();
				} else { studyTime = "";}

				gdcm::Attribute<0x0020,0x0011> at4; /* series number */
				if (dataSet.FindDataElement( at4.GetTag() )) {
					const gdcm::DataElement &de4 = dataSet.GetDataElement( at4.GetTag() );
					at4.SetFromDataElement( de4 );
					seriesNum = wxString::Format("%d",at4.GetValue());
				} else { seriesNum = "";}

				gdcm::Attribute<0x0020,0x0011> at5; /* series number */
				if (dataSet.FindDataElement( at5.GetTag() )) {
					const gdcm::DataElement &de5 = dataSet.GetDataElement( at5.GetTag() );
					at5.SetFromDataElement( de5 );
					sliceNum = wxString::Format("%d",at5.GetValue());
				} else { sliceNum = "";}
				patientAndDOB = patName + "&" + patDOB;
				studyDateTimeStr = studyDate + "&" + studyTime;
				browsedb[patientAndDOB][studyDateTimeStr][seriesNum][files[i]].slicenum = atoi(sliceNum.c_str());
			}
		}
	}

	/* write out the database file */
	for (i=0;i<browsedb.GetNumPatients();i++) {
		patName = browsedb[i].patientName;
		numStudies = browsedb[i].GetNumStudies();
		str = patName + "\n";
		outfile.Write(str);
		for (j=0;j<numStudies;j++) {
			studyName = browsedb[i][j].studyName;
			numSeries = browsedb[i][j].GetNumSeries();
			str = "\t" + studyName + "\n";
			outfile.Write(str);
			for (k=0;k<numSeries;k++) {
				seriesName = browsedb[i][j][k].seriesName;
				numFiles = browsedb[i][j][k].GetNumFiles();
				str = "\t\t" + seriesName + "\n";
				outfile.Write(str);
				for (m=0;m<numFiles;m++) {
					fileName = browsedb[i][j][k][m].fileName;
					str = "\t\t\t" + fileName + "\n";
					outfile.Write(str);
				}
			}
		}
	}
}


// --------------------------------------------------------
// ---------- ReloadDB ------------------------------------
// --------------------------------------------------------
void MIViewBrowser::ReloadDB()
{
	int i;
	wxString msg;

	/* before anything, clear the tree control */
	treeFileList->DeleteAllItems();

	/* read in the database file */
	wxString line;
	wxString patient, study, series, filename;
	wxString date,time, dob;
	wxDateTime studyDateTime;
	int numLines;
	wxTextFile pdbFile("browsedb.txt");
	pdbFile.Open();

	rootID = treeFileList->AddRoot("Root");
	wxTreeItemId curPatientID, curStudyID, curSeriesID, curFileID;

	/* loop through, line by line */
	numLines = (int)pdbFile.GetLineCount();

	/* setup the progress dialog */
	wxProgressDialog progDlg(wxT("Loading Database..."), wxT(" "), numLines, this, wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);
	for (i=0;i<numLines;i++) {
		if (!progDlg.Update(i,msg.Format("Reading entry %d of %d",i,numLines)))
			break;

		line = pdbFile[i];
		if (line[0] != '\t') { /* its a new patient/DOB item */
			/* add a node to the root, set current patient node ID */
			patient = line.Trim(true).Trim(false);
			dob = patient.AfterFirst('&');
			dob = ParseDate(dob);
			patient = patient.BeforeFirst('&');
			patient.Replace("^",",");
			patient += " (DOB: " + dob + ")";
			curPatientID = treeFileList->AppendItem(rootID,patient,0);
			treeFileList->SetItemBackgroundColour(curPatientID,wxColour(200,200,200));
			treeFileList->SetItemBold(curPatientID);
		}
		if ((line[0] == '\t') && (line[1] != '\t')) {
			/* add a new studyDate node, set current studyDate node ID */
			study = line.Trim(true).Trim(false);
			date = study.BeforeFirst('&');
			time = study.AfterFirst('&');
			study = ParseDateAndTime(date,time);

			curStudyID = treeFileList->AppendItem(curPatientID,study,1);

			treeFileList->SetItemBackgroundColour(curStudyID,wxColour(225,225,225));
		}
		if ((line[0] == '\t') && (line[1] == '\t') && (line[2] != '\t')) {
			/* add a new seriesNumber node, set current seriesNumber node ID */
			series = line.Trim(true).Trim(false);
			curSeriesID = treeFileList->AppendItem(curStudyID,series,2);
		}
		if ((line[0] == '\t') && (line[1] == '\t') && (line[2] == '\t')) {
			/* add a new filename node */
			filename = line.Trim(true).Trim(false);
			curFileID = treeFileList->AppendItem(curSeriesID,filename);
		}
		::wxSafeYield();
	}
	ExpandAllPatients();
}


// --------------------------------------------------------
// ---------- OnSelectScanDirClick ------------------------
// --------------------------------------------------------
void MIViewBrowser::OnSelectScanDirClick( wxCommandEvent& event )
{
	wxString dirPath;

	wxDirDialog *dirDialog = new wxDirDialog(this, "Select Browse Directory", "", wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST, wxDefaultPosition, wxDefaultSize,"wxDirCtrl");
	if (dirDialog->ShowModal() == wxID_OK) {
		dirPath = dirDialog->GetPath();
		txtScanDir->SetValue(dirPath);
	}
}


/*!
 * wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_FILELIST_TREE
 */

void MIViewBrowser::OnFilelistTreeItemActivated( wxTreeEvent& event )
{
////@begin wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_FILELIST_TREE in MIViewBrowser.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_FILELIST_TREE in MIViewBrowser. 
}


// --------------------------------------------------------
// ---------- OnFilelistTreeSelChanged --------------------
// --------------------------------------------------------
void MIViewBrowser::OnFilelistTreeSelChanged( wxTreeEvent& event )
{
	wxTreeItemId id = event.GetItem();
	wxString str;
	
	/* this is a patient item */
	if ( treeFileList->GetRootItem() == treeFileList->GetItemParent(id) ) {
		DisplayPatientHTML(id);
	}
	/* this is a study item */
	else if ( treeFileList->GetRootItem() == treeFileList->GetItemParent(treeFileList->GetItemParent(id)) ) {
		DisplayStudyHTML(id);
	}
	/* this is a series item */
	else if ( treeFileList->GetRootItem() == treeFileList->GetItemParent(treeFileList->GetItemParent(treeFileList->GetItemParent(id))) ) {
		DisplaySeriesHTML(id);
	}
	/* otherwise it must be a file item */
	else {
		DisplayFileHTML(id);
	}
}


int MIViewBrowser::GetItemType(wxTreeItemId id)
{
	int type;

	/* this is a patient item */
	if ( rootID == treeFileList->GetItemParent(id) ) {
		type = TREE_TYPE_PATIENT;
	}
	/* this is a study item */
	else if ( rootID == treeFileList->GetItemParent(treeFileList->GetItemParent(id)) ) {
		type = TREE_TYPE_STUDY;
	}
	/* this is a series item */
	else if ( rootID == treeFileList->GetItemParent(treeFileList->GetItemParent(treeFileList->GetItemParent(id))) ) {
		type = TREE_TYPE_SERIES;
	}
	/* otherwise it must be a file item */
	else {
		type = TREE_TYPE_FILE;
	}

	return type;
}


// --------------------------------------------------------
// ---------- DisplayPatientHTML --------------------------
// --------------------------------------------------------
void MIViewBrowser::DisplayPatientHTML(wxTreeItemId id)
{
	wxString str;
	int i;
	int childCount = (int)treeFileList->GetChildrenCount(id);
	wxArrayString kids = GetChildList(id);

	htmlInfo->SetPage("");

	str = "Patient <b>" + treeFileList->GetItemText(id) + "</b> has the following studies:<br><ul>";
	for (i=0;i<kids.Count();i++)
		str += "<li>" + kids[i] + "<br>";
	str += "</ul>";
	htmlInfo->AppendToPage(str);
}


// --------------------------------------------------------
// ---------- DisplayStudyHTML ----------------------------
// --------------------------------------------------------
void MIViewBrowser::DisplayStudyHTML(wxTreeItemId id)
{
	wxString str;
	int i;
	int childCount = (int)treeFileList->GetChildrenCount(id);
	wxArrayString kids = GetChildList(id);

	htmlInfo->SetPage("");

	str = "Study <b>" + treeFileList->GetItemText(id) + "</b> has the following series:<br>";
	for (i=0;i<kids.Count();i++)
		str += kids[i] + "<br>";
	htmlInfo->AppendToPage(str);
}


// --------------------------------------------------------
// ---------- DisplaySeriesHTML ---------------------------
// --------------------------------------------------------
void MIViewBrowser::DisplaySeriesHTML(wxTreeItemId id)
{
	wxString str;
	int i;
	int childCount = (int)treeFileList->GetChildrenCount(id);
	wxArrayString kids = GetChildList(id);

	htmlInfo->SetPage("");

	str = "Series <b>" + treeFileList->GetItemText(id) + "</b> has the following files:<br>";
	for (i=0;i<kids.Count();i++)
		str += kids[i] + "<br>";
	htmlInfo->AppendToPage(str);
}


// --------------------------------------------------------
// ---------- DisplayFileHTML -----------------------------
// --------------------------------------------------------
void MIViewBrowser::DisplayFileHTML(wxTreeItemId id)
{
	wxString str;
//	int i;
	int childCount = (int)treeFileList->GetChildrenCount(id);

	htmlInfo->SetPage("");

	str = "File <b>" + treeFileList->GetItemText(id) + "</b><br>";
	htmlInfo->AppendToPage(str);
}


// --------------------------------------------------------
// ---------- GetChildList --------------------------------
// --------------------------------------------------------
wxArrayString MIViewBrowser::GetChildList(wxTreeItemId id)
{
	int childCount = (int)treeFileList->GetChildrenCount(id);
	wxArrayString kids;
	wxTreeItemId kid;
	wxTreeItemIdValue cookie;
	bool done = false;

	kids.Add(treeFileList->GetItemText(treeFileList->GetFirstChild(id,cookie)));
	while (!done) {
		kid = treeFileList->GetNextChild(id, cookie);
		if ( !kid.IsOk() ) {
			done = true;
			break;
		}
		kids.Add(treeFileList->GetItemText(kid));
	}

	return kids;
}


// --------------------------------------------------------
// ---------- ParseDateAndTime ----------------------------
// --------------------------------------------------------
wxString MIViewBrowser::ParseDateAndTime(wxString date, wxString time)
{
	wxDateTime *ddate, *dtime;
	wxString str = "";

	ddate = new wxDateTime();
	dtime = new wxDateTime();

	/* check the date */
	if (ddate->ParseFormat(date,"%Y%m%d") != NULL)
		str += ddate->Format("%A %B %d %Y");
	if (ddate->ParseFormat(date,"%Y:%m:%d") != NULL)
		str += ddate->Format("%A %B %d %Y");
	if (ddate->ParseFormat(date,"%Y-%m-%d") != NULL)
		str += ddate->Format("%A %B %d %Y");
	if (ddate->ParseFormat(date,"%Y.%m.%d") != NULL)
		str += ddate->Format("%A %B %d %Y");

	if (dtime->ParseFormat(time,"%H%M%S") != NULL)
		str += dtime->Format(" %I:%M:%S %p");
	if (dtime->ParseFormat(time,"%H.%M.%S") != NULL)
		str += dtime->Format(" %I:%M:%S %p");
	if (dtime->ParseFormat(time,"%H:%M:%S") != NULL)
		str += dtime->Format(" %I:%M:%S %p");

	if (str == "")
		str = date + " " + time;

	return str;
}


// --------------------------------------------------------
// ---------- ParseDate -----------------------------------
// --------------------------------------------------------
wxString MIViewBrowser::ParseDate(wxString date)
{
	wxDateTime *ddate;
	wxString str = "";

	ddate = new wxDateTime();

	/* check the date */
	if (ddate->ParseFormat(date,"%Y%m%d") != NULL)
		str += ddate->Format("%Y/%m/%d");
	if (ddate->ParseFormat(date,"%Y:%m:%d") != NULL)
		str += ddate->Format("%Y/%m/%d");
	if (ddate->ParseFormat(date,"%Y-%m-%d") != NULL)
		str += ddate->Format("%Y/%m/%d");
	if (ddate->ParseFormat(date,"%Y.%m.%d") != NULL)
		str += ddate->Format("%Y/%m/%d");

	if (str == "")
		str = date;

	return str;
}


// --------------------------------------------------------
// ---------- OnExpandAllClick ----------------------------
// --------------------------------------------------------
void MIViewBrowser::OnExpandAllClick( wxCommandEvent& event )
{
	ExpandAllPatientsRecursive();
}


// --------------------------------------------------------
// ---------- OnExpandAllPatientsClick --------------------
// --------------------------------------------------------
void MIViewBrowser::OnExpandAllPatientsClick( wxCommandEvent& event )
{
	ExpandAllPatients();
}


// --------------------------------------------------------
// ---------- OnExpandAllPatients -------------------------
// --------------------------------------------------------
void MIViewBrowser::ExpandAllPatients()
{
	int childCount = (int)treeFileList->GetChildrenCount(rootID);
	wxTreeItemId kid;
	wxTreeItemIdValue cookie;
	bool done = false;

	kid = treeFileList->GetFirstChild(rootID,cookie);
	treeFileList->Expand(kid);
	while (!done) {
		kid = treeFileList->GetNextChild(rootID, cookie);
		if ( !kid.IsOk() ) {
			done = true;
			break;
		}
		treeFileList->Expand(kid);
	}
}


// --------------------------------------------------------
// ---------- OnExpandAllPatientsRecursive ----------------
// --------------------------------------------------------
void MIViewBrowser::ExpandAllPatientsRecursive()
{
	int childCount = (int)treeFileList->GetChildrenCount(rootID);
	wxTreeItemId kid;
	wxTreeItemIdValue cookie;
	bool done = false;

	kid = treeFileList->GetFirstChild(rootID,cookie);
	treeFileList->ExpandAllChildren(kid);
	while (!done) {
		kid = treeFileList->GetNextChild(rootID, cookie);
		if ( !kid.IsOk() ) {
			done = true;
			break;
		}
		treeFileList->ExpandAllChildren(kid);
	}
}


// --------------------------------------------------------
// ---------- CollapseAllPatients -------------------------
// --------------------------------------------------------
void MIViewBrowser::CollapseAllPatients()
{
	int childCount = (int)treeFileList->GetChildrenCount(rootID);
	wxTreeItemId kid;
	wxTreeItemIdValue cookie;
	bool done = false;

	kid = treeFileList->GetFirstChild(rootID,cookie);
	treeFileList->CollapseAllChildren(kid);
	while (!done) {
		kid = treeFileList->GetNextChild(rootID, cookie);
		if ( !kid.IsOk() ) {
			done = true;
			break;
		}
		treeFileList->CollapseAllChildren(kid);
	}
}


// --------------------------------------------------------
// ---------- OnCollapseAllClick --------------------------
// --------------------------------------------------------
void MIViewBrowser::OnCollapseAllClick( wxCommandEvent& event )
{
	CollapseAllPatients();
}


// --------------------------------------------------------
// ---------- ShowContextMenu -----------------------------
// --------------------------------------------------------
void MIViewBrowser::ShowContextMenu(const wxPoint& pos)
{
	wxMenu menu;

	menu.Append(ID_MENU_OPEN_SERIES, _("Open Series"), _T(""), wxITEM_NORMAL);
	menu.Append(ID_MENU_OPEN_FILE, _("Open File"), _T(""), wxITEM_NORMAL);
	menu.AppendSeparator();
	menu.Append(wxID_CANCEL, _T("Cancel"));

	PopupMenu(&menu, pos.x, pos.y);
}


// --------------------------------------------------------
// ---------- OnRightDown ---------------------------------
// --------------------------------------------------------
void MIViewBrowser::OnRightDown( wxMouseEvent& event )
{
	wxPoint p = event.GetPosition();

	/* figure out which item was clicked on */
	wxTreeItemId *id = new wxTreeItemId(treeFileList->HitTest(p));

	if (id == NULL) {}
	else {
		if (id->IsOk()) {
			/* select that item */
			treeFileList->SelectItem(id);

			if ( (GetItemType(id) == TREE_TYPE_SERIES) || (GetItemType(id) == TREE_TYPE_FILE) ) {
				/* show the menu */
				ShowContextMenu(p);
			}
		}
	}
	delete id;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPEN_SERIES
 */

void MIViewBrowser::OnOpenSeriesClick( wxCommandEvent& event )
{
	wxTreeItemId id = wxTreeItemId(treeFileList->GetSelection());

	/* this is a patient item */
	if ( rootID == treeFileList->GetItemParent(id) ) {
		wxMessageBox("Select a file or series to open","MIView Information");
	}
	/* this is a study item */
	else if ( rootID == treeFileList->GetItemParent(treeFileList->GetItemParent(id)) ) {
		wxMessageBox("Select a file or series to open","MIView Information");
	}
	/* this is a series item */
	else if ( rootID == treeFileList->GetItemParent(treeFileList->GetItemParent(treeFileList->GetItemParent(id))) ) {
		//wxMessageBox("Select a file to open","MIView Information");
		/* get list of children */
		wxArrayString filelist = GetChildList(id);
		frame->OpenFiles(filelist);
	}
	/* otherwise it must be a file item */
	else {
		/* open the file */
		//wxMessageBox("You selected a file item","MIView Information");
		wxArrayString filelist = GetChildList(treeFileList->GetItemParent(id));
		frame->OpenFiles(filelist);
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPEN_FILE
 */

void MIViewBrowser::OnOpenFileClick( wxCommandEvent& event )
{
	wxTreeItemId id = wxTreeItemId(treeFileList->GetSelection());

	/* this is a patient item */
	if ( rootID == treeFileList->GetItemParent(id) ) {
		wxMessageBox("Select a file to open","MIView Information");
	}
	/* this is a study item */
	else if ( rootID == treeFileList->GetItemParent(treeFileList->GetItemParent(id)) ) {
		wxMessageBox("Select a file to open","MIView Information");
	}
	/* this is a series item */
	else if ( rootID == treeFileList->GetItemParent(treeFileList->GetItemParent(treeFileList->GetItemParent(id))) ) {
		wxMessageBox("Select a file to open","MIView Information");
	}
	/* otherwise it must be a file item */
	else {
		/* open the file */
		//wxMessageBox("You selected a file item","MIView Information");
		wxArrayString filelist;
		filelist.Add(treeFileList->GetItemText(id));
		frame->OpenFiles(filelist);
	}
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
 */

void MIViewBrowser::OnCloseClick( wxCommandEvent& event )
{
	this->Close();
}

