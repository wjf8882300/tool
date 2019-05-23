#include "file/FolderDialog.h"

namespace MFCExt { namespace Control {
    
    static int CALLBACK BrowseDirectoryCallback( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData ) {
        CFolderDialog* folderDialog = reinterpret_cast< CFolderDialog* >( lpData ); 
        folderDialog->OnCallback( hWnd, uMsg, lParam );            
        return 0;
    }    



    CFolderDialog::CFolderDialog( 
        int rootDirFlag, 
        const CString& focusDir,
        const CString& title,
        DWORD browseInfoFlag
        )
        : rootDirFlag_( rootDirFlag )
        , focusDir_( focusDir )
        , title_( title )
        , browseInfoFlag_( browseInfoFlag )
        , hDialog_( NULL )
        , finalPath_( _T( "" ) )
    {}



    INT_PTR CFolderDialog::DoModal() {
        LPITEMIDLIST rootLoation;
        SHGetSpecialFolderLocation( NULL, rootDirFlag_, &rootLoation );
        if ( rootLoation == NULL ) {
            throw new CInvalidArgException();
        }

        BROWSEINFO browseInfo;
        ZeroMemory( &browseInfo, sizeof( browseInfo ) );
        browseInfo.pidlRoot = rootLoation;
        browseInfo.ulFlags = browseInfoFlag_;
        browseInfo.lpszTitle = title_;
        browseInfo.lpfn = BrowseDirectoryCallback;
        browseInfo.lParam = ( LPARAM )this;
        
        LPITEMIDLIST targetLocation = SHBrowseForFolder( &browseInfo );
        if ( targetLocation == NULL ) {
            return IDCANCEL;
        }
        
        TCHAR targetPath[ MAX_PATH ] = { _T( '\0' ) };
        SHGetPathFromIDList( targetLocation, targetPath );
        finalPath_ = targetPath;
        
        return IDOK;
    }



    void CFolderDialog::OnCallback( HWND hWnd, UINT uMsg, LPARAM lParam ) {
        hDialog_ = hWnd;
        if ( uMsg == BFFM_INITIALIZED ) {
            SetFocusDirectory();            
        }
    }



    void CFolderDialog::SetFocusDirectory() {
        ASSERT( hDialog_ != NULL );
        if ( focusDir_ != _T( "" ) ) {
            ::SendMessage( hDialog_, BFFM_SETSELECTION, TRUE, ( LPARAM )focusDir_.GetString() ); 
        }
    } 

}

}



namespace AutoTest {

    void TestMFCExtFolderDialog() {
        {        
            MFCExt::CFolderDialog dlg;
            if ( dlg.DoModal() == IDOK ) {
                AfxMessageBox( dlg.GetPath() );
            }
        }

        {        
            MFCExt::CFolderDialog dlg( CSIDL_DRIVES, _T( "C:\\Windows" ), _T( "Title" ) );
            if ( dlg.DoModal() == IDOK ) {
                AfxMessageBox( dlg.GetPath() );
            }
        }
    }

}