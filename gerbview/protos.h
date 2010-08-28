/* declarations prototype */

int* InstallDialogLayerPairChoice( WinEDA_GerberFrame* parent );

bool Read_Config();

void Print_PcbItems( BOARD* Pcb, wxDC* DC, int drawmode, int printmasklayer );

void DisplayColorSetupFrame( WinEDA_GerberFrame* parent, const wxPoint& framepos );
void Trace_Segment( BOARD* aBrd, WinEDA_DrawPanel* panel,
                    wxDC*             DC,
                    TRACK*            pt_piste,
                    int               draw_mode );


