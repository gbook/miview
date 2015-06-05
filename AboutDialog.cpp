/* ----------------------------------------------------------------------------
	AboutDialog.cpp


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
#pragma implementation "AboutDialog.h"
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
#include "MIView.h"
#include "MainFrame.h"
#include <wx/datetime.h>
#include <wx/msgdlg.h>
#include <wx/wxhtml.h>
#include "AboutDialog.h"
#include <wx/utils.h>

////@begin XPM images
////@end XPM images

/*!
 * AboutDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AboutDialog, wxDialog )

/*!
 * AboutDialog event table definition
 */

BEGIN_EVENT_TABLE( AboutDialog, wxDialog )

////@begin AboutDialog event table entries
    EVT_BUTTON( wxID_OK, AboutDialog::OnOkClick )

////@end AboutDialog event table entries

END_EVENT_TABLE()

/*!
 * AboutDialog constructors
 */

AboutDialog::AboutDialog( )
{
}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * AboutDialog creator
 */

bool AboutDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AboutDialog member initialisation
    txtBuildDate = NULL;
    lblVideoCard = NULL;
    lblGLVersion = NULL;
    lblDisplay = NULL;
    lblOSVersion = NULL;
    lblPlatform = NULL;
    htmlWindow = NULL;
////@end AboutDialog member initialisation

////@begin AboutDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AboutDialog creation
    return true;
}

/*!
 * Control creation for AboutDialog
 */

