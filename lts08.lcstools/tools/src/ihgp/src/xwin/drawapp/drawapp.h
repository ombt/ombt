#ifndef __DRAWAPP_H
#define __DRAWAPP_H

/* draw or editing window */
#define EDITING_WINDOW 0

/* shapes */
#define DRAW_LINE 1
#define DRAW_POINTS 2
#define DRAW_RECT 3
#define FILL_RECT 4
#define DRAW_OVAL 5
#define FILL_OVAL 6
#define DRAW_TEXT 7

/* drawing modes */
#define NOT_DRAWING 0
#define DRAWING 1

/* drawing colors */
#define DRAWING_BLACK 8
#define DRAWING_WHITE 9

/* action windows */
#define WIPE_OUT 10
#define UNDO_LAST 11
#define SAVE_FILE 12
#define LOAD_FILE 13
#define QUIT_ID 14

/* bitmap-saved flags */
#define BITMAP_SAVED 0
#define BITMAP_CHANGED 1

/* functions */
extern int CreateDrawPixmap();
extern void DestroyDrawPixmap();
extern int CreateUndoPixmap();
extern void DestroyUndoPixmap();
extern void MaintainUndo();
extern void Undo();
extern void SetDrawingShape();
extern void CreateButton();
extern void EventLoop();
extern int main();
extern void DrawItem();
extern void DrawShape();
extern void SetDrawAppColor();
extern void PopUpLoadFile();
extern void LoadFile();
extern void LoadDrawappFile();
extern void CheckForFileLoad();
extern void SaveFile();
extern void SetFileName();
extern void Quit();
extern void ReallyQuit();
extern void HandleMetaKey();
extern void CreateEditingGraphics();
extern void CreateEditingWindow();
extern void ResizeEditingWindow();
extern void EditingDrawFunc();
extern void EditingUserFunc();
extern void EditingEventFunc();
extern void SetDrawingColor();
extern void WipeOut();
extern void EditText();

#endif
