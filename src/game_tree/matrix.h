/***************************************************************************
 *   Copyright (C) 2009 by The qGo Project                                 *
 *                                                                         *
 *   This file is part of qGo.   					   *
 *                                                                         *
 *   qGo is free software: you can redistribute it and/or modify           *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 *   or write to the Free Software Foundation, Inc.,                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef MATRIX_H
#define MATRIX_H

#include "defines.h"

/*
* Marks used in editing a game
*/
/* FIXME, I multiplied these by 10 because they prevented the use of countLiberties
 * by checkfalseEye by corrupting the stone.  It seems to me rather silly to
 * have defines like these in addition to the marking system, but they are sort
 * of internal marks.  At any rate, things still seem to work so I guess
 * this fixed things.  But its still ugly and there could still be more problems */
#define MARK_TERRITORY_VISITED    0x0800
#define MARK_TERRITORY_DAME       0x0400
#define MARK_SEKI                 0x1000
#define MARK_TERRITORY_DONE_BLACK 0x0200
#define MARK_TERRITORY_DONE_WHITE 0x0100

//first nibble is for stone color/erase
#define MX_STONEDEAD	0x8000
#define MX_STONEEDIT	0x4000

// #define MX_STONEDIRTY	0x2000 // free

class Group;

struct ASCII_Import
{
    char blackStone, whiteStone, starPoint, emptyPoint, hBorder, vBorder;
};

class Matrix
{
public:
    Matrix(int s=DEFAULT_BOARD_SIZE);
    Matrix(const Matrix &m, bool cleanup = false);
    ~Matrix();
	int getSize() const { return size; }
	void clear();
	void insertStone(int x, int y, StoneColor c, bool fEdit = false);
	StoneColor getStoneAt(int x, int y);
    bool isStoneDead(int x, int y);
    void stoneUpdated(int x, int y);
    MarkType getMarkAt(int x, int y);
	QString getFirstTextAvailable(MarkType t);

	void insertMark(int x, int y, MarkType t);
	void removeMark(int x, int y);
	void setMarkText(int x, int y, const QString &txt);
	const QString getMarkText(int x, int y);
	unsigned short at(int x, int y) const;
	void set(int x, int y, int n);
	void clearAllMarks();
	void clearTerritoryMarks();
	void absMatrix();
	const QString saveMarks();
	const QString saveEditedMoves(Matrix *parent=0);
	const QString printMe(ASCII_Import *charset);

    void traverseTerritory( int x, int y, StoneColor &col);
    int checkStoneCaptures(StoneColor ourColor, int x, int y, Group ** gm, std::vector<Group *> &visited) const;
    void removeGroup(Group * g, Group ** gm, Group * killer);
    void removeStoneFromGroups(int x, int y, StoneColor ourColor, Group ** gm);
    void invalidateChangedGroups(Matrix & m, Group ** gm);
    void invalidateAdjacentGroups(int x, int y, Group ** gm);
    void invalidateAllGroups(Group ** gm);

	bool checkfalseEye( int x, int y, StoneColor col);
	void toggleGroupAt( int x, int y );
	void toggleStoneAt(int x, int y);
	void markStoneDead(int x, int y);
	void markGroupDead(int x, int y);
	void markGroupAlive(int x, int y);
	void toggleAreaAt( int x, int y );
	void markAreaDead(int x, int y);
	void markAreaAlive(int x, int y);
	void updateDeadMarks(int &black, int &white);
    int countDeadWhite();
    int countDeadBlack();

    static const QString coordsToString(int x, int y)
    { return QString(QChar(static_cast<const char>('a' + x))).append(QChar(static_cast<const char>('a' + y))); }

    int makeMoveIfNotSuicide(int x, int y, StoneColor c, Group **gm);
protected:
    friend class Group;

    // This function returns a list of keys of points adjacent to the point "key".
    std::vector<int> getNeighbors(int key) const;

private:
    int internalCoordsToKey(int i, int j) const { return i*size + j; }
    int coordsToKey(int x, int y) const { return internalCoordsToKey(x-1,y-1); }
    void keyToCoords(int key, int &x, int &y) const
    { x = key / size; y = key - (x++)*size + 1; }

    unsigned short at(int key) const;
    void set(int key, int n);
    StoneColor getStoneAt(int key) const;
    MarkType getMarkAt(int key) const;
    void insertStone(int key, StoneColor c, bool fEdit = false);
    Group* assembleGroup(int key, StoneColor c, Group ** gm = NULL) const;
    void toggleStoneAt(int key);
    void traverseTerritory( int key, StoneColor &col);
    bool checkfalseEye( int key, StoneColor col);

    void findInvalidAdjacentGroups(Group * g, Group ** gm, std::vector<Group*> & groupList);
    int sharedLibertyWithGroup(int x, int y, Group * g, Group ** gm);
    Group* checkNeighbour(int key, StoneColor color, Group *group, Group ** gm = NULL, std::vector<int> * libertyList = NULL) const;
    int countLiberties(Group *group);
    int countScoredLiberties(Group *group);
    Group* assembleAreaGroups(int key, StoneColor c);

    unsigned short * matrix;
    const int size;
    QHash<int,QString> markTexts;
};

#endif
