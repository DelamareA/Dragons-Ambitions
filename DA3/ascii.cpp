#include "Fenetre.h"
#include "constante.h"
#include "header.h"

QString Fenetre::imageToAscii(QImage image, int col, int lin){
    QVector<char> charList;
    QVector<QImage> imageList;
    coo dim;
    dim.x = image.width() / col;
    dim.y = image.height() / lin;
    QImage sobelImage(image.width(), image.height(), QImage::Format_RGB32);
    qDebug() << image.width();
    sobelImage.fill(QColor(255, 255, 255));
    QString final = "";

    for (int i = 32; i <= 126; i++){
        if (i == 32 || i == 33 || i == 35 || i == 47 || i == 61 || i == 92 || i == 95 || i == 124){
            charList.push_back((char)i);

            QImage im(dim.x, dim.y, QImage::Format_RGB32);
            im.fill(QColor(255, 255, 255));

            QPainter painter;
            painter.begin(&im);
            painter.setPen(QColor(0, 0, 0));
            painter.setFont(QFont("Courier", dim.y));
            //painter.setCompositionMode(QPainter::CompositionMode_Source);

            QString s((char) i);
            painter.drawText(0, dim.y-1, s);

            painter.end();

            //im.save(QString::number(i) + ".png");

            imageList.push_back(im);
        }
    }

    for (int y = 0; y < image.height(); y++){
        QRgb* rgb = (QRgb*)sobelImage.scanLine(y);

        for (int x = 0; x < image.width(); x++) {
            int gx = 0;
            int gy = 0;

            if (x > 0 && y > 0){
                gx += -1 * qGray(((QRgb*)image.scanLine(y-1))[x-1]);
                gy += -1 * qGray(((QRgb*)image.scanLine(y-1))[x-1]);
            }
            if (x > 0){
                gx += -2 * qGray(((QRgb*)image.scanLine(y))[x-1]);
            }
            if (x > 0 && y < image.height()-1){
                gx += -1 * qGray(((QRgb*)image.scanLine(y+1))[x-1]);
                gy +=  1 * qGray(((QRgb*)image.scanLine(y+1))[x-1]);
            }
            if (x < image.width()-1 && y > 0){
                gx +=  1 * qGray(((QRgb*)image.scanLine(y-1))[x+1]);
                gy += -1 * qGray(((QRgb*)image.scanLine(y-1))[x+1]);
            }
            if (x < image.width()-1){
                gx +=  2 * qGray(((QRgb*)image.scanLine(y))[x+1]);
            }
            if (x < image.width()-1 && y < image.height()-1){
                gx +=  1 * qGray(((QRgb*)image.scanLine(y+1))[x+1]);
                gy +=  1 * qGray(((QRgb*)image.scanLine(y+1))[x+1]);
            }
            if (y > 0){
                gy += -2 * qGray(((QRgb*)image.scanLine(y-1))[x]);
            }
            if (y < image.height()-1){
                gy +=  2 * qGray(((QRgb*)image.scanLine(y+1))[x]);
            }

            double g = sqrt(gx*gx + gy*gy);
            QRgb rgb2;

            if (g > 100){
                rgb2 = qRgb(0, 0, 0);
            }
            else {
                rgb2 = qRgb(255, 255, 255);
            }

            rgb[x] = rgb2;
        }
    }

    sobelImage.save("sobel.png");

    for (int i = 0; i < lin; i++){
        for (int j = 0; j < col; j++){
            //qDebug() << "(" << j << " , " << i << ")";
            QImage im(dim.x, dim.y, QImage::Format_RGB32);

            for (int y = 0; y < dim.y; y++){
                QRgb* rgb = (QRgb*)im.scanLine(y);
                for (int x = 0; x < dim.x; x++){
                    rgb[x] = sobelImage.pixel(j * dim.x + x, i * dim.y + y);
                }
            }

            double min = std::numeric_limits<int>::max()/2;
            int index_min = -1;

            for (int index = 0; index < charList.size(); index++){

                for (int dx = -2; dx <= 2; dx++){
                    for (int dy = -2; dy <= 2; dy++){

                        double val = 0;

                        for (int x = 0; x < dim.x; x++){
                            for (int y = 0; y < dim.y; y++){
                                if (x + dx >= 0 && x + dx < dim.x && y + dy >= 0 && y + dy < dim.y){
                                    val += abs(qGray(imageList[index].pixel(x + dx, y + dy)) - qGray(im.pixel(x, y)));
                                }
                                else {
                                    val += 60;
                                }
                            }
                        }

                        if (index == 0){
                            val *= 2;
                        }

                        if (val < min){
                            min = val;
                            index_min = index;
                        }

                    }
                }
            }



            if (index_min == -1){
                final += ' ';
            }
            else {
                final += charList[index_min];
            }
        }
        final+='\n';
    }

    QFile file("ascii.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return "";

    QTextStream out(&file);
    out << final;


    return final;
}
