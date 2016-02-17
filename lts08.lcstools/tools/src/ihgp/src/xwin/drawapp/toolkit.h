#ifndef __TOOLKIT_H
#define __TOOLKIT_H

/* required headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "myxlib.h"

/* function externs */
extern int CreateToolWindow();
extern Display *ToolGetDisplay();
extern Window ToolGetWindow();
extern void ToolSetBackground();
extern void ToolSetForeground();
extern void ToolSetGC();
extern void ToolSetFont();
extern int ToolGetGroup();
extern void ToolSetGroup();
extern int ToolGetState();
extern void ToolSetState();
extern void ToolGetName();
extern void ToolSetName();
extern void ToolCenterText();
extern void ToolDrawName();
extern void ToolSetFunctions();
extern void ToolSetUserFunc();
extern void ToolExec();
extern void NullDrawFunc();
extern void NullUserFunc();
extern void NullEventFunc();
extern void ToolHighlightGroup();
extern void ToolHighlight();
extern void ToolDrawLine();
extern void ToolClear();
extern void ToolRedraw();
extern void ToolResize();
extern int ToolEvent();
extern int DispatchEvent();
extern void TextEntryEvent();
extern void TextEntryDraw();
extern int CreateTextLabel();
extern void PushButtonEvent();
extern void PushButtonDraw();
extern void UnhighlightPushButton();
extern int CreatePushButton();
extern void LabelDraw();
extern int CreateTextLabel();
extern Window CreateDialog();
extern void PopDownDialog();
extern void PopUpDialog();
extern void GetDialogData();
extern void SetDialogCallback();
extern void SetDialogCancelCallback();
extern void SetDialogData();
extern void SetDialogPrompt();
extern void QueryPointer();
extern int CreateTransientWindow();

#endif
