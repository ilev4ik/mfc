#ifndef CANVAS
#define CANVAS

#include <afxwin.h>
#include "frame.h"

class MainCanvas: public CFrameWnd
{
public:
	MainCanvas(CWnd*, const CRect &);
	~MainCanvas();
private:
	LeftTopFrame*		frame_LT;	// top left
	RightTopFrame*		frame_RT;	// top right
	LeftBottomFrame*	frame_LB;	// bottom left
	RightBottomFrame*	frame_RB;	// bottom right

	//DECLARE_MESSAGE_MAP()
};

#endif