#include "canvas.h"

MainCanvas::MainCanvas(CWnd* parentWnd, const CRect &rect)
{
	this->frame_LT = new LeftTopFrame(parentWnd, 
		CRect(0, 0, rect.Width() / 2, rect.Height() / 2)
		);

	this->frame_RT = new RightTopFrame(parentWnd, CRect(
		rect.Width() / 2, 0, rect.Width(), rect.Height() / 2));

	this->frame_LB = new LeftBottomFrame(parentWnd,
		CRect(0, rect.Height() / 2, rect.Width() / 2, rect.Height()));
	
	this->frame_RB = new RightBottomFrame(parentWnd,
		CRect(rect.Width() / 2, rect.Height() / 2, rect.Width(), rect.Height()));
}

MainCanvas::~MainCanvas()
{
	delete frame_LB;
	delete frame_LT;
	delete frame_RB;
	delete frame_RT;
}
