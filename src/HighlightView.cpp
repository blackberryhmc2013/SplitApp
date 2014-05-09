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

#include "HighlightView.h"

#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>

const bb::cascades::Color HighlightView::myColor = bb::cascades::Color::Green;
const bb::cascades::Color HighlightView::othersColor = bb::cascades::Color::DarkBlue;

HighlightView::HighlightView(int width, int height, int x, int y, bb::cascades::Color color, bool movable, Highlight* dat)
	: m_movable(movable), m_datPtr(dat) {
	m_rootContainer = new bb::cascades::Container();
	m_rootContainer -> setBackground(color);
	m_rootContainer -> setOpacity(.2);
	m_rootContainer -> setPreferredSize(width,height);
	m_rootContainer -> setLayout(new bb::cascades::DockLayout);
	m_rootContainer -> setImplicitLayoutAnimationsEnabled(false);

	bb::cascades::ImageView* handle = new bb::cascades::ImageView();
	handle -> setPreferredSize(100,100);
	handle -> setImageSource(QUrl("asset:///images/topLeft.png"));
	handle -> setImplicitLayoutAnimationsEnabled(false);
	m_rootContainer->add(handle);

	bb::cascades::ImageView* handleb =  new bb::cascades::ImageView();
	handleb -> setPreferredSize(100,100);
	handleb -> setImageSource(QUrl("asset:///images/bottomRight.png"));
	handleb -> setVerticalAlignment(bb::cascades::VerticalAlignment::Bottom);
	handleb -> setHorizontalAlignment(bb::cascades::HorizontalAlignment::Right);
	handleb -> setImplicitLayoutAnimationsEnabled(false);
	m_rootContainer->add(handleb);

	m_absLayout = bb::cascades::AbsoluteLayoutProperties::create();
	this->setLayoutProperties(m_absLayout);

	m_absLayout->setPositionX(x);
	m_absLayout->setPositionY(y);

	this->setImplicitLayoutAnimationsEnabled(false);
	this->setOverlapTouchPolicy(bb::cascades::OverlapTouchPolicy::Deny);

	setRoot(m_rootContainer);
	connect(m_rootContainer, SIGNAL(touch(bb::cascades::TouchEvent*)),
	                  this, SLOT(onTouched(bb::cascades::TouchEvent*)));
}

void HighlightView::setBotRight(int x, int y)
{
	// Force the container to be a minimum size of handleX,Y so it doesnt get too small
	m_rootContainer->setPreferredWidth (std::max( int(handleX) , x - topLeftX()));
	m_rootContainer->setPreferredHeight(std::max( int(handleY) , y - topLeftY()));
}


int HighlightView::topLeftX() const {
	return m_absLayout->positionX();
}

int HighlightView::topLeftY() const {
	return m_absLayout->positionY();
}

int HighlightView::botRightX() const {
	return m_absLayout->positionX() + m_rootContainer->preferredWidth();
}

int HighlightView::botRightY() const {
	return m_absLayout->positionY() +  m_rootContainer->preferredHeight();
}


bool HighlightView::isInsideArea(int x, int y) const {
	return (x > topLeftX() && x < botRightX() && y > topLeftY() && y < botRightY());
}

void HighlightView::showAsMine() {
	m_rootContainer -> setBackground(myColor);
}

void HighlightView::showAsOthers() {
	m_rootContainer -> setBackground(othersColor);
}


void HighlightView::onTouched(bb::cascades::TouchEvent* touchEvent)
{
	if(!m_movable)
		return;

	if (touchEvent->isMove()) {
		// Disable resizing for now
		int currWidth = m_rootContainer->preferredWidth();
		int currHeight = m_rootContainer->preferredHeight();
		if (touchEvent->localX() < (handleX * 2) && touchEvent->localY() < (handleY * 2)) {
			m_absLayout->setPositionX(m_absLayout->positionX() + touchEvent->localX() - handleX);
			m_absLayout->setPositionY(m_absLayout->positionY() + touchEvent->localY() - handleY);

			m_rootContainer->setPreferredWidth(std::max(handleX*4, m_rootContainer->preferredWidth() - (touchEvent->localX() - handleX)));
			m_rootContainer->setPreferredHeight(std::max(handleY*2 + 10, m_rootContainer->preferredHeight() - (touchEvent->localY() - handleY)));
		} else if (touchEvent->localX() > (currWidth - handleX * 2) && touchEvent->localY() > (currHeight - handleY * 2)) {
			m_rootContainer->setPreferredWidth(std::max(handleX*4, currWidth + touchEvent->localX() - (currWidth - handleX)));
			m_rootContainer->setPreferredHeight(std::max(handleX*2 + 10, currHeight + touchEvent->localY() - (currHeight - handleY)));
		} else {
			// ?? can we get position inside the parent container?
			m_absLayout->setPositionX(touchEvent->screenX() - m_rootContainer->preferredWidth()/2);
			m_absLayout->setPositionY(touchEvent->screenY()-140 - m_rootContainer->preferredHeight()/2);
		}
	}

}
