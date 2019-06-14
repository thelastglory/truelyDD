#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include<QMessageBox>
#include<QApplication>
#include<QTimer>
#include <QVBoxLayout>
#define GetRowCol(__row, __col, __id) \
    int __row = _stone[__id]._row;\
    int __col = _stone[__id]._col

Board::Board(QWidget *parent) : QFrame(parent)
{
    this->_iR = 20;
    setMinimumSize(_iR*18+1, _iR*20+1);
    init(true);
}

Board::~Board()
{

}

void Board::init(bool bRedSide)
{
    for(int i=0; i<32; ++i)
    {
        _stone[i].init(i);
    }
    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            _stone[i].rotate();
        }
    }

    _iSelectid = -1;
    _bRedTurn = true;
    _bSide = bRedSide;
    update();
}

void Board::paintEvent(QPaintEvent *)
{
    int r = height()/20;
    _iR = r;
    _offset = QPoint(r+1, r+1);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.save();
    drawBoard(painter);
    painter.restore();

    painter.save();
    drawSlash(painter);
    painter.restore();

    painter.save();
    drawChess(painter);
    painter.restore();
}

void Board::drawChess(QPainter &p)
{
    for(int i=0; i<32; i++)
    {
        drawChess(p, i);
    }
}

bool Board::beDead(int id)
{
    if(id == -1){
        return true;
    }
    return _stone[id]._dead;
}

void Board::drawChess(QPainter &p, int id)
{
    if(beDead(id)){
         return;
    }

    QColor color;
    if(red(id)) color = Qt::red;
    else color = Qt::black;

    p.setPen(QPen(QBrush(color), 2));

    if(id == _iSelectid) p.setBrush(Qt::gray);
    else p.setBrush(Qt::yellow);

    p.drawEllipse(cell(id));

    p.setFont(QFont("system", _iR-5, 700));
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));
}

QString Board::name(int id)
{
    return _stone[id].name();
}

bool Board::red(int id)
{
    return _stone[id]._red;
}


void Board::drawSlash(QPainter &p)
{
    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));

    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}

void Board::drawBoard(QPainter &p)
{
    for(int i=0; i<10; ++i)
    {
        if(i==0 || i==9)
        {
            QPen pen;
            pen.setWidth(3);
            p.setPen(pen);
        }
        else
        {
            QPen pen;
            pen.setWidth(1);
            p.setPen(pen);
        }
        p.drawLine(center(i, 0), center(i, 8));
    }

    for(int i=0; i<9; ++i)
    {
        if(i==0 || i==8)
        {
            QPen pen;
            pen.setWidth(3);
            p.setPen(pen);
            p.drawLine(center(0, i), center(9, i));
        }
        else
        {
            QPen pen;
            pen.setWidth(1);
            p.setPen(pen);
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }
}

QPoint Board::center(int row, int col)
{
    QPoint pt(_iR*col*2, _iR*row*2);
    return pt + _offset;
}

QPoint Board::center(int id)
{
    return center(_stone[id]._row, _stone[id]._col);
}

QPoint Board::topLeft(int row, int col)
{
    return center(row, col) - QPoint(_iR, _iR);
}

QPoint Board::topLeft(int id)
{
    return center(id) - QPoint(_iR, _iR);
}

QRect Board::cell(int row, int col)
{
    return QRect(topLeft(row, col), QSize(_iR*2-1, _iR*2-1));
}

QRect Board::cell(int id)
{
    return QRect(topLeft(id), QSize(_iR*2-1, _iR*2-1));
}

bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            QPoint distance = center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < _iR* _iR)
                return true;
        }
    }
    return false;
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }

    click(ev->pos());
}

bool Board::canSelect(int id)
{
    return _bRedTurn == _stone[id]._red;
}

void Board::SelectChess(int id)
{
    if(id == -1){
        return;
    }

    if(!canSelect(id))
        return;

    _iSelectid = id;
    update();
}

bool Board::sameSide(int id1, int id2)
{
    if(id1 == -1 || id2 == -1){
         return false;
    }

    return red(id1) == red(id2);
}

