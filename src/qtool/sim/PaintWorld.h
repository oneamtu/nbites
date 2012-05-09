/**
 * @class PaintWorld
 *
 * A simple class that paints world state
 *
 * @author Octavian Neamtu
 */

#pragma once

#include "image/BMPImage.h"
#include "image/Color.h"

#include "overseer/GroundTruth.h"
#include "FieldConstants.h"

#include "WorldModel.h"

namespace qtool {
namespace sim {

class PaintWorld : public image::BMPImage {

    Q_OBJECT

    static const int BALL_DIM = 3;
    static const int ROBOT_DIM = 9;

public:
    PaintWorld(WorldModel* worldModel, QObject* parent) :
        BMPImage(parent), worldModel(worldModel) {

        bitmap = QPixmap(getWidth(), getHeight());
    }

    virtual unsigned getWidth() const { return FIELD_WIDTH; }
    virtual unsigned getHeight() const { return FIELD_HEIGHT; }

protected:
    virtual void buildBitmap() {

        bitmap.fill(Qt::transparent);

        QPainter painter(&bitmap);

        QColor orange = QColor::fromRgb(image::Color_RGB[image::Orange]);
        painter.setPen(orange);
        painter.setBrush(orange);
        painter.drawEllipse(QPoint(worldModel->ball.getX(),
                                   worldModel->ball.getY()),
                                   BALL_DIM, BALL_DIM);

        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);
        painter.drawEllipse(QPoint(worldModel->robot.getX(),
                                   worldModel->robot.getY()),
                                   ROBOT_DIM, ROBOT_DIM);

    }

protected:
    WorldModel* worldModel;

};

}
}
