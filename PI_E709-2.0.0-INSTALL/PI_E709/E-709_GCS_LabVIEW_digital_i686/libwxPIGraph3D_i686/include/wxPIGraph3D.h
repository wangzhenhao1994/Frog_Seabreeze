
#include "wx/glcanvas.h"
#include <vector>
#include <map>
#include <valarray>
#include <GCSArray/GCSArray.h>
#include <General/Observer.h>

class ColourMap
{
public:
	void AddColour(const wxColour& colour);
	wxColour GetColour(float fValue) const;
	void GetColour(float fValue, float& red, float& green, float& blue) const;

protected:
	std::vector<wxColour> m_Colours;
	float m_fDiff;
};

class AxisGraph3D : public CObservable
{
public:
	AxisGraph3D();

	void SetRange(double dmin, double dmax);
	void GetRange(double& dmin, double& dmax) const;
	bool InRange(double v) const;
	void SetTicks(int nTicks);
	int GetTicks() const { return m_nTicks; }
	const std::vector<double>& GetGridSteps() const { return m_GridSteps; }

	bool GetDrawTicks() const { return m_bDrawTicks; }
	void SetDrawTicks(bool bDrawTicks = true ) { m_bDrawTicks = bDrawTicks; }

	bool GetDrawLabel() const { return m_bDrawLabel; }
	void SetDrawLabel(bool bDrawLabel = true ) { m_bDrawLabel = bDrawLabel; }

	const wxString& GetLabelText() const { return m_sLabelText; }
	void SetLabelText(const wxString& sLabelText ) { m_sLabelText = sLabelText; }

	const wxString& GetTickFormat() const { return m_sTickFormat; }
	void SetTickFormat(const wxString& sTickFormat) { m_sTickFormat = sTickFormat; }

	enum
	{
		RANGE_CHANGED = 1
	};

protected:
	double m_dMin;
	double m_dMax;
	double m_nTicks;

	bool m_bDrawTicks;
	bool m_bDrawLabel;
	wxString m_sLabelText;
	wxString m_sTickFormat;

	void CalcGridSteps();
	std::vector<double> m_GridSteps;
};

class wxPIGraph3D: public wxGLCanvas, public CObserver
{
	void Render();

public:
	wxPIGraph3D(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style=0, const wxString& name="wxPIGraph3D", const wxPalette& palette = wxNullPalette);
	
	virtual ~wxPIGraph3D();

    virtual bool SetBackgroundColour(const wxColour& colour);

	void ResetPointOfView();

	AxisGraph3D& GetXAxis() { return m_XAxis; }
	const AxisGraph3D& GetXAxis() const { return m_XAxis; }
	AxisGraph3D& GetYAxis() { return m_YAxis; }
	const AxisGraph3D& GetYAxis() const { return m_YAxis; }
	AxisGraph3D& GetZAxis() { return m_ZAxis; }
	const AxisGraph3D& GetZAxis() const { return m_ZAxis; }

	AxisGraph3D* GetAxis(int axis);

	bool GetAutoRange() const { return m_bAutoRange; }
	void SetAutoRange(bool bAutoRange = true);

	bool GetDrawXYGrid() const { return m_bDrawXYGrid; }
	void SetDrawXYGrid(bool bDraw = true) { m_bDrawXYGrid = bDraw; }
	bool GetDrawXZGrid() const { return m_bDrawXZGrid; }
	void SetDrawXZGrid(bool bDraw = true) { m_bDrawXZGrid = bDraw; }
	bool GetDrawYZGrid() const { return m_bDrawYZGrid; }
	void SetDrawYZGrid(bool bDraw = true) { m_bDrawYZGrid = bDraw; }

	bool GetDrawXYProjection() const { return m_bDrawXYProjection; }
	void SetDrawXYProjection(bool bDraw = true) { m_bDrawXYProjection = bDraw; }
	bool GetDrawXZProjection() const { return m_bDrawXZProjection; }
	void SetDrawXZProjection(bool bDraw = true) { m_bDrawXZProjection = bDraw; }
	bool GetDrawYZProjection() const { return m_bDrawYZProjection; }
	void SetDrawYZProjection(bool bDraw = true) { m_bDrawYZProjection = bDraw; }