void Board::tryMoveChess(int killid, int row, int col)
{
    if(killid != -1 && sameSide(killid, _iSelectid))
    {
        SelectChess(killid);
        return;
    }

    bool ret = canMove(_iSelectid, killid, row, col);
    if(ret)
    {
        moveChess(_iSelectid, killid, row, col);
        _iSelectid = -1;
        update();
    }
}

int Board::getChessCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;

    if(row1 == row2 && col1 != col2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getChessId(row1, col) != -1){
                ++ret;
            }
        }
    }
    else if(row1 != row2 && col1 == col2)
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getChessId(row, col1) != -1){
                ++ret;
            }
        }
    }
    else{
        return -1;
    }

    return ret;
}

int Board::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);
}

bool Board::canMoveChe(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getChessCountAtLine(row1, col1, row, col);
    if(ret == 0){
        for(int i = 0; i < 32; i++){
            if(_stone[i]._type == Stone::JIANG && !sameSide(moveid, i)){
                if(_stone[i]._row == row || _stone[i]._col == col){
                    int i_judge = getChessCountAtLine(_stone[i]._row, _stone[i]._col, row, col);
                    if(i_judge == 0){
                           _bToWin = true;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool Board::canMoveMa(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    switch(relation(row1, col1, row, col)){
        case 12:
            if(getChessId(row1, (col+col1)/2) != -1){
                return false;
            }
            else{
                for(int i = 0; i < 32; i++){
                    if(_stone[i]._type == Stone::JIANG && !sameSide(moveid, i)){
                        int r = relation(_stone[i]._row, _stone[i]._col, row, col);
                        if(r != 12 && r != 21){
                            _bToWin = false;
                        }
                        else if(r == 12)
                        {
                            if(getChessId(row1, (col+col1)/2) != -1)
                                _bToWin = false;
                            else{
                                _bToWin = true;
                            }
                        }
                        else
                        {
                            if(getChessId((row+row1)/2, col1) != -1){
                                _bToWin = false;
                            }
                            else{
                                _bToWin = true;
                            }
                        }
                    }
                }
                return true;
            }
        case 21:
            if(getChessId((row+row1)/2, col1) != -1){
                return false;
            }
            else{

                 for(int i = 0; i < 32; i++){
                        if(_stone[i]._type == Stone::JIANG && !sameSide(moveid, i)){
                            int r = relation(_stone[i]._row, _stone[i]._col, row, col);
                            if(r != 12 && r != 21){
                                _bToWin = false;
                            }
                            else if(r == 12)
                            {
                                if(getChessId(row1, (col+col1)/2) != -1)
                                    _bToWin = false;
                                else{
                                    _bToWin = true;
                                }
                            }
                            else
                            {
                                if(getChessId((row+row1)/2, col1) != -1){
                                    _bToWin = false;
                                }
                                else{
                                    _bToWin = true;
                                }
                            }
                        }
                    }
                    return true;
           }

    }
    return false;

}

bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getChessCountAtLine(row, col, row1, col1);
    if(killid != -1)
    {
        if(ret == 1){
            for(int i = 0; i < 32; i++){
                if(_stone[i]._type == Stone::JIANG && !sameSide(moveid, i)){
                    if(_stone[i]._row == row || _stone[i]._col == col){
                        int i_judge = getChessCountAtLine(_stone[i]._row, _stone[i]._col, row, col);
                        if(i_judge == 1){
                               _bToWin = true;
                        }
                    }
                }
            }
            return true;
        }

    }
    else
    {
        if(ret == 0){
            for(int i = 0; i < 32; i++){
                if(_stone[i]._type == Stone::JIANG && !sameSide(moveid, i)){
                    if(_stone[i]._row == row || _stone[i]._col == col){
                        int i_judge = getChessCountAtLine(_stone[i]._row, _stone[i]._col, row, col);
                        if(i_judge == 1){
                               _bToWin = true;
                        }
                    }
                }
            }
            return true;
        }

    }
    return false;
}

bool Board::canMoveBing(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10){
        return false;
    }

    if(isBottomSide(moveid))
    {
        if(row1 > row){
            return true;
        }
        else if(row1 < 5 && row == row1){
            return true;
        }
    }
    else
    {
        if(row1 < row){
            return true;
        }
        else if(row1 > 4 && row == row1){
            return true;
        }
    }
    for(int i = 0; i < 32; i++){
        if(_stone[i]._type == Stone::JIANG && !sameSide(moveid, i)){
            int b = relation(_stone[i]._row, _stone[i]._col, row, col);
            if(b == 10 || b == 1){
                _bToWin = true;
            }
        }
    }
    return false;
}

bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
    if(killid != -1 && _stone[killid]._type == Stone::JIANG)
        return canMoveChe(moveid, killid, row, col);

    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r == 1 || r == 10){
        if(col >= 3 && col <= 5){
            if(isBottomSide(moveid)){
                if(row >= 7){
                    return true;
                }
            }
            else{
                if(row <= 2){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::canMoveShi(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r == 11 && col >= 3 && col <= 5){
        if(isBottomSide(moveid)){
            if(row >= 7){
                return true;
            }
        }
        else{
            if(row <= 2){
                return true;
            }
        }
    }
    return false;
}

bool Board::canMoveXiang(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r == 22){
        int rEye = (row+row1)/2;
        int cEye = (col+col1)/2;
        if(getChessId(rEye, cEye) != -1){
            return false;
        }
        else{
            if(isBottomSide(moveid)){
                if(row >= 5){
                    return true;
                }
            }
            else{
                if(row <= 4){
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::canMove(int moveid, int killid, int row, int col)
{
    _bToWin = false;
    if(sameSide(moveid, killid)) return false;


    switch (_stone[moveid]._type)
    {
    case Stone::CHE:
        return canMoveChe(moveid, killid, row, col);

    case Stone::MA:
        return canMoveMa(moveid, killid, row, col);

    case Stone::PAO:
        return canMovePao(moveid, killid, row, col);

    case Stone::BING:
        return canMoveBing(moveid, killid, row, col);

    case Stone::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case Stone::SHI:
        return canMoveShi(moveid, killid, row, col);

    case Stone::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    }
    return false;
}

void Board::reliveChess(int id)
{
    if(id==-1) return;
    _stone[id]._dead = false;
}

void Board::killChess(int id)
{
    if(id==-1) return;
    _stone[id]._dead = true;
}

bool Board::isBottomSide(int id)
{
    return _bSide == _stone[id]._red;
}

void Board::moveChess(int moveid, int row, int col)
{
    _stone[moveid]._row = row;
    _stone[moveid]._col = col;

    _bRedTurn = !_bRedTurn;
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}

void Board::moveChess(int moveid, int killid, int row, int col)
{
    saveStep(moveid, killid, row, col, _steps);

    killChess(killid);

    moveChess(moveid, row, col);
    if(_stone[killid]._type == Stone::JIANG){
        if(!sameSide(moveid, killid)){
            QMessageBox::StandardButton ret;
            ret = QMessageBox::question(NULL, "yes or not", "恭喜你赢了，是否关闭？");
            if(ret == QMessageBox::Yes){
                 QApplication* app;
                 app->exit(0);
            }
        }
    }
    if(_bToWin){
        QMessageBox* box = new QMessageBox;
        QTimer::singleShot(1000, box, SLOT(close()));

        box->setWindowTitle(tr("啦啦啦"));
        box->setIcon(QMessageBox::Warning);
        box->setText(tr("将军"));
        box->show();//////
    }

}

void Board::click(int id, int row, int col)
{
    if(this->_iSelectid == -1)
    {
        SelectChess(id);
    }
    else
    {
        tryMoveChess(id, row, col);
    }
}

void Board::click(QPoint pt)
{
    int row, col;
    bool bClicked = getRowCol(pt, row, col);
    if(!bClicked) return;

    int id = getChessId(row, col);
    click(id, row, col);
}

int Board::getChessId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(_stone[i]._row == row && _stone[i]._col == col && !beDead(i))
            return i;
    }
    return -1;
}

void Board::back(Step *step)
{
    killChess(step->_killid);
    moveChess(step->_moveid, step->_rowFrom, step->_colFrom);
}

void Board::backOne()
{
    if(this->_steps.size() == 0) return;

    Step* step = this->_steps.last();
    _steps.removeLast();
    back(step);

    update();
    delete step;
}

void Board::back()
{
    backOne();
}

void Board::slotBack()
{
    back();
}
