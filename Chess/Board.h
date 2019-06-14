#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Chess.h"
#include "Step.h"
#include <QVector>

class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    /* 棋子参数 */
    Stone _stone[32];
    int _iR;
    QPoint _offset; //偏移量
    bool _bSide;
    bool _bToWin;

    QVector<Step*> _steps;

    // 黑红属性
    int _iSelectid; //选择的棋子
    bool _bRedTurn;
    void init(bool bRedSide);


    void paintEvent(QPaintEvent *);
    void drawBoard(QPainter& p);//棋盘
    void drawSlash(QPainter& p);//画X
    void drawChess(QPainter& p);//棋子
    void drawChess(QPainter& p, int id);

    QPoint center(int row, int col);
    QPoint center(int id);
    QPoint topLeft(int row, int col);
    QPoint topLeft(int id);
    QRect cell(int row, int col);
    QRect cell(int id);
    bool getRowCol(QPoint pt, int& row, int& col);
    QString name(int id);
    bool red(int id);
    bool sameSide(int id1, int id2);
    int getChessId(int row, int col);
    void killChess(int id);
    void reliveChess(int id);
    void moveChess(int moveid, int row, int col);
    bool beDead(int id);


    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void SelectChess(int id);
    void tryMoveChess(int killid, int row, int col);
    void moveChess(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void backOne();
    void back(Step* step);
    virtual void back();

    //走棋规则
    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);

    bool canSelect(int id);//选棋子
    //两棋子之间是否有其他棋子
    int getChessCountAtLine(int row1, int col1, int row2, int col2);
    int relation(int row1, int col1, int row, int col);//横纵坐标关系
    bool isBottomSide(int id);//是否在棋盘下方

signals:

public slots:
    void slotBack();

};

#endif // BOARD_H
