#include "transform.h"

void drawCube(double originX, double originY, double originZ, double size, int glBeginType, float red,float green,float blue)
{
	double half = size / 2.0;
	double front     = originZ - half;
	double back      = originZ + half;
	double left      = originX - half;
	double right     = originX + half;
	double bottom    = originY - half;
	double top       = originY + half;

	glPushMatrix();

	//Front Face
	glBegin(glBeginType);
	glColor3f(red,green,blue);
	glVertex3f(right, bottom, front);
	glVertex3f(right, top,    front);
	glVertex3f(left,  top,    front);
	glVertex3f(left,  bottom, front);
	glEnd();

	//Back Face
	glBegin(glBeginType);
	glColor3f(red,green,blue);
	glVertex3f(right, bottom, back);
	glVertex3f(right, top,    back);
	glVertex3f(left,  top,    back);
	glVertex3f(left,  bottom, back);
	glEnd();

	//Right Face
	glBegin(glBeginType);
	glColor3f(red,green,blue);
	glVertex3f(right, bottom, front);
	glVertex3f(right, top,    front);
	glVertex3f(right, top,    back);
	glVertex3f(right, bottom, back);
	glEnd();

	//Left Face
	glBegin(glBeginType);
	glColor3f(red,green,blue);
	glVertex3f(left, bottom, back);
	glVertex3f(left, top,    back);
	glVertex3f(left, top,    front);
	glVertex3f(left, bottom, front);
	glEnd();

	//Top Face
	glBegin(glBeginType);
	glColor3f(red,green,blue);
	glVertex3f(right, top, back);
	glVertex3f(right, top, front);
	glVertex3f(left,  top, front);
	glVertex3f(left,  top, back);
	glEnd();

	//Bottom Face
	glBegin(glBeginType);
	glColor3f(red,green,blue);
	glVertex3f(right, bottom, front);
	glVertex3f(right, bottom, back);
	glVertex3f(left,  bottom, back);
	glVertex3f(left,  bottom, front);
	glEnd();

	glPopMatrix();
}

struct window{
	int xMax,yMax,xMin,yMin;
	void assign(int x0,int y0, int x1, int y1){
		xMin = x0;
		yMin = y0;
		xMax = x1;
		yMax = y1;
	}
};

const int inside = 0; // 0000
const int LEFT = 8;   // 1000
const int RIGHT = 4;  // 0100
const int BOTTOM = 2; // 0010
const int TOP = 1;    // 0001
 
int bitCalc(double x, double y, window win)
{
        int code;
 
        code = inside;          // initialised as being inside of clip window
 
        if (x < win.xMin)           // to the left of clip window
                code |= LEFT;
        else if (x > win.xMax)      // to the right of clip window
                code |= RIGHT;
        if (y < win.yMin)           // below the clip window
                code |= BOTTOM;
        else if (y > win.yMax)      // above the clip window
                code |= TOP;
        return code;
}
 
// Cohen–Sutherland clipping algorithm is used to clip a line 
// P0 = (x0, y0) to P1 = (x1, y1) wrt a rectangle (xMin, yMin) to (xMax, yMax).
void clipping(double x0, double y0, double z0, double x1, double y1, double z1, window win, float red,float green,float blue,int size)
{
        bool draw = 0;
 
        while (1) {
			//codes for P0, P1
			int code0 = bitCalc(x0, y0, win);
			int code1 = bitCalc(x1, y1, win);
            if (code0 & code1) { //Completely outside. Break.
                break;
			}
			else if (!(code0 | code1)) { //Completely inside. Set draw and break
                draw = 1;
                break;
            }
			 else {
                //calculating the line segment to clip
                double x, y;
				int codeOut;
                // At least one endpoint is outside the clip rectangle so select that point.
                if(code0 == 0)
					codeOut = code1;
				else
					codeOut = code0;
                //find the intersection point using the line equation formed using 2 points.
                if (codeOut & RIGHT) {
                    x = win.xMax;
                    y = y0 + (y1 - y0) * (win.xMax - x0) / (x1 - x0);
                } else if (codeOut & LEFT) {
                    x = win.xMin;
                    y = y0 + (y1 - y0) * (win.xMin - x0) / (x1 - x0);
                } else if (codeOut & TOP) {
                    x = x0 + (x1 - x0) * (win.yMax - y0) / (y1 - y0);
                    y = win.yMax;
                } else if (codeOut & BOTTOM) {
                    x = x0 + (x1 - x0) * (win.yMin - y0) / (y1 - y0);
                    y = win.yMin;
				}
                // Now we move outside point to intersection point to clip
                // and get ready for next pass.
                if (codeOut == code0) {
                    x0 = x;
                    y0 = y;
                } else {
                    x1 = x;
                    y1 = y;
                }
            }
        }
        if (draw)
		{
			if(y1 - y0 < 0)
			{
				int glBeginType = GL_POLYGON;
				drawCube(x0,y1+(1.0)/2.0,z0,size,glBeginType,red,green,blue);
			}
        }
}