void AboutDialog::CreateControls()
{    
////@begin AboutDialog content construction
    AboutDialog* itemDialog1 = this;

    this->SetBackgroundColour(wxColour(255, 255, 255));
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxPanel* itemPanel3 = new wxPanel( itemDialog1, ID_PANEL2, wxDefaultPosition, wxSize(25, -1), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemPanel3->SetBackgroundColour(wxColour(145, 200, 255));
    itemBoxSizer2->Add(itemPanel3, 1, wxGROW|wxALL, 0);

    wxStaticLine* itemStaticLine4 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    itemBoxSizer2->Add(itemStaticLine4, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBitmap* itemStaticBitmap7 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("miview.ico")), wxDefaultPosition, wxSize(32, 32), 0 );
    itemBoxSizer6->Add(itemStaticBitmap7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer6->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("MIView"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText9->SetFont(wxFont(20, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer6->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 0);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Medical Image Viewer"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText10->SetForegroundColour(wxColour(0, 0, 202));
    itemStaticText10->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer5->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, wxGetTranslation(wxString(wxT("version 0.7 (build 53)\n")) + (wxChar) 0x00A9 + wxT(" Greg Book 2004-2012")), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemBoxSizer5->Add(itemStaticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 10);

    txtBuildDate = new wxStaticText( itemDialog1, wxID_STATIC, _("Build date:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(txtBuildDate, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl13 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINKCTRL2, _("http://gbooksoft.com"), _T("http://gbooksoft.com"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemBoxSizer5->Add(itemHyperlinkCtrl13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl14 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINKCTRL1, _("Email the developer"), _T("mailto:gbook@gbook.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemBoxSizer5->Add(itemHyperlinkCtrl14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemDialog1, wxID_STATIC, _("End User License Agreement"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText16->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer5->Add(itemStaticText16, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

    wxRichTextCtrl* itemRichTextCtrl17 = new wxRichTextCtrl( itemDialog1, ID_RICHTEXTCTRL1, _("IMPORTANT, PLEASE READ CAREFULLY. THIS IS A LICENSE AGREEMENT\n\n\tThis SOFTWARE PRODUCT is protected by copyright laws and international copyright treaties, as well as other intellectual property laws and treaties. This SOFTWARE PRODUCT is licensed, not sold.\n\nEnd User License Agreement\n\nThis End User License Agreement (\"EULA\") is a legal agreement between you (either an individual or a single entity) and The Book Company with regard to the copyrighted Software (herein referred to as \"SOFTWARE PRODUCT\" or \"SOFTWARE\") provided with this EULA.   The SOFTWARE PRODUCT includes computer software, the associated media, any printed materials, and any \"online\" or electronic documentation. Use of any software and related documentation (\"Software\") provided to you by The Book Company in whatever form or media, will constitute your acceptance of these terms, unless separate terms are provided by the software supplier, in which case certain additional or different terms may apply. If you do not agree with the terms of this EULA, do not download, install, copy or use the Software. By installing, copying or otherwise using the SOFTWARE PRODUCT, you agree to be bound by the terms of this EULA.  If you do not agree to the terms of this EULA, The Book Company is unwilling to license the SOFTWARE PRODUCT to you.\n\n0. This software shall be used for research or informational purposes ONLY. It shall not be used as a diagnostic tool or in the treatment of patients. This software is NOT FDA approved. DO NOT USE THIS SOFTWARE AS A TOOL FOR DIAGNOSIS OF MEDICAL IMAGES.\n\n1.  Eligible Licensees. This Software is available for license solely to SOFTWARE owners, with no right of duplication or further distribution, licensing, or sub-licensing.  IF YOU DO NOT OWN THE SOFTWARE, THEN DO NOT DOWNLOAD, INSTALL, COPY OR USE THE SOFTWARE.\n \n2.  License Grant.  The Book Company grants to you a personal, non-transferable and non-exclusive right to use the copy of the Software provided with this EULA. You agree you will not copy the Software except as necessary to use it on a single computer. You agree that you may not copy the written materials accompanying the Software. Modifying, translating, renting, copying, transferring or assigning all or part of the Software, or any rights granted hereunder, to any other persons and removing any proprietary notices, labels or marks from the Software is strictly prohibited.  Furthermore, you hereby agree not to create derivative works based on the Software.  You may not transfer this Software.\n\n3.  Copyright.  The Software is licensed, not sold.  You acknowledge that no title to the intellectual property in the Software is transferred to you. You further acknowledge that title and full ownership rights to the Software will remain the exclusive property of The Book Company and/or its suppliers, and you will not acquire any rights to the Software, except as expressly set forth above. All copies of the Software will contain the same proprietary notices as contained in or on the Software. All title and copyrights in and to the SOFTWARE PRODUCT (including but not limited to any images, photographs, animations, video, audio, music, text and \"applets,\" incorporated into the SOFTWARE PRODUCT), the accompanying printed materials, and any copies of the SOFTWARE PRODUCT, are owned by The Book Company or its suppliers.  The SOFTWARE PRODUCT is protected by copyright laws and international treaty provisions.  You may not copy the printed materials \naccompanying the SOFTWARE PRODUCT.\n\n4.  Reverse Engineering.  You agree that you will not attempt, and if you are a corporation, you will use your best efforts to prevent your employees and contractors from attempting to reverse compile, modify, translate or disassemble the Software in whole or in part. Any failure to comply with the above or any other terms and conditions contained herein will result in the automatic termination of this license and the reversion of the rights granted hereunder to The Book Company.\n\n5.  Disclaimer of Warranty. The Software is provided \"AS IS\" without warranty of any kind. The Book Company and its suppliers disclaim and make no express or implied warranties and specifically disclaim the warranties of merchantability, fitness for a particular purpose and non-infringement of third-party rights. The entire risk as to the quality and performance of the Software is with you. Neither The Book Company nor its suppliers warrant that the functions contained in the Software will meet your requirements or that the operation of the Software will be uninterrupted or error-free. The Book Company IS NOT OBLIGATED TO PROVIDE ANY UPDATES TO THE SOFTWARE.\n\n6.  Limitation of Liability. The Book Company assumes no liability from the use of this software.  In no event shall The Book Company or its suppliers be liable to you for any consequential, special, incidental or indirect damages of any kind arising out of the use or inability to use the software, even if The Book Company or its supplier has been advised of the possibility of such damages, or \nany claim by a third party. \n\n7.  Rental.  You may not loan, rent, or lease the SOFTWARE.  \n\n8.  Upgrades.  If the SOFTWARE is an upgrade from an earlier release or previously released version, you now may use that upgraded product only in accordance with this EULA.  If the SOFTWARE PRODUCT is an upgrade of a software program which you licensed as a single product, the SOFTWARE PRODUCT may be used only as part of that single product package and may not be separated for use on more than one computer.\n\n9.  OEM Product Support. Product support for the SOFTWARE PRODUCT IS provided by The Book Company.  For product support, please call The Book Company.  Should you have any questions concerning this, please refer to the address provided in the documentation.\n\n10.  No Liability for Consequential Damages.  In no event shall The Book Company or its suppliers be liable for any damages whatsoever (including, without limitation, incidental, direct, indirect special and consequential damages, damages for loss of business profits, business interruption, loss of business information, or other pecuniary loss) arising out of the use or inability to use this Book Company product, even if The Book Company has been advised of the possibility of such damages.  Because some states/countries do not allow the exclusion or limitation of liability for consequential or incidental damages, the above limitation may not apply to you.\n\n11.  Indemnification By You.  If you distribute the Software in violation of this Agreement, you agree to indemnify, hold harmless and defend The Book Company and its suppliers from and against any claims or lawsuits, including attorney's fees that arise or result from the use or distribution of the Software in violation of this Agreement.\n\nThe Book Company\nWest Hartford, CT 06110\ngbook@gbook.org"), wxDefaultPosition, wxSize(100, 150), wxTE_READONLY|wxWANTS_CHARS );
    itemRichTextCtrl17->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer5->Add(itemRichTextCtrl17, 0, wxGROW|wxALL, 2);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer18 = new wxStdDialogButtonSizer;

    itemBoxSizer5->Add(itemStdDialogButtonSizer18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton19 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer18->AddButton(itemButton19);

    itemStdDialogButtonSizer18->Realize();

    wxStaticLine* itemStaticLine20 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    itemBoxSizer2->Add(itemStaticLine20, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer21, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("System Information"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText22->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemStaticText22->Wrap(210);
    itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer21->Add(itemBoxSizer23, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText24 = new wxStaticText( itemDialog1, wxID_STATIC, _("Video Card"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText24->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer23->Add(itemStaticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    lblVideoCard = new wxStaticText( itemDialog1, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    lblVideoCard->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer23->Add(lblVideoCard, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer21->Add(itemBoxSizer26, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText27 = new wxStaticText( itemDialog1, wxID_STATIC, _("OpenGL Version"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText27->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer26->Add(itemStaticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    lblGLVersion = new wxStaticText( itemDialog1, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    lblGLVersion->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer26->Add(lblGLVersion, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer21->Add(itemBoxSizer29, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText30 = new wxStaticText( itemDialog1, wxID_STATIC, _("Display Resolution (Depth)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText30->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer29->Add(itemStaticText30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    lblDisplay = new wxStaticText( itemDialog1, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    lblDisplay->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer29->Add(lblDisplay, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer21->Add(itemBoxSizer32, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText33 = new wxStaticText( itemDialog1, wxID_STATIC, _("Operating System"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText33->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer32->Add(itemStaticText33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    lblOSVersion = new wxStaticText( itemDialog1, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    lblOSVersion->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer32->Add(lblOSVersion, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer21->Add(itemBoxSizer35, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText36 = new wxStaticText( itemDialog1, wxID_STATIC, _("Platform"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText36->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer35->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    lblPlatform = new wxStaticText( itemDialog1, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    lblPlatform->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer35->Add(lblPlatform, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    htmlWindow = new wxHtmlWindow( itemDialog1, ID_HTMLWINDOW, wxDefaultPosition, wxSize(200, 150), wxHW_SCROLLBAR_AUTO|wxNO_BORDER|wxHSCROLL|wxVSCROLL );
    itemBoxSizer21->Add(htmlWindow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer21->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( itemDialog1, wxID_STATIC, _("Libraries"), wxDefaultPosition, wxSize(200, -1), wxALIGN_CENTRE );
    itemStaticText40->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer21->Add(itemStaticText40, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxStaticText* itemStaticText41 = new wxStaticText( itemDialog1, wxID_STATIC, _("MIView was built using the following programs and libraries:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText41->Wrap(300);
    itemBoxSizer21->Add(itemStaticText41, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer42 = new wxFlexGridSizer(4, 2, 0, 0);
    itemBoxSizer21->Add(itemFlexGridSizer42, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxHyperlinkCtrl* itemHyperlinkCtrl43 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINK_WX, _("wxWidgets 2.8.11"), _T("http://www.wxwidgets.com"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemFlexGridSizer42->Add(itemHyperlinkCtrl43, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText44 = new wxStaticText( itemDialog1, wxID_STATIC, _("Cross platform application framework"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText44->Wrap(210);
    itemFlexGridSizer42->Add(itemStaticText44, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl45 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINK_GDCM, _("gdcm 2.0.16"), _T("http://sourceforge.net/projects/gdcm"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemFlexGridSizer42->Add(itemHyperlinkCtrl45, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText46 = new wxStaticText( itemDialog1, wxID_STATIC, _("Llibrary for reading DICOM/ACR/NEMA images"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText46->Wrap(210);
    itemFlexGridSizer42->Add(itemStaticText46, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl47 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINK_AFNI, _("ANFI"), _T("http://afni.nimh.nih.gov/afni/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemFlexGridSizer42->Add(itemHyperlinkCtrl47, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText48 = new wxStaticText( itemDialog1, wxID_STATIC, _("Library for reading/writing Analyze/Nifti1 images"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText48->Wrap(210);
    itemFlexGridSizer42->Add(itemStaticText48, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl49 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINK_GLSL, _("glslDevil"), _T("http://www.vis.uni-stuttgart.de/glsldevil/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemFlexGridSizer42->Add(itemHyperlinkCtrl49, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText50 = new wxStaticText( itemDialog1, wxID_STATIC, _("Excellent free GLSL debugger"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText50->Wrap(210);
    itemFlexGridSizer42->Add(itemStaticText50, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl51 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINKCTRL, _("GLEW"), _T("http://glew.sourceforge.net/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemFlexGridSizer42->Add(itemHyperlinkCtrl51, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText52 = new wxStaticText( itemDialog1, wxID_STATIC, _("OpenGL Extension Wrangler"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer42->Add(itemStaticText52, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end AboutDialog content construction
	htmlWindow->SetBorders(1);
	CheckOpenGL();
}

/*!
 * Should we show tooltips?
 */

bool AboutDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AboutDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AboutDialog bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("miview.ico"))
    {
        wxIcon icon(_T("miview.ico"), wxBITMAP_TYPE_ICO);
        wxBitmap bitmap;
        bitmap.CopyFromIcon(icon);
        return bitmap;
    }
    return wxNullBitmap;
////@end AboutDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AboutDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AboutDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AboutDialog icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void AboutDialog::OnOkClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in AboutDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in AboutDialog. 
}



void AboutDialog::CheckOpenGL(void)
{
//	IWbemServices::GetObject(Win32_VideoController;
	const GLubyte *vendorString = glGetString(GL_VENDOR);
	const GLubyte *rendererString = glGetString(GL_RENDERER);
	const GLubyte *versionString = glGetString(GL_VERSION);

	wxString vendorStr = vendorString;
	wxString rendererStr = rendererString;
	wxString versionStr = versionString;
	wxString osVersion = ::wxGetOsDescription();
	wxSize displaySize = wxGetDisplaySize();
	int displayDepth = ::wxDisplayDepth();

	lblVideoCard->SetLabel(vendorStr + " " + rendererStr);
	lblGLVersion->SetLabel(versionStr);
	lblOSVersion->SetLabel(osVersion);
	lblDisplay->SetLabel(wxString::Format("%dx%d (%d bpp)", displaySize.GetWidth(), displaySize.GetHeight(),displayDepth));
	txtBuildDate->SetLabel("Build date: " __DATE__);
#ifdef WIN64
	lblPlatform->SetLabel("64-bit");
#else
	lblPlatform->SetLabel("32-bit");
#endif

	wxString str;
	wxString openGLver = "1.1";
	wxString vertexShader = "Not present";
	wxString fragmentShader = "Not present";
	wxString vertexProgram = "Not present";
	wxString fragmentProgram = "Not present";
	wxString bgcolor = "red";
	wxString texture3D = "Not present";

	/* print the first line to setup the HTML table */
	str = "<font size=\"2\"><table border=\"0\" cellspacing=\"0\" cellpadding=\"2\" width=\"100%\"><tr><td align=\"right\"><b>OpenGL Property</b></td><td><b>Value</b></td></tr>";

	/* get the OpenGL version */
	if (GLEW_VERSION_1_2) { openGLver = "1.2"; bgcolor = "red"; }
	if (GLEW_VERSION_1_3) { openGLver = "1.3"; bgcolor = "red"; }
	if (GLEW_VERSION_1_4) { openGLver = "1.4"; bgcolor = "red"; }
	if (GLEW_VERSION_1_5) { openGLver = "1.5"; bgcolor = "red"; }
	if (GLEW_VERSION_2_0) { openGLver = "2.0"; bgcolor = "#DBF7DD"; }
	if (GLEW_VERSION_2_1) { openGLver = "2.1"; bgcolor = "#DBF7DD"; }
	if (GLEW_VERSION_3_0) { openGLver = "3.0"; bgcolor = "#DBF7DD"; }
	if (GLEW_VERSION_3_1) { openGLver = "3.1"; bgcolor = "#DBF7DD"; }
	if (GLEW_VERSION_3_2) { openGLver = "3.2"; bgcolor = "#DBF7DD"; }
	if (GLEW_VERSION_3_3) { openGLver = "3.3"; bgcolor = "#DBF7DD"; }
	if (GLEW_VERSION_4_0) { openGLver = "4.0"; bgcolor = "#DBF7DD"; }
	if (GLEW_VERSION_4_1) { openGLver = "4.1"; bgcolor = "#DBF7DD"; }

	/* print the OpenGL version */
	str += "<tr><td align=\"right\">OpenGL Version</td><td bgcolor=\"" + bgcolor + "\">" + openGLver + "</td></tr>";

	/* check if vertex shader exists */
	if (GL_ARB_vertex_shader) { vertexShader = "Present"; bgcolor = "#DBF7DD"; }
	else { vertexShader = "Not present"; bgcolor = "red"; }
	str += "<tr><td align=\"right\">Vertex Shader</td><td bgcolor=\"" + bgcolor + "\">" + vertexShader + "</td></tr>";

	/* check if fragment shader exists */
	if (GL_ARB_fragment_shader) { fragmentShader = "Present"; bgcolor = "#DBF7DD"; }
	else { fragmentShader = "Not present"; bgcolor = "red"; }
	str += "<tr><td align=\"right\">Fragment Shader</td><td bgcolor=\"" + bgcolor + "\">" + fragmentShader + "</td></tr>";

	/* check if vertex program exists */
	if (GL_ARB_vertex_program) { vertexProgram = "Present"; bgcolor = "#DBF7DD"; }
	else { vertexProgram = "Not present"; bgcolor = "red"; }
	str += "<tr><td align=\"right\">Vertex Program</td><td bgcolor=\"" + bgcolor + "\">" + vertexProgram + "</td></tr>";

	/* check if fragment program exists */
	if (GL_ARB_fragment_program) { fragmentProgram = "Present"; bgcolor = "#DBF7DD"; }
	else { fragmentProgram = "Not present"; bgcolor = "red"; }
	str += "<tr><td align=\"right\">Fragment Program</td><td bgcolor=\"" + bgcolor + "\">" + fragmentProgram + "</td></tr>";

	/* check if 3D texture exists */
	if (GL_EXT_texture3D) { texture3D = "Present"; bgcolor = "#DBF7DD"; }
	else { texture3D = "Not present"; bgcolor = "red"; }
	str += "<tr><td align=\"right\">3D Textures</td><td bgcolor=\"" + bgcolor + "\">" + texture3D + "</td></tr>";

	str += "</table></font>";
	htmlWindow->AppendToPage(str);
}
