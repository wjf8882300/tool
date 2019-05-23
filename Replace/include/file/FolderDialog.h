//------------------------------------------------------
// Í·ÎÄ¼þ
#ifndef MFCExt_Control_FolderDialog_H_INCLUDED_
#define MFCExt_Control_FolderDialog_H_INCLUDED_

#include "util/stdafx.h"
namespace MFCExt { 
	
	namespace Control {

    class CFolderDialog {
    public:
        CFolderDialog( 
            int rootDirFlag = CSIDL_DESKTOP, 
            const CString& focusDir = _T( "" ),
            const CString& title = _T( "" ),
            DWORD browseInfoFlag = 0
            );

        INT_PTR DoModal();
        const CString& GetPath() const { return finalPath_; }

    private:
        void OnCallback( HWND hWnd, UINT uMsg, LPARAM lParam );
        void SetFocusDirectory();

    private:
        const int rootDirFlag_;
        const CString focusDir_;
        const CString title_;
        const DWORD browseInfoFlag_;

        HWND hDialog_;
        CString finalPath_;

        friend int CALLBACK BrowseDirectoryCallback( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
    };   

} 

}



namespace MFCExt {
    using Control::CFolderDialog;
}



namespace AutoTest {
    void TestMFCExtFolderDialog();
}

#endif // MFCExt_Control_FolderDialog_H_INCLUDED_