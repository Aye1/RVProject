#ifndef __VIEWER__
#define __VIEWER__

#include <QGLViewer/qglviewer.h>
#include <QStringList>
#include <QColor>

namespace apig {
    class Renderable;
    }

class Viewer : public QGLViewer {
    public:
        Viewer(QString fileName, QWidget* parent = NULL);
        ~Viewer();
    
    protected:
        virtual void init();
        virtual void draw();
        virtual void keyPressEvent(QKeyEvent *);
        
    private:
        apig::Renderable *object;
        bool wireframe;
        int bgrIndex;
        static const QList<QColor> backgrounds;
    };

#endif

