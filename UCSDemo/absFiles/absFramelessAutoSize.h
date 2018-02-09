#ifndef ABSFRAMELESSAUTOSIZE_H
#define ABSFRAMELESSAUTOSIZE_H

#include <QMouseEvent>
#include <QWidget>

typedef enum CursorPos
{
    Default,
    Right,
    Left,
    Bottom,
    Top,
    TopRight,
    TopLeft,
    BottomRight,
    BottomLeft
} CursorPos;

typedef struct pressWindowState
{
    /* window move */
    bool    MousePressed;
    /* window resize */
    bool    IsPressBorder;
    QPoint  MousePos;
    QPoint  WindowPos;
    QSize   PressedSize;
} pressWindowState;

class AbsFrameLessAutoSize : public QWidget
{
    Q_OBJECT
public:
    explicit AbsFrameLessAutoSize(QWidget *parent = 0);
    virtual ~AbsFrameLessAutoSize() {}
    inline void setBorder(int border);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    int m_border;
private:
    /* for resize top window */
    void mouseMoveRect(const QPoint &p);


private:    
    pressWindowState m_state;
    CursorPos m_curPos;

};

#endif // ABSFRAMELESSAUTOSIZE_H
