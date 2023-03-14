/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  wxparaver                                *
 *              Paraver Trace Visualization and Analysis Tool                *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

#include "wx/grid.h"
#include <vector>
#include <string>
#include "paraverkerneltypes.h"
#include "paravertypes.h"

class gHistogram;

using std::vector;
using std::string;


class HistoTableBase : public wxGridTableBase
{
  public:
    HistoTableBase();
    HistoTableBase( gHistogram* whichHisto );
    virtual ~HistoTableBase();

    virtual int GetNumberRows();
    virtual int GetNumberCols();

    virtual wxString GetRowLabelValue( int row );
    virtual wxString GetColLabelValue( int col );

    virtual wxString GetValue( int row, int col );
    virtual void SetValue( int row, int col, const wxString &value );
    virtual bool IsEmptyCell( int row, int col );

    virtual wxGridCellAttr *GetAttr( int row, int col, wxGridCellAttr::wxAttrKind kind );

    void setSelectedRows( vector< TObjectOrder > *whichRows );
    void setNoVoidSemRanges( vector< THistogramColumn > *whichCols );
    void setDefaultFontBold( wxFont& whichFont );

  private:
    gHistogram *myHisto;
    vector< TObjectOrder > *selectedRows;
    vector< THistogramColumn > *noVoidSemRanges;
    wxFont cellFontBold;

    string tmpStr;
    wxString label;

    int getNumSemanticColumns() const;
    const wxColour *getTextColorFromLuminance( rgb fromColour ) const;
};