	bool GetShowWires() const { return m_bShowWires; }
	void SetShowWires( bool bShowWires = true ) { m_bShowWires = bShowWires; }
	bool GetShowHiddenWires() const { return m_bShowHiddenWires; }
	void SetShowHiddenWires( bool bShowHidden = true ) { m_bShowHiddenWires = bShowHidden; }
	bool GetShowSurfaces() const { return m_bShowSurfaces; }
	void SetShowSurfaces( bool bShowSurfaces = true ) { m_bShowSurfaces = bShowSurfaces; }

	void InitGridData( double xStart, double dx, size_t nx, double yStart, double dy, size_t ny );
	void AddDataPoint( size_t ix, size_t iy, double v );

	void ClearData();
	bool PlotGCSArray( const CGCSArray& data );

	float m_light0x;
	float m_light0y;
	float m_light0z;

	typedef enum PROJECTION
	{
		ORTHOGRAPHIC = 1, PERSPECTIVE = 2
	};

	static const wxString BLUE_RED;
	static const wxString SPECTRUM;
	static const wxString GRAYSCALE;
	static const wxString RED_YELLOW;
	static const wxString RED_GREEN;


	PROJECTION GetProjection() const { return m_Projection; }
	void SetProjection( PROJECTION p );

	void CopyToClipboard();
	bool SaveToImageFile(const wxString& filename, int type);

	bool SelectColourMap( const wxString& sName );
	void AddColourMap( const wxString& sName, const ColourMap& colourMap );
	const ColourMap* GetCurrentColourMap() { return &m_currentColourMap; }

	// from CObserver
	virtual void Update(CObservable* pObservable, int cookie, int data);

#ifdef _DEBUG
	virtual void Refresh(bool eraseBackground = true, const wxRect* rect = NULL);
#endif

protected:

	void Resized(wxSizeEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void Paint(wxPaintEvent& event);
	void DrawCoordsys(void);
	void DrawTicks(void);
	void DrawGrids(void);
	void DrawData(void);
	void CalcColourValues(void);

	void MouseMoved(wxMouseEvent& event);
	void MouseLeftDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void MouseLeftUp(wxMouseEvent& event);
	void MouseRightDown(wxMouseEvent& event);
	void MouseRightUp(wxMouseEvent& event);
	void MouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	void EvtEraseBackground(wxEraseEvent& event)  { /* do nothing to prevent flicker*/ }
	void MouseCaptureLost(wxMouseCaptureLostEvent& event);

    DECLARE_EVENT_TABLE()
	wxGLContext m_context;
	float m_xRot;
	float m_yRot;
	bool m_bDragLeft;
	bool m_bDragShiftLeft;
	bool m_bDragRight;
	wxPoint m_PosDragStart;
	float m_xRotStart;
	float m_yRotStart;
	float m_xShift;
	float m_yShift;
	float m_zShift;
	float m_xShiftStart;
	float m_yShiftStart;
	float m_zShiftStart;

	float m_BackColRed;
	float m_BackColGreen;
	float m_BackColBlue;

	AxisGraph3D m_XAxis;
	AxisGraph3D m_YAxis;
	AxisGraph3D m_ZAxis;

	bool m_bDrawXYGrid;
	bool m_bDrawXZGrid;
	bool m_bDrawYZGrid;

	bool m_bDrawXYProjection;
	bool m_bDrawXZProjection;
	bool m_bDrawYZProjection;

	std::valarray<bool> m_vaGridDataValid;
	std::valarray<double> m_vaGridDataZ;
	std::vector<double> m_vGridDataX;
	std::vector<double> m_vGridDataY;
	std::valarray<float> m_vaColourRed;
	std::valarray<float> m_vaColourGreen;
	std::valarray<float> m_vaColourBlue;
	

	bool m_bAutoRange;
	bool m_bZRangeModified;
	bool m_bIgnoreRangeChange;

	bool m_bShowWires;
	bool m_bShowHiddenWires;
	bool m_bShowSurfaces;

	void glPrint( const wxString& sText, float x, float y, float z);

	void glPreparePrint( const wxString& sText, wxSize& textSize);
	void glFinishPrint( wxSize& textSize, float x, float y, float z);
#ifdef WIN32
	wxBitmap m_TextBmp;
	GLubyte* m_TextRaster;
	int m_TextRasterBufSize;
#else
	wxString m_sPrintText;
#endif
	
	PROJECTION m_Projection;

	ColourMap m_currentColourMap;
	std::map<wxString, ColourMap> m_ColourMaps;

	static bool RectInRectangles(wxRect& r, std::vector<wxRect>& rects);

};
 
