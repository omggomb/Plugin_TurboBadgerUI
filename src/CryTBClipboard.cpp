#include <StdAfx.h>
#include <tb_system.h>

namespace tb
{
    /** Empty the contents of the clipboard. */
    void TBClipboard::Empty()
    {
    }

    /** Return true if the clipboard currently contains text. */
    bool TBClipboard::HasText()
    {
        return false;
    }

    /** Set the text of the clipboard in UTF-8 format. */
    bool TBClipboard::SetText( const char* text )
    {
        return false;
    }

    /** Get the text from the clipboard in UTF-8 format.
    Returns true on success. */
    bool TBClipboard::GetText( TBStr& text )
    {
        return false;
    }
}