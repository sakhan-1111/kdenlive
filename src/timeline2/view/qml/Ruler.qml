/*
 * Copyright (c) 2013 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    property int labelSize: fontMetrics.tightBoundingRect('00:00:00:00').width
    property int stepSize: stepSize
    property int frameSize: stepSize
    property int index: 0
    property real timeScale: timeline.scaleFactor
    property real fontUnit: root.baseUnit * 0.9

    SystemPalette { id: activePalette }

    function adjustStepSize() {
        if (timeScale > 19) {
            frameSize = timeScale
            stepSize = timeScale > labelSize * 1.3 ? timeScale : Math.floor(labelSize/timeScale+1) * timeScale
        } else {
            frameSize = labelSize / 3
            stepSize = labelSize * 4 / 3
        }
    }

    id: rulerTop
    enabled: false
    height: fontMetrics.font.pixelSize * 2
    color: activePalette.window

    Repeater {
        model: parent.width / frameSize
        Rectangle {
            anchors.bottom: rulerTop.bottom
            height: (index % 4)? ((index % 2) ? 3 : 7) : 14
            width: 1
            color: activePalette.windowText
            x: index * frameSize
        }
    }
    Repeater {
        model: parent.width / stepSize
        Label {
            anchors.top: rulerTop.top
            anchors.topMargin: 2
            x: index * stepSize + 2
            text: timeline.timecode(index * stepSize / timeScale)
            font.pointSize: fontUnit
        }
    }
}

