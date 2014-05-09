/* Copyright (c) 2013-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HIGHLIGHTVIEW_H_
#define HIGHLIGHTVIEW_H_

#include <bb/cascades/Container>
#include <bb/cascades/CustomControl>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/DockLayout>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/Color>
#include "Highlight.h"

class HighlightView: public bb::cascades::CustomControl {
	Q_OBJECT

public:
	HighlightView(int width, int height, int x, int y, bb::cascades::Color color, bool movable, Highlight* dat);
	int topLeftX() const;
	int topLeftY() const;
	int botRightX() const;
	int botRightY() const;
	void setBotRight(int x, int y);
	bool isInsideArea(int x, int y) const;
	Highlight* m_datPtr;
	void showAsMine();
	void showAsOthers();
	static const bb::cascades::Color myColor;
	static const bb::cascades::Color othersColor;

private Q_SLOTS:
	void onTouched(bb::cascades::TouchEvent *touchEvent);

private:
	bb::cascades::Container* m_rootContainer;
	bb::cascades::AbsoluteLayoutProperties* m_absLayout;
	static const float handleX = 50.0;
	static const float handleY = 50.0;
	bool m_movable;

};

#endif /* HIGHLIGHTVIEW_H_ */
